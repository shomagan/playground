from os import walk 
import re
import sys
'''
const fb_cals_t fb_cals[]={
    {sizeof(fb1_above_set_point_input_t),sizeof(fb1_above_set_point_local_t),sizeof(fb1_above_set_point_output_t),fb1_above_set_init, fb1_above_set_point},

'''
file_make = open("fb_references.templ", 'w', encoding="utf-8")
file_make.write("const fb_cals_t fb_cals[FB_NUMBER]={\n")

NAME_FILE_TYPE = re.compile(r"fb(?P<NUMBER>\d+)_(?P<NAME>[\w\d\W]+)\.c")
for (dirpath, dirnames, filenames) in walk("fb/"):
    for i in range(len(filenames)):
        res = NAME_FILE_TYPE.match(filenames[i])
        if res:
            res_dict = res.groupdict()
            fb_number = res_dict["NUMBER"]
            fb_name = res_dict["NAME"]
            name_full = "fb"+fb_number+"_"+fb_name
            file_make.write("   {{{},\"{}\",".format(fb_number,fb_name))
            file_make.write("sizeof({}_input_t),sizeof({}_input_flags)/sizeof(u32),{}_input_flags,\n".format(name_full,name_full,name_full))
            file_make.write("   sizeof({}_local_t),sizeof({}_local_flags)/sizeof(u32),{}_local_flags,\n".format(name_full,name_full,name_full))
            file_make.write("   sizeof({}_output_t),sizeof({}_output_flags)/sizeof(u32),{}_output_flags,\n".format(name_full,name_full,name_full))
            file_make.write("   {}_init, {}}},\n".format(name_full,name_full))
file_make.write("};\n")
file_make.close
