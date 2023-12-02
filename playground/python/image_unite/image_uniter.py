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
    group = optparse.OptionGroup(parser, "Port settings")
    group.add_option("-d", "--dir", dest="directory", action="store",
                     help="path to images", default='Images')
    group.add_option("-w", "--width", dest="width", action="store", type="int",
                     help="collag width", default=4000)
    group.add_option("--he", "--height", dest="height", action="store", type="int",
                     help="collag height", default=6000)
    group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                     help="DEBUG, INFO, WARNING, ERROR, CRITICAL", default='INFO')
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    logging.info(__description__)
    max_row = 3
    max_line = 3
    position_row = 0
    position_line = 0
    file_number = 1
    brim = 10
    for (dirpath, dirnames, filenames) in walk(options.directory):
        for file_name in filenames:
            try:
                image_data = Image.open(options.directory + "/" + file_name)
                w, h = image_data.size
                if w > h:
                    logging.info(f"{options.directory}/{file_name} - horizontal {w} {h}")
                else:
                    logging.info(f"{options.directory}/{file_name} - vertical {w} {h}")
                    image_data = image_data.rotate(90, expand=True)
                    w, h = image_data.size
                    logging.debug(f"{options.directory}/{file_name} after rotating {w} {h}")

                coefficient = calculate_coefficient(w, h, (options.width - (brim * (max_row - 1))) / max_row,
                                              (options.height-(brim*(max_row-1))) / max_line)
                if position_row == 0 and position_line == 0:
                    collage_image = Image.new('RGB', (options.width, options.height))
                image_data = image_data.resize((int(w * coefficient), int(h * coefficient)), Image.LANCZOS)
                if position_row:
                    brim_r = brim
                else:
                    brim_r = 0
                if position_line:
                    brim_l = brim
                else:
                    brim_l = 0
                collage_image.paste(image_data, (int(position_row*(options.width/max_row) + brim_r), 
                                                int(position_line*(options.height/max_line) + brim_l)))

                if position_row == max_row - 1 and position_line == max_line - 1:
                    file_name = find_next_free_name()
                    collage_image.save(file_name, quality=95)
                    collage_image.close()
                    position_row = 0
                    position_line = 0
                    file_number += 1
                elif position_row >= max_row -1:
                    position_row = 0
                    if position_line >= max_line -1:
                        position_line = 0
                    else:
                        position_line += 1
                else:
                    position_row += 1
                image_data.close()
            except PIL.UnidentifiedImageError:
                logging.debug(options.directory + "/" + file_name + "is not image")
    if position_row != 0 and position_line != 0:
        '''save last if what ever we have'''
        file_name = find_next_free_name()
        collage_image.save(file_name, quality=95)
        collage_image.close()


def calculate_coefficient(im_width, im_height, width, height):
   if width/im_width < height/im_height:
      coeff = width/im_width
   else:
      coeff = height/im_height
   return coeff


def find_next_free_name():
    digit = 0
    while 1:
        template = f"collage_{digit}.jpg"
        if not os.path.isfile(template):
            return template
        digit += 1


if __name__ == "__main__":
    main()
