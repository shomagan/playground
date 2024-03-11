import last_version


def main():
    fs_save_file = open(last_version.sofi_plc_project_path+'/free_rtos/inc/regs.h', 'a')
    fs_save_file.write(' ')
    fs_save_file.close()


if __name__ == "__main__":
    main()
