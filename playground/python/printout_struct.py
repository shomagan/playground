import sys
import re
import optparse
import logging
import os
import fileinput

__description__ = '''stdout struct '''
CURRENT_FOLDER = os.path.dirname(os.path.abspath(__file__))

def handle_struct(file_path, prefix):
    started = 0
    file_to_write = open('stdout_' + file_path, 'w', encoding='UTF-8')
    for line in fileinput.input(file_path):
        if re.search(r"struct\s+[a-zA-Z_$][\w$]*\s*\{", line):
            started = 1
            file_to_write.write(line)
        elif started:
            cancel = re.compile(r'^\s*\}\;', re.ASCII)
            if cancel.match(line):
                started = 0
                break;
            element = re.compile(r'^\s*[a-zA-Z_$][\w$]*\s*\w*\s*\w*\s*\w*\s+(?P<NAME>[a-zA-Z_$][\w$]*)\s*\;', re.ASCII)
            search_res = element.match(line)
            if search_res:
                group_dict = search_res.groupdict()
                file_to_write.write(f"std::cout << \"{group_dict['NAME']} \" << {prefix}{group_dict['NAME']} << std::endl;\n")
            else:
                file_to_write.write(line)
    file_to_write.close()


def main():
    parser = optparse.OptionParser(
        usage="python printout_struct.py -f file_with_struct -p \"nic_stat.\"", description=__description__
    )
    group = optparse.OptionGroup(parser, "Port settings")
    group.add_option(
        "-f",
        "--path",
        dest="file_path",
        action="store",
        help="active file to read",
        default=CURRENT_FOLDER + "/struct.c",
    )
    group.add_option(
        "-p",
        "--prefix",
        dest="prefix",
        action="store",
        help="add head to the element",
        default=" ",
    )

    group.add_option(
        "-l",
        "--logging_level",
        dest="loglevel",
        action="store",
        help="DEBUG, INFO, WARNING, ERROR, CRITICAL",
        default="INFO",
    )
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    logging.info(__description__)
    handle_struct(options.file_path, options.prefix)



if __name__  == "__main__":
   main()
