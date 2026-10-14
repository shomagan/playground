/*
 * Runs on Redbubble's "Add new work" page. When you choose (or drop) a JPEG,
 * reads its embedded title / tags / description and fills the form. It never
 * presses Save — you review and save yourself.
 */
(function () {
  "use strict";

  // Redbubble: one main tag + up to 14 supporting tags = 15 in total. The
  // file's first keyword becomes the main tag, the next 14 the supporting tags.
  const MAX_TAGS = 15;
  const MAX_TITLE = 60;

  // Fields are found by exact selector first, then by what the field is called
  // on the page (label / placeholder / aria-label / name / id). Fix here if
  // Redbubble changes its markup — "Show form fields" in the panel lists what
  // the page has.
  const FIELDS = {
    title: { sel: '#work_title_en, input[name="work[title_en]"]', label: /title/i },
    mainTag: { sel: "#work_main_tag_en", label: /main\s*tag|primary\s*tag/i },
    tags: {
      sel: '#work_tag_field_en, textarea[name="work[tag_field_en]"]',
      label: /supporting\s*tags|tags/i,
    },
    description: {
      sel: '#work_description_en, textarea[name="work[description_en]"]',
      label: /descr/i,
    },
  };
  // Choices made for every work. Option texts as shown on the page; the section
  // is found by its heading. MEDIA: Redbubble allows up to 2.
  const MEDIA = ["Photography"];
  const MATURE_ANSWER = "No"; // "Is this mature content?"

  const RIGHTS_SEL = '#rightsDeclaration, input[type="checkbox"][name*="rights" i]';

  const parseXml = (s) => new DOMParser().parseFromString(s, "application/xml");

  let current = null; // {name, meta}
  const userEdited = new Set(); // fields the person typed into: never overwrite

  // --- picking up the file ------------------------------------------------

  // Capture phase on document: works however Redbubble re-renders its inputs.
  document.addEventListener(
    "change",
    (e) => {
      const el = e.target;
      if (el instanceof HTMLInputElement && el.type === "file" && el.files && el.files[0]) {
        handleFile(el.files[0]);
      }
    },
    true,
  );
  document.addEventListener(
    "drop",
    (e) => {
      const f = e.dataTransfer && e.dataTransfer.files && e.dataTransfer.files[0];
      if (f) handleFile(f);
    },
    true,
  );
  document.addEventListener(
    "input",
    (e) => {
      if (!e.isTrusted) return;
      const fields = findFields();
      for (const key of Object.keys(fields)) {
        if (e.target === fields[key]) userEdited.add(key);
      }
    },
    true,
  );

  async function handleFile(file) {
    if (!/\.jpe?g$/i.test(file.name) && file.type !== "image/jpeg") {
      panel.show({ error: `${file.name} is not a JPEG — nothing filled.` });
      return;
    }
    try {
      const meta = RBMeta.readMetadata(await readBytes(file), file.name, parseXml);
      current = { name: file.name, meta };
      userEdited.clear();
      fillWithRetry();
    } catch (err) {
      panel.show({ error: `Could not read ${file.name}: ${err.message}` });
    }
  }

  // JPEG metadata (APP1 EXIF/XMP, APP13 IPTC) sits before the image data;
  // 4 MB is far more than any metadata block.
  const HEAD_BYTES = 4 * 1024 * 1024;

  /**
   * Read the start of the file as bytes this content script owns.
   *
   * The File comes from the page, and in a Firefox content script FileReader /
   * Response are the page's constructors too, so any ArrayBuffer they return
   * lives in the page and `new Uint8Array(buf)` dies with "Permission denied to
   * access property constructor". A string is a primitive and crosses that wall
   * freely, so read a binary string and build the bytes here.
   */
  function readBytes(file) {
    return new Promise((resolve, reject) => {
      const reader = new FileReader();
      reader.onload = () => {
        const str = String(reader.result);
        const bytes = new Uint8Array(str.length);
        for (let i = 0; i < str.length; i++) bytes[i] = str.charCodeAt(i) & 0xff;
        resolve(bytes.buffer);
      };
      reader.onerror = () => reject(new Error(String(reader.error && reader.error.message)));
      reader.readAsBinaryString(file.slice(0, HEAD_BYTES));
    });
  }

  // --- filling the form ---------------------------------------------------

  /** Everything that names a field: id, name, placeholder, aria-label, labels. */
  function describe(el) {
    const parts = [el.id, el.name, el.placeholder, el.getAttribute("aria-label")];
    for (const l of el.labels || []) parts.push(l.textContent);
    const by = el.getAttribute("aria-labelledby");
    if (by) for (const id of by.split(/\s+/)) parts.push(document.getElementById(id)?.textContent);
    // Nearby heading/label text for fields without a proper <label>.
    // Stop at the first container holding another field: past that point the
    // heading belongs to a different field.
    const FIELD = "input:not([type=hidden]), textarea, select, [contenteditable]";
    let node = el.parentElement;
    for (let depth = 0; node && depth < 3; depth++, node = node.parentElement) {
      if (Array.from(node.querySelectorAll(FIELD)).some((f) => f !== el)) break;
      const lab = node.querySelector("label, legend, h2, h3, h4, [class*='label' i]");
      if (lab && !lab.contains(el)) {
        parts.push(lab.textContent);
        break;
      }
    }
    return parts.filter(Boolean).join(" | ").replace(/\s+/g, " ").trim();
  }

  function textFields() {
    return Array.from(document.querySelectorAll("input, textarea")).filter(
      (el) =>
        (el.tagName === "TEXTAREA" || /^(text|search|)$/i.test(el.type)) &&
        !el.disabled &&
        !el.closest(".rbmf-panel") &&
        el.getClientRects().length > 0,
    );
  }

  /**
   * Diagnostics: the HTML around every "... tag(s)" heading, for widgets that
   * are not plain inputs (tag chips, contenteditable). Cleaned and shortened.
   */
  function tagBlocksHtml() {
    const heads = Array.from(document.querySelectorAll("body *")).filter(
      (n) =>
        !n.closest(".rbmf-panel") &&
        n.children.length === 0 &&
        /\b(main|supporting)?\s*tags?\b/i.test(n.textContent || "") &&
        (n.textContent || "").trim().length < 40,
    );
    const seen = new Set();
    const out = [];
    for (const h of heads) {
      // Climb until the block also contains something you can type into.
      let block = h.parentElement;
      for (let i = 0; block && i < 5; i++, block = block.parentElement) {
        if (block.querySelector("input, textarea, [contenteditable], [role='combobox'], [role='textbox']")) break;
      }
      if (!block || seen.has(block)) continue;
      seen.add(block);
      const html = block.outerHTML
        .replace(/<svg[\s\S]*?<\/svg>/g, "<svg/>")
        .replace(/\s(style|d)="[^"]*"/g, "");
      out.push(`--- "${h.textContent.trim()}" ---\n${html.slice(0, 3000)}`);
    }
    return out.join("\n\n") || "(no 'tag' headings found)";
  }

  /** Resolve every field once per fill; a field is never used for two keys. */
  function findFields() {
    const found = {};
    const used = new Set();
    // mainTag before tags: "main tag" also matches the plain /tags/ pattern.
    for (const key of ["title", "mainTag", "tags", "description"]) {
      let el = document.querySelector(FIELDS[key].sel);
      if (!el || used.has(el)) {
        el = textFields().find((f) => !used.has(f) && FIELDS[key].label.test(describe(f)));
      }
      if (el) {
        found[key] = el;
        used.add(el);
      }
    }
    return found;
  }

  /** Set a value so frameworks (React, Stimulus, plain listeners) notice it. */
  function setValue(el, value) {
    const proto = el instanceof HTMLTextAreaElement ? HTMLTextAreaElement : HTMLInputElement;
    const setter = Object.getOwnPropertyDescriptor(proto.prototype, "value").set;
    setter.call(el, value);
    el.dispatchEvent(new Event("input", { bubbles: true }));
    el.dispatchEvent(new Event("change", { bubbles: true }));
  }

  /** Shorten to MAX_TITLE at a word boundary ("…and gill", never "…and gil"). */
  function fitTitle(title) {
    if (title.length <= MAX_TITLE) return title;
    const cut = title.slice(0, MAX_TITLE + 1);
    const space = cut.lastIndexOf(" ");
    return (space > MAX_TITLE / 2 ? cut.slice(0, space) : cut.slice(0, MAX_TITLE)).trim();
  }

  // --- choice options (Media, mature content) ------------------------------

  const OPTION_SEL =
    'label, button, [role="checkbox"], [role="radio"], [role="button"], [aria-pressed], ' +
    'input[type="checkbox"], input[type="radio"]';
  const clicked = new Set(); // "file|option": options whose state we cannot read, clicked once

  const norm = (t) => (t || "").replace(/\s+/g, " ").trim().toLowerCase();

  function optionText(el) {
    if (el instanceof HTMLInputElement) {
      return Array.from(el.labels || []).map((l) => l.textContent).join(" ") || el.value;
    }
    return el.textContent;
  }

  /**
   * The element showing `text` (e.g. "No") inside the smallest block whose
   * text matches `section` (e.g. /mature/). Smallest wins: the mature question's
   * own block is far smaller than the whole form that merely *contains* it, so
   * its "No" beats the "No" of any other yes/no question on the page.
   */
  function findOption(section, text) {
    let best = null;
    let bestSize = Infinity;
    for (const el of document.querySelectorAll(OPTION_SEL)) {
      if (el.closest(".rbmf-panel") || norm(optionText(el)) !== norm(text)) continue;
      for (let node = el.parentElement; node; node = node.parentElement) {
        const t = node.textContent || "";
        if (section.test(t)) {
          if (t.length < bestSize) {
            best = el;
            bestSize = t.length;
          }
          break;
        }
      }
    }
    return best;
  }

  /** true / false when the option's state is readable, null when it is not. */
  function isSelected(el) {
    const input =
      el instanceof HTMLInputElement
        ? el
        : (el.htmlFor && document.getElementById(el.htmlFor)) || el.querySelector("input");
    if (input && /^(checkbox|radio)$/i.test(input.type)) return input.checked;
    for (const n of [el, el.querySelector("[aria-checked], [aria-pressed], [aria-selected]")]) {
      if (!n) continue;
      for (const a of ["aria-checked", "aria-pressed", "aria-selected"]) {
        const v = n.getAttribute(a);
        if (v != null) return v === "true";
      }
    }
    return null;
  }

  /** Select an option once; never toggles an already selected one off. */
  function choose(section, text) {
    const el = findOption(section, text);
    if (!el) return false;
    const state = isSelected(el);
    const key = `${current.name}|${section}|${text}`;
    if (state === true || (state === null && clicked.has(key))) return true;
    clicked.add(key);
    el.click();
    return true;
  }

  /** Split keywords: main tag + supporting tags when the page has both fields. */
  function split(keywords, hasMainTag) {
    const kw = keywords.slice(0, MAX_TAGS);
    return hasMainTag ? { mainTag: kw[0] || "", tags: kw.slice(1) } : { mainTag: "", tags: kw };
  }

  function fill() {
    if (!current) return { missing: [], hasMainTag: false };
    const fields = findFields();
    const m = current.meta;
    const eds = tagEditors();
    if (eds && tagJob.name !== current.name) fillTags(eds); // async, runs once
    const { mainTag, tags } = split(m.keywords, eds ? !!eds.main : !!fields.mainTag);
    const values = {
      title: fitTitle(m.title),
      description: m.description,
    };
    if (!eds) {
      // Older form: plain text fields for tags.
      values.mainTag = mainTag;
      values.tags = tags.join(", ");
    }
    const missing = [];
    for (const [key, value] of Object.entries(values)) {
      if (!value || userEdited.has(key)) continue;
      const el = fields[key];
      if (!el) {
        if (key !== "mainTag") missing.push(key);
        continue;
      }
      if (el.value !== value) setValue(el, value);
    }
    for (const media of MEDIA) {
      if (!choose(/\bmedia\b/i, media)) missing.push(`Media "${media}"`);
    }
    if (!choose(/mature/i, MATURE_ANSWER)) missing.push(`mature content "${MATURE_ANSWER}"`);
    const rights = document.querySelector(RIGHTS_SEL);
    if (rights && !rights.checked) rights.click();
    else if (!rights) missing.push("rights checkbox");
    return { missing, hasMainTag: eds ? !!eds.main : !!fields.mainTag };
  }

  // --- Redbubble's tag editor (React pills) --------------------------------
  //
  // Main Tag / Supporting Tags are not inputs but editable <span role=textbox>
  // elements (e.g. #main-tag-en) inside a pill container: you type a tag and
  // press Enter, and React turns it into a pill. So that is what we do, one tag
  // at a time, checking that a pill appeared before going on.

  const TAG_ROOT = '[id^="RB_React_Component_UploaderTagEd"]';
  const TAG_EDIT = '[role="textbox"], [contenteditable]:not([contenteditable="false"])';
  const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

  // Per picture: idle -> running -> done | failed. Runs once; "Fill again" resets.
  let tagJob = { name: null, state: "idle", main: "", added: 0, total: 0, failedTag: "" };

  function tagEditors() {
    const all = Array.from(document.querySelectorAll(`${TAG_ROOT} ${TAG_EDIT}`)).filter(
      (n) => n.getClientRects().length > 0,
    );
    // An editable span may sit inside a role=textbox wrapper: keep the outer one.
    const edits = all.filter((e) => !all.some((o) => o !== e && o.contains(e)));
    if (!edits.length) return null;
    const label = (e) =>
      `${e.id} ${e.closest('[class*="tag-field" i]')?.querySelector("label")?.textContent || ""}`;
    const main = edits.find((e) => /main/i.test(label(e))) || null;
    const supporting =
      edits.find((e) => e !== main && /support/i.test(label(e))) ||
      edits.find((e) => e !== main) ||
      null;
    return { main, supporting };
  }

  /** Texts of the pills already in an editor's container. */
  function pillTexts(edit) {
    const box = edit.parentElement;
    if (!box) return [];
    return Array.from(box.children)
      .filter((c) => c !== edit && !c.contains(edit))
      .map((c) => (c.textContent || "").replace(/[×✕✖]/g, "").trim())
      .filter(Boolean);
  }

  function press(el, key) {
    const keyCode = key === "Enter" ? 13 : 188;
    const code = key === "Enter" ? "Enter" : "Comma";
    for (const type of ["keydown", "keypress", "keyup"]) {
      el.dispatchEvent(
        new KeyboardEvent(type, {
          key,
          code,
          keyCode,
          which: keyCode,
          charCode: type === "keypress" ? keyCode : 0,
          bubbles: true,
          cancelable: true,
        }),
      );
    }
  }

  /** Put the caret at the end of an editable span; optionally select all of it. */
  function placeCaret(edit, selectAll = false) {
    edit.focus();
    const range = document.createRange();
    range.selectNodeContents(edit);
    if (!selectAll) range.collapse(false);
    const sel = window.getSelection();
    sel.removeAllRanges();
    sel.addRange(range);
  }

  /** Type text like a keyboard (React sees a real input event), replacing what is there. */
  function typeInto(edit, text) {
    placeCaret(edit, true);
    let typed = false;
    try {
      typed = document.execCommand("insertText", false, text);
    } catch {
      typed = false;
    }
    if (!typed) {
      edit.textContent = text;
      edit.dispatchEvent(new InputEvent("input", { bubbles: true, inputType: "insertText", data: text }));
    }
  }

  /** Offer text as a paste — tag editors often split pasted text at commas. */
  function pasteInto(edit, text) {
    placeCaret(edit);
    try {
      const data = new DataTransfer();
      data.setData("text/plain", text);
      edit.dispatchEvent(
        new ClipboardEvent("paste", { clipboardData: data, bubbles: true, cancelable: true }),
      );
    } catch {
      // no DataTransfer / ClipboardEvent support: the caller falls back to typing
    }
  }

  const typedText = (edit) => (edit.textContent || "").trim();

  /**
   * Try to turn the typed text into pill(s): Enter, comma, then leaving the
   * field. Returns how many new pills appeared. Text that did not become a pill
   * is put back if leaving the field cleared it, so the field is never emptied.
   */
  async function commit(edit, text, before) {
    for (const key of ["Enter", ","]) {
      press(edit, key);
      await sleep(200);
      if (pillTexts(edit).length > before) return pillTexts(edit).length - before;
    }
    edit.blur();
    edit.dispatchEvent(new FocusEvent("focusout", { bubbles: true }));
    await sleep(250);
    const added = pillTexts(edit).length - before;
    if (added <= 0 && !typedText(edit)) typeInto(edit, text);
    return Math.max(0, added);
  }

  /** Empty the editable span the way a user deleting the text would. */
  function clearText(edit) {
    if (!typedText(edit)) return;
    placeCaret(edit, true);
    let done = false;
    try {
      done = document.execCommand("delete", false);
    } catch {
      done = false;
    }
    if (!done || typedText(edit)) {
      edit.textContent = "";
      edit.dispatchEvent(new InputEvent("input", { bubbles: true, inputType: "deleteContentBackward" }));
    }
  }

  /**
   * One tag into Main Tag: "pill" when committed, "typed" when only written.
   * Synthetic Enter does not commit here, but the editor does take pasted or
   * comma-terminated text (that is how Supporting Tags get their pills), so try
   * those first, clearing the field between attempts.
   */
  async function fillMain(edit, tag) {
    const before = pillTexts(edit).length;
    const committed = () => pillTexts(edit).length > before;
    const attempts = [
      () => pasteInto(edit, tag),
      () => pasteInto(edit, `${tag},`),
      () => typeInto(edit, `${tag},`),
    ];
    for (const attempt of attempts) {
      clearText(edit);
      attempt();
      await sleep(300);
      if (committed()) {
        clearText(edit); // drop a leftover "," if the editor kept one
        return "pill";
      }
    }
    clearText(edit);
    typeInto(edit, tag);
    await sleep(80);
    return (await commit(edit, tag, before)) > 0 ? "pill" : "typed";
  }

  /** All supporting tags at once, comma separated: paste first, then type. */
  async function fillSupporting(edit, tags) {
    const line = tags.join(", ");
    const before = pillTexts(edit).length;
    pasteInto(edit, line);
    await sleep(400);
    if (pillTexts(edit).length > before) return { state: "pills", added: pillTexts(edit).length - before };
    typeInto(edit, line);
    await sleep(100);
    const added = await commit(edit, line, before);
    return added > 0 ? { state: "pills", added } : { state: "typed", added: 0 };
  }

  async function fillTags(eds) {
    const name = current.name;
    const kw = current.meta.keywords.slice(0, MAX_TAGS);
    const mainTag = eds.main ? kw[0] || "" : "";
    const rest = eds.main ? kw.slice(1) : kw;
    tagJob = { name, state: "running", main: mainTag, mainState: "", supState: "", added: 0, total: rest.length };
    panel.refresh();
    const stillCurrent = () => current && current.name === name;
    try {
      if (eds.main && mainTag) {
        const have = pillTexts(eds.main);
        if (have.length) {
          tagJob.main = have[0]; // already set: leave it
          tagJob.mainState = "pill";
        } else {
          tagJob.mainState = await fillMain(eds.main, mainTag);
        }
      }
      if (eds.supporting && rest.length && stillCurrent()) {
        const have = pillTexts(eds.supporting).map((h) => h.toLowerCase());
        const todo = rest.filter((t) => !have.includes(t.toLowerCase()));
        const res = todo.length ? await fillSupporting(eds.supporting, todo) : { state: "pills", added: 0 };
        tagJob.supState = res.state;
        tagJob.added = rest.length - todo.length + res.added;
      }
      tagJob.state = "done";
    } catch (err) {
      tagJob.state = "failed";
      tagJob.error = String(err && err.message);
    }
    if (stillCurrent()) panel.refresh();
  }

  /**
   * Fill now and a few more times over the next seconds: the fields may render
   * only after the upload starts, and Redbubble may pre-fill the title from the
   * file name once its own processing finishes. Fields you typed into are left alone.
   */
  function fillWithRetry() {
    const name = current.name;
    let res = fill();
    panel.show({ meta: current.meta, name, ...res });
    for (const delay of [1000, 3000, 6000, 12000, 20000]) {
      setTimeout(() => {
        if (!current || current.name !== name) return;
        res = fill();
        panel.show({ meta: current.meta, name, ...res });
      }, delay);
    }
  }

  // --- status panel -------------------------------------------------------

  const panel = (() => {
    let box;
    const el = (tag, cls, text) => {
      const n = document.createElement(tag);
      if (cls) n.className = cls;
      if (text != null) n.textContent = text;
      return n;
    };
    function ensure() {
      if (box && document.body.contains(box)) return box;
      box = el("div", "rbmf-panel");
      document.body.appendChild(box);
      return box;
    }
    let last = null;
    function refresh() {
      if (last) show(last);
    }
    function show(args) {
      last = args;
      const { meta, name, missing = [], hasMainTag = false, error } = args;
      const b = ensure();
      b.replaceChildren();
      const head = el("div", "rbmf-head", "Metadata filler");
      const close = el("button", "rbmf-close", "×");
      close.title = "Hide";
      close.onclick = () => b.remove();
      head.appendChild(close);
      b.appendChild(head);
      if (error) {
        b.appendChild(el("div", "rbmf-error", error));
        return;
      }
      b.appendChild(el("div", "rbmf-file", name));
      const row = (label, value) => {
        const r = el("div", "rbmf-row");
        r.appendChild(el("span", "rbmf-label", label));
        r.appendChild(el("span", value ? "rbmf-value" : "rbmf-value rbmf-empty", value || "— none in file"));
        b.appendChild(r);
      };
      const title = fitTitle(meta.title);
      row(title === meta.title ? "Title" : `Title (shortened to ${MAX_TITLE} chars)`, title);
      const { mainTag, tags } = split(meta.keywords, hasMainTag);
      const dropped = Math.max(0, meta.keywords.length - MAX_TAGS);
      const over = dropped ? `, ${dropped} over the limit dropped` : "";
      if (hasMainTag) {
        row("Main tag", mainTag);
        row(`Supporting tags (${tags.length}${over})`, tags.join(", "));
      } else {
        row(`Tags (${tags.length}${over})`, tags.join(", "));
      }
      row("Description", meta.description);
      if (tagJob.name === name && tagJob.state !== "idle") {
        const t = tagJob;
        if (t.state === "running") {
          b.appendChild(el("div", "rbmf-ok", "Adding tags…"));
        } else if (t.state === "failed") {
          b.appendChild(el("div", "rbmf-warn", `Tags failed: ${t.error}. Press Fill again.`));
        } else {
          const lines = [];
          if (t.mainState === "pill") lines.push(`Main tag "${t.main}" added.`);
          if (t.mainState === "typed") lines.push(`Main tag "${t.main}" typed in.`);
          if (t.supState === "pills") lines.push(`${t.added} supporting tags added.`);
          if (t.supState === "typed") lines.push(`${t.total} supporting tags typed in, comma separated.`);
          const typed = t.mainState === "typed" || t.supState === "typed";
          b.appendChild(el("div", typed ? "rbmf-warn" : "rbmf-ok", lines.join(" ")));
          if (typed) {
            b.appendChild(
              el(
                "div",
                "rbmf-warn",
                "Not turned into tag pills yet: click at the end of that field and press Enter (or type a comma).",
              ),
            );
          }
        }
      }
      if (missing.length) {
        b.appendChild(el("div", "rbmf-warn", `Field not found on the page: ${missing.join(", ")}`));
      } else {
        b.appendChild(el("div", "rbmf-ok", "Filled. Review, then press Save."));
      }
      const again = el("button", "rbmf-btn", "Fill again");
      again.onclick = () => {
        userEdited.clear();
        tagJob = { ...tagJob, name: null, state: "idle" };
        fillWithRetry();
      };
      b.appendChild(again);
      const diag = el("button", "rbmf-btn", "Show form fields");
      diag.onclick = () => {
        const fields = findFields();
        const role = (f) => Object.keys(fields).find((k) => fields[k] === f) || "-";
        const out = el("textarea", "rbmf-diag");
        out.readOnly = true;
        out.value =
          textFields()
            .map((f) => `[${role(f)}] <${f.tagName.toLowerCase()}> ${describe(f)}`)
            .join("\n") +
          "\n\n" +
          tagBlocksHtml();
        b.appendChild(out);
        out.select();
        diag.remove();
      };
      b.appendChild(diag);
    }
    return { show, refresh };
  })();
})();
