#!/usr/bin/env python3
"""
python tag_stock_photos_gemini.py "C:\_camlea\photo\stocks\out_8_07_26" --csv report.csv

Analyzes JPG photos with Google's Gemini vision API (free tier) and writes
stock-photo-ready metadata (Title, Description, Keywords) directly into
each file's IPTC/XMP fields using ExifTool.

SETUP (one-time)
-----------------

1. Install ExifTool:
   - Windows: download the "Windows Executable" from https://exiftool.org
     Rename the downloaded "exiftool(-k).exe" to "exiftool.exe" and either:
       a) put it in this script's folder, OR
       b) put it somewhere on your PATH
   - Mac: brew install exiftool
   - Linux: sudo apt install libimage-exiftool-perl

2. Install the Python package:
       pip install google-genai

3. Get a FREE Gemini API key (no credit card required):
       https://aistudio.google.com/apikey

USAGE
-----
    python tag_stock_photos_gemini.py "C:\\_camlea\\photo\\stocks\\out_8_07_26"

Optional flags:
    --dry-run          Preview results without writing metadata to files
    --csv report.csv   Also save a CSV summary of all titles/keywords
    --overwrite        Re-tag files even if they already have a Title set
    --model MODEL      Gemini model to use (default: gemini-2.5-flash)

WHAT IT WRITES
--------------
    IPTC:ObjectName        <- Title
    XMP:Title               <- Title
    IPTC:Caption-Abstract   <- Description
    XMP:Description         <- Description
    IPTC:Keywords           <- Keywords (multi-value)
    XMP:Subject              <- Keywords (multi-value)

These are the fields Shutterstock, Adobe Stock, and most agencies read
during upload / bulk metadata import.
"""

import argparse
import csv
import json
import os
import shutil
import subprocess
import sys
import time
import credentials
from pathlib import Path

try:
    from google import genai
    from google.genai import types
except ImportError:
    print("Missing dependency. Run:  pip install google-genai")
    sys.exit(1)
#DEFAULT_MODEL = "gemini-flash-latest"
DEFAULT_MODEL = "gemini-flash-lite-latest"

PROMPT = """You are a professional stock photo keyworder. Analyze this image and \
respond with ONLY a JSON object (no markdown fences, no preamble) in exactly \
this format:

{
  "title": "Short, natural, sellable title (6-12 words, no camera jargon)",
  "description": "One sentence description of the scene (max 25 words)",
  "keywords": ["keyword1", "keyword2", "... 30-45 single or short multi-word keywords, ordered by relevance, most specific and important first, include: main subject, setting/location, colors, mood/concept, season if relevant, composition style, potential use-case terms (e.g. 'copy space', 'background')"]
}

Write for a stock photography buyer searching by keyword. Be accurate and \
specific - do not invent details not visible in the image. Do not include \
brand names, recognizable logos, or identifiable people's names."""


def check_exiftool():
    exe = shutil.which("exiftool") or shutil.which("exiftool.exe")
    if exe:
        return exe
    local = Path(__file__).parent / "exiftool.exe"
    if local.exists():
        return str(local)
    print("ERROR: exiftool not found. See setup instructions at the top of this script.")
    sys.exit(1)


def already_tagged(exiftool_path: str, image_path: Path) -> bool:
    result = subprocess.run(
        [exiftool_path, "-Title", "-s3", str(image_path)],
        capture_output=True, text=True
    )
    return bool(result.stdout.strip())


class DailyQuotaExhausted(Exception):
    """Raised when the free-tier daily quota is hit (not just a transient rate limit)."""
    pass


def analyze_image(client: "genai.Client", model: str, image_path: Path, max_retries: int = 5) -> dict:
    with open(image_path, "rb") as f:
        image_bytes = f.read()

    delay = 5
    last_err = None
    for attempt in range(1, max_retries + 1):
        try:
            response = client.models.generate_content(
                model=model,
                contents=[
                    types.Part.from_bytes(data=image_bytes, mime_type="image/jpeg"),
                    PROMPT,
                ],
                config=types.GenerateContentConfig(
                    response_mime_type="application/json",
                ),
            )
            text = response.text.strip()
            text = text.removeprefix("```json").removeprefix("```").removesuffix("```").strip()
            return json.loads(text)

        except Exception as e:
            msg = str(e)
            last_err = e
            # RESOURCE_EXHAUSTED / 429 = rate limit or daily quota hit
            if "RESOURCE_EXHAUSTED" in msg or "429" in msg or "quota" in msg.lower():
                # Try to detect whether it's a per-day quota (no point retrying today)
                if "PerDay" in msg or "per day" in msg.lower():
                    raise DailyQuotaExhausted(msg)
                print(f"    Rate limited, waiting {delay}s before retry ({attempt}/{max_retries})...")
                time.sleep(delay)
                delay = min(delay * 2, 60)
                continue
            raise
    raise last_err


def write_metadata(exiftool_path: str, image_path: Path, meta: dict, dry_run: bool):
    title = meta["title"]
    description = meta["description"]
    keywords = meta["keywords"]

    if dry_run:
        print(f"\n  [DRY RUN] Would write to {image_path.name}:")
        print(f"    Title: {title}")
        print(f"    Description: {description}")
        print(f"    Keywords ({len(keywords)}): {', '.join(keywords)}")
        return

    cmd = [
        exiftool_path,
        "-overwrite_original",
        "-codedcharacterset=utf8",
        f"-IPTC:ObjectName={title}",
        f"-XMP:Title={title}",
        f"-IPTC:Caption-Abstract={description}",
        f"-XMP:Description={description}",
        "-IPTC:Keywords=",   # clear existing first
        "-XMP:Subject=",
    ]
    for kw in keywords:
        cmd.append(f"-IPTC:Keywords+={kw}")
        cmd.append(f"-XMP:Subject+={kw}")
    cmd.append(str(image_path))

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  ERROR writing metadata: {result.stderr.strip()}")
    else:
        print(f"  Metadata written ({len(keywords)} keywords)")


def main():
    parser = argparse.ArgumentParser(description="AI-tag stock photos with Gemini vision (free) + ExifTool")
    parser.add_argument("folder", help="Folder containing JPG files")
    parser.add_argument("--dry-run", action="store_true", help="Preview without writing to files")
    parser.add_argument("--csv", metavar="FILE", help="Also write a CSV summary")
    parser.add_argument("--overwrite", action="store_true", help="Re-tag files that already have a Title")
    parser.add_argument("--model", default=DEFAULT_MODEL, help="Gemini model to use")
    args = parser.parse_args()

    folder = Path(args.folder)
    if not folder.is_dir():
        print(f"ERROR: folder not found: {folder}")
        sys.exit(1)

    api_key = credentials.GEMINI_API
    if not api_key:
        print("ERROR: GEMINI_API_KEY environment variable not set. See setup instructions.")
        sys.exit(1)

    exiftool_path = check_exiftool()
    client = genai.Client(api_key=api_key)

    jpgs = sorted([p for p in folder.iterdir() if p.suffix.lower() in (".jpg", ".jpeg")])
    if not jpgs:
        print(f"No JPG files found in {folder}")
        sys.exit(0)

    print(f"Found {len(jpgs)} JPG file(s) in {folder}\n")

    csv_rows = []

    for i, path in enumerate(jpgs, 1):
        print(f"[{i}/{len(jpgs)}] {path.name}")

        if not args.overwrite and not args.dry_run and already_tagged(exiftool_path, path):
            print("  Skipping (already has a Title). Use --overwrite to re-tag.")
            continue

        try:
            meta = analyze_image(client, args.model, path)
        except DailyQuotaExhausted:
            print("\n  Daily free-tier quota reached. Stopping here.")
            print(f"  Progress so far saved. Re-run the same command tomorrow to")
            print(f"  continue already-tagged files are skipped automatically")
            print(f"  (use --overwrite only if you want to redo them).")
            break
        except Exception as e:
            print(f"  ERROR analyzing image: {e}")
            continue

        write_metadata(exiftool_path, path, meta, args.dry_run)

        csv_rows.append({
            "filename": path.name,
            "title": meta["title"],
            "description": meta["description"],
            "keywords": ", ".join(meta["keywords"]),
        })

        # Small delay to stay comfortably within free-tier rate limits
        time.sleep(1)

    if args.csv and csv_rows:
        with open(args.csv, "w", newline="", encoding="utf-8") as f:
            writer = csv.DictWriter(f, fieldnames=["filename", "title", "description", "keywords"])
            writer.writeheader()
            writer.writerows(csv_rows)
        print(f"\nCSV summary saved to {args.csv}")

    print("\nDone.")


if __name__ == "__main__":
    main()