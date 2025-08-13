import sys
import re
import optparse
import logging
from PIL import Image
import PIL
from os import walk
import os

__description__ = 'unites few picture in one(collage)'


def main():
   parser = optparse.OptionParser(
      usage="",
      description=""
   )
   group = optparse.OptionGroup(parser, "recursivle remove files")
   group.add_option("-e", "--extension", dest="extension", action="store", type="string",
                  help="file extension to remove", default='.arw')
   group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                  help="DEBUG, INFO, WARNING, ERROR, CRITICAL", default='INFO')
   '''reqursivle remove all files in directory with ectension '''
   parser.add_option_group(group)
   (options, args) = parser.parse_args()
   logging.basicConfig(level=options.loglevel.upper())
   logging.info("start removing files with extension %s", options.extension)
   for (dirpath, dirnames, filenames) in walk('.'):
      for file_name in filenames:
         if file_name.endswith(options.extension):
            logging.info(f"removing {dirpath}/{file_name}")
            os.remove(f"{dirpath}/{file_name}")


if __name__ == "__main__":
   main()
