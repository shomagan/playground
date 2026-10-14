/*
 * Read title, keywords and description embedded in a JPEG — a port of
 * app/metadata.py, same sources and priority (first non-empty value wins):
 *
 *   1. XMP   dc:title, dc:description, dc:subject
 *   2. IPTC  2:05 ObjectName, 2:120 Caption, 2:25 Keywords
 *   3. EXIF  XPTitle / XPSubject, ImageDescription / XPComment, XPKeywords
 *   4. the file name, for the title only
 *
 * Plain script: in the extension it defines `RBMeta` on the content-script
 * global; under Node it is exported for the tests.
 */
(function (root) {
  "use strict";

  const utf8 = new TextDecoder("utf-8", { fatal: true });
  const latin1 = new TextDecoder("latin1");
  const utf16le = new TextDecoder("utf-16le");

  function decodeText(bytes) {
    try {
      return utf8.decode(bytes);
    } catch {
      return latin1.decode(bytes);
    }
  }

  function clean(s) {
    return (s || "").replace(/\s+/g, " ").trim();
  }

  /** Walk the JPEG segments before the image data: [{marker, data}]. */
  function segments(buf) {
    const v = new DataView(buf);
    if (v.byteLength < 4 || v.getUint16(0) !== 0xffd8) throw new Error("not a JPEG");
    const out = [];
    let pos = 2;
    while (pos + 4 <= v.byteLength) {
      if (v.getUint8(pos) !== 0xff) break;
      const marker = v.getUint8(pos + 1);
      if (marker === 0xd8 || (marker >= 0xd0 && marker <= 0xd7)) { pos += 2; continue; }
      if (marker === 0xda || marker === 0xd9) break; // start of scan / end: no more metadata
      const len = v.getUint16(pos + 2);
      // Clip: the caller may pass only the head of a large file.
      const size = Math.min(Math.max(0, len - 2), v.byteLength - (pos + 4));
      out.push({ marker, data: new Uint8Array(buf, pos + 4, size) });
      pos += 2 + len;
    }
    return out;
  }

  function startsWith(bytes, ascii) {
    if (bytes.length < ascii.length) return false;
    for (let i = 0; i < ascii.length; i++) if (bytes[i] !== ascii.charCodeAt(i)) return false;
    return true;
  }

  // --- XMP ------------------------------------------------------------------

  const XMP_SIG = "http://ns.adobe.com/xap/1.0/\0";
  const NS_RDF = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
  const NS_DC = "http://purl.org/dc/elements/1.1/";
  const NS_XML = "http://www.w3.org/XML/1998/namespace";

  function fromXmp(segs, parseXml) {
    const seg = segs.find((s) => s.marker === 0xe1 && startsWith(s.data, XMP_SIG));
    if (!seg || !parseXml) return {};
    let doc;
    try {
      doc = parseXml(decodeText(seg.data.subarray(XMP_SIG.length)).replace(/<\?xpacket[^>]*\?>/g, ""));
    } catch {
      return {};
    }
    if (!doc || doc.getElementsByTagName("parsererror").length) return {};
    return {
      title: xmpAlt(doc, "title"),
      description: xmpAlt(doc, "description"),
      keywords: xmpBag(doc, "subject"),
    };
  }

  function xmpAlt(doc, name) {
    for (const d of Array.from(doc.getElementsByTagNameNS(NS_RDF, "Description"))) {
      const attr = d.getAttributeNS(NS_DC, name);
      if (attr) return attr;
    }
    const el = doc.getElementsByTagNameNS(NS_DC, name)[0];
    if (!el) return "";
    const items = Array.from(el.getElementsByTagNameNS(NS_RDF, "li"));
    const def = items.find((li) => li.getAttributeNS(NS_XML, "lang") === "x-default" && li.textContent);
    if (def) return def.textContent;
    const first = items.find((li) => li.textContent);
    return first ? first.textContent : (el.textContent || "").trim();
  }

  function xmpBag(doc, name) {
    const el = doc.getElementsByTagNameNS(NS_DC, name)[0];
    if (!el) return [];
    return Array.from(el.getElementsByTagNameNS(NS_RDF, "li")).map((li) => li.textContent).filter(Boolean);
  }

  // --- IPTC (Photoshop APP13 / 8BIM 0x0404) ---------------------------------

  function fromIptc(segs) {
    const seg = segs.find((s) => s.marker === 0xed && startsWith(s.data, "Photoshop 3.0\0"));
    if (!seg) return {};
    const d = seg.data;
    const v = new DataView(d.buffer, d.byteOffset, d.byteLength);
    let pos = 14;
    while (pos + 12 <= d.length) {
      if (!startsWith(d.subarray(pos), "8BIM")) break;
      const id = v.getUint16(pos + 4);
      const nameLen = d[pos + 6];
      let p = pos + 6 + 1 + nameLen;
      if ((1 + nameLen) % 2) p++; // Pascal name padded to even length
      const size = v.getUint32(p);
      p += 4;
      if (id === 0x0404) return parseIim(d.subarray(p, p + size));
      pos = p + size + (size % 2);
    }
    return {};
  }

  function parseIim(d) {
    const out = { title: "", description: "", keywords: [] };
    let pos = 0;
    while (pos + 5 <= d.length && d[pos] === 0x1c) {
      const rec = d[pos + 1], ds = d[pos + 2];
      const len = (d[pos + 3] << 8) | d[pos + 4];
      const val = decodeText(d.subarray(pos + 5, pos + 5 + len));
      if (rec === 2 && ds === 5 && !out.title) out.title = val;
      else if (rec === 2 && ds === 120 && !out.description) out.description = val;
      else if (rec === 2 && ds === 25) out.keywords.push(val);
      pos += 5 + len;
    }
    return out;
  }

  // --- EXIF (IFD0) -----------------------------------------------------------

  function fromExif(segs) {
    const seg = segs.find((s) => s.marker === 0xe1 && startsWith(s.data, "Exif\0\0"));
    if (!seg) return {};
    const t = seg.data.subarray(6);
    const v = new DataView(t.buffer, t.byteOffset, t.byteLength);
    const le = t[0] === 0x49; // "II" little endian, "MM" big endian
    const u16 = (o) => v.getUint16(o, le);
    const u32 = (o) => v.getUint32(o, le);
    const tags = {};
    try {
      const ifd = u32(4);
      const n = u16(ifd);
      for (let i = 0; i < n; i++) {
        const e = ifd + 2 + i * 12;
        const tag = u16(e), type = u16(e + 2), count = u32(e + 4);
        const unit = { 1: 1, 2: 1, 7: 1 }[type];
        if (!unit) continue; // only BYTE / ASCII / UNDEFINED carry our text
        const size = count * unit;
        const off = size <= 4 ? e + 8 : u32(e + 8);
        tags[tag] = t.subarray(off, off + size);
      }
    } catch {
      return {};
    }
    const xp = (b) => (b ? utf16le.decode(b).replace(/\0+$/g, "").trim() : "");
    const ascii = (b) => (b ? decodeText(b).replace(/\0+$/g, "").trim() : "");
    const kw = xp(tags[0x9c9e]);
    return {
      title: xp(tags[0x9c9b]) || xp(tags[0x9c9f]),
      description: ascii(tags[0x010e]) || xp(tags[0x9c9c]),
      keywords: kw ? [kw] : [],
    };
  }

  // --- public ----------------------------------------------------------------

  function titleFromFilename(name) {
    const stem = name.replace(/\.[^.]+$/, "");
    return stem
      .split(/[\s_\-.]+/)
      .filter(Boolean)
      .map((w) => w[0].toUpperCase() + w.slice(1).toLowerCase())
      .join(" ");
  }

  function normalizeKeywords(raw) {
    const out = [];
    const seen = new Set();
    for (const item of raw) {
      for (let kw of item.split(/[,;]/)) {
        kw = clean(kw);
        if (kw && !seen.has(kw.toLowerCase())) {
          seen.add(kw.toLowerCase());
          out.push(kw);
        }
      }
    }
    return out;
  }

  /**
   * @param {ArrayBuffer} buf  JPEG bytes
   * @param {string} filename  used for the title fallback
   * @param {(xml: string) => Document} [parseXml]  DOMParser wrapper (browser)
   */
  function readMetadata(buf, filename, parseXml) {
    const segs = segments(buf);
    const sources = [fromXmp(segs, parseXml), fromIptc(segs), fromExif(segs)];
    const pick = (k) => {
      for (const s of sources) if (s[k] && s[k].length) return s[k];
      return k === "keywords" ? [] : "";
    };
    return {
      title: clean(pick("title")) || titleFromFilename(filename || ""),
      description: (pick("description") || "").trim(),
      keywords: normalizeKeywords(pick("keywords")),
    };
  }

  const api = { readMetadata, normalizeKeywords, titleFromFilename };
  if (typeof module !== "undefined" && module.exports) module.exports = api;
  else root.RBMeta = api;
})(typeof self !== "undefined" ? self : this);
