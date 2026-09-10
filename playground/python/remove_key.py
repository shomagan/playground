#!/usr/bin/env python3

import argparse
import shutil
import subprocess
import sys
from pathlib import Path
'''exiftool.exe -G1 -a -s -Keywords -Subject -XPKeywords '''
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


def change_metadata(exiftool_path: str, image_path: Path):
    key_to_remove = {"photography", "stock photo", "stock photography", "horizontal", "photo"}
    cmd = [
        exiftool_path,
        "-overwrite_original",
    ]
    for key in key_to_remove:
        cmd.append(f"-IPTC:Keywords-={key}")
        cmd.append(f"-XMP:Subject-={key}")

    cmd.append(str(image_path))

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  ERROR writing metadata: {result.stderr.strip()}")
    else:
        print(f"  keywords  removed ")


def main():
    parser = argparse.ArgumentParser(description="AI-tag stock photos with Gemini vision (free) + ExifTool")
    parser.add_argument("folder", help="Folder containing JPG files")
    args = parser.parse_args()

    folder = Path(args.folder)
    if not folder.is_dir():
        print(f"ERROR: folder not found: {folder}")
        sys.exit(1)

    exiftool_path = check_exiftool()

    jpgs = sorted([p for p in folder.iterdir() if p.suffix.lower() in (".jpg", ".jpeg")])
    if not jpgs:
        print(f"No JPG files found in {folder}")
        sys.exit(0)

    print(f"Found {len(jpgs)} JPG file(s) in {folder}\n")

    for i, path in enumerate(jpgs, 1):
        print(f"[{i}/{len(jpgs)}] {path.name}")

        if already_tagged(exiftool_path, path):
            change_metadata(exiftool_path, path)


    print("\nDone.")


if __name__ == "__main__":
    main()