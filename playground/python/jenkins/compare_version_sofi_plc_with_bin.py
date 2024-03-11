import os
import argparse
import last_version
from telegram_api import send_message


__description__ = 'compare version with bin files on build path'


def main():
    parser = argparse.ArgumentParser(description=__description__)
    parser.add_argument('-v', '--version',  default='v0.0.0.0',
                        dest='version',
                        help='version for compare')
    args = parser.parse_args()
    result = "files_not_exist"
    overlap = 0
    if args.version != 'v0.0.0.0':
        if os.path.exists(last_version.sofi_plc_project_path + "/free_rtos/build"):
            version = args.version.replace(".", "_")
            version = version.replace("v", "")
            for (dirpath, dirnames, filenames) in os.walk(last_version.sofi_plc_project_path + "/free_rtos/build"):
                for filename in filenames:
                    if ("os1_crc" in filename) or ("os2_crc" in filename):
                        if version in filename:
                            overlap += 1
    if overlap >= 2:
        send_message("compare_version_sofi_plc_with_bin: bin file exist")
        result = "files_exist"
    else:
        send_message("compare_version_sofi_plc_with_bin: bin file not exist")
    print(result)
    return result


if __name__ == "__main__":
    main()
