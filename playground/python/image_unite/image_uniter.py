import sys
import re
import optparse
import logging
from PIL import Image
import PIL
from os import walk

__description__ = 'unites few picture in one(collage)'


def main():
    parser = optparse.OptionParser(
        usage="",
        description=""
    )
    group = optparse.OptionGroup(parser, "Port settings")
    group.add_option("-d", "--dir", dest="directory", action="store",
                     help="path to images", default='Images')
    group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                     help="DEBUG, INFO, WARNING, ERROR, CRITICAL", default='INFO')
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    logging.info(__description__)
    for (dirpath, dirnames, filenames) in walk(options.directory):
        for file_name in filenames:
            try:
                image_data = Image.open(options.directory + "/" + file_name)
                w, h = image_data.size
                if w > h:
                    logging.info(options.directory + "/" + file_name + " horizontal")
                else:
                    logging.info(options.directory + "/" + file_name + " vertical")
            except PIL.UnidentifiedImageError:
                logging.debug(options.directory + "/" + file_name + "is not image")


if __name__ == "__main__":
    main()
