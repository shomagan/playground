import re
import optparse
import os
import logging

__description__ = ''' delete all comments and empty lines from c and h '''

def main():
    parser = optparse.OptionParser(
        usage="", description=__description__
    )
    group = optparse.OptionGroup(parser, "Port settings")

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

    walk_through_files(['Appl'])

def walk_through_files(paths_to_walk, file_extension=['.c','.h']):
    for init_path in paths_to_walk:
        for (dirpath, dirnames, filenames) in os.walk(init_path):
            for filename in filenames:
                if filename.endswith(file_extension[0]) or filename.endswith(file_extension[1]):
                    logging.info("cleaning file - "+ dirpath +'\\'+ filename)
                    clean_up_the_file(dirpath + '\\' + filename)

def clean_up_the_file(file_path):
    clean_code = ""
    with open(file_path, "r", encoding="utf-8", errors='ignore') as file_to_modify:
        clean_code = remove_comment(file_to_modify.read())

    with open(file_path, "w", encoding="utf-8") as file_to_modify:
        file_to_modify.write(clean_code)

    with open(file_path, "r", encoding="utf-8", errors='ignore') as file_to_modify:
        clean_code = remove_empty_lines(file_to_modify)

    with open(file_path, "w", encoding="utf-8") as file_to_modify:
        file_to_modify.write(clean_code)
    


def remove_comment(text):
    pattern = re.compile(
        r'(".*?"|\'.*?\')|(//.*?$|/\*.*?\*/)',
        re.DOTALL | re.MULTILINE)
    return re.sub(pattern, lambda m: m.group(1) if m.group(1) else "", text)

def remove_empty_lines(text):
    lines = text.readlines()
    text = ""
    switch_to_next_line = 0
    pattern = re.compile(r'^\s*$',re.DOTALL)
    for line in lines:
       if switch_to_next_line == 1 or not pattern.match(line):
          text += line
       if line.rstrip().endswith('\\'):
          switch_to_next_line = 1
       else:
          switch_to_next_line = 0
    return text

def compress_if_else_statement(text):
    pattern_if = re.compile(r'^(\s*)if(.*){',re.DOTALL | re.MULTILINE)
    pattern_else = re.compile(r'\}\s*$\s*else\s*$\s*\{',re.MULTILINE)
    first_step = re.sub(pattern_if, "\g<1>if\g<2>{", text)
    second_step = re.sub(pattern_else, "}else{", first_step)
    return second_step


if __name__ == '__main__':
    ''' delete all comments and empty lines from c and h '''
    main()