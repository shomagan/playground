import sys
import re
import optparse
import logging

__description__ = 'reads, compares two csv files and outputs a list of a different SWSs'

def main():
   parser = optparse.OptionParser(
           usage="",
           description=""
   )
   group = optparse.OptionGroup(parser, "Port settings")
   group.add_option("-a","--f0",dest="csv_file_0",action="store",help="chose you file",default='Book_high.csv')
   group.add_option("-b","--f1",dest="csv_file_1",action="store",help="chose you file",default='_BMW_mPAD_CP_05_SW_Design_02_SWS_BMW_mPAD_CP_SWS_SwcFaultMgr.CSV')
   group.add_option("-l","--logging_level",dest="loglevel",action="store",help="DEBUG, INFO, WARNING, ERROR, CRITICAL",default='INFO')
   parser.add_option_group(group)
   (options, args) = parser.parse_args()
   logging.basicConfig(level=options.loglevel.upper())
   logging.info(__description__)
   sws_id_list0,sws_id_list1 = csv_files_analyze_and_create_list_of_SWS(options.csv_file_0,options.csv_file_1)
   diff_sws_list0,diff_sws_list1 = compare_and_create_diff(sws_id_list0,sws_id_list1)
   write_results_to_files(diff_sws_list0,diff_sws_list1)


def csv_files_analyze_and_create_list_of_SWS(csv_file_name_0,csv_file_name_1):
   csv_file_0 = open(csv_file_name_0,"r")
   csv_lines_0 = csv_file_0.readlines()
   csv_file_1 = open(csv_file_name_1,"r")
   csv_lines_1 = csv_file_1.readlines()
   #Object Identifier,REQ_Revision,Object_Type,Change_Indication,Object_Status,Implementation_Plan_4Cp_CP_G70,Verification_Criteria,REQ_ID (SRS+SwAD-REQ_ID),Safety_Level
   #BMW-mPADCP-SWS-SwcFaultMgr-2068,9,Requirement,Changed,Approved,23-11_R-220,,BMW_mPAD_CP_SRS_SE_Safety_Controller_SFG_Communication_8533,QM
   #BMW-mPADCP-SWS-SwcFaultMgr-2068;9;Requirement;Changed;Approved;23-11_R-220;BMW_mPAD_CP_SRS_SE_Safety_Controller_SFG_Communication_8533;QM
   sws_id_list0 = create_sws_list_with_matched_pattern(csv_lines_0)
   sws_id_list0["name"] = csv_file_name_0;
   logging.info(f"found {len(sws_id_list0)} unic ids in the file {csv_file_name_0}")
   sws_id_list1 = create_sws_list_with_matched_pattern(csv_lines_1)
   sws_id_list1["name"] = csv_file_name_1;
   logging.info(f"found {len(sws_id_list1)} unic ids in the file {csv_file_name_1}")
   csv_file_0.close()
   csv_file_1.close()
   return sws_id_list0,sws_id_list1


def write_results_to_files(diff_sws_list0,diff_sws_list1):
   uniq_csv_file_0 = open("uniq_" + diff_sws_list0["name"],"w")
   uniq_csv_file_1 = open("uniq_" + diff_sws_list1["name"],"w")
   for key in diff_sws_list0.keys():
      uniq_csv_file_0.write(f"{diff_sws_list0[key]}\n")
   for key in diff_sws_list1.keys():
      uniq_csv_file_1.write(f"{diff_sws_list1[key]}\n")
   uniq_csv_file_0.close()
   uniq_csv_file_1.close()


def create_sws_list_with_matched_pattern(lines_of_the_file):
   re_compile_pattern = re.compile(r'^\s*(?P<ID>[\w\-]+)\s*[,;]\s*(?P<REVISION>[\d]+)\s*[,;]\s*(?P<TYPE>[\w\-]+)\s*[,;]\s*(?P<CHANGE_INDICATORE>[\w\-]+)\s*[,;]\s*(?P<OBJECT_STATUS>[\w\-]+)\s*[,;]\s*(?P<IMPLEMENTATION_PLAN>[\w\-\/]+)\s*[,;]+\s*(?P<PARENT_ID>[\w\-]+)\s*[,;]\s*(?P<SAFETY_LEVEL>[\w\-]+)')
   index = 0
   SWSs = {}
   for line in lines_of_the_file:
      search_res = re_compile_pattern.match(line)
      if search_res:
         group_dict = search_res.groupdict()
         sws_number = sws_find_id_number(group_dict["ID"])
         if sws_number not in SWSs.keys():
            SWSs[sws_number] = group_dict["ID"]
            index+=1
         else:
            name_id = group_dict["ID"]
            logging.error(f'sws id -{name_id} already exist - {SWSs[sws_number]}')
      else:
         logging.warning(f'does not match to the SWS object pattern - {line}')
   logging.info(f"parsed - {len(lines_of_the_file)} found - {index} SWSs")
   return SWSs


def sws_find_id_number(full_sws_name):
   #BMW-mPADCP-SWS-SwcFaultMgr-2068
   PATTERN = re.compile(r'(?P<CASTOMER>[\w]+)\-(?P<PROJECT>[\w]+)\-(?P<REQ_TYPE>[\w]+)\-(?P<MODULE>[\w]+)\-(?P<NUMBER>[\d]+)')
   search_res = PATTERN.match(full_sws_name)
   id_number = -1
   if search_res:
      group_dict = search_res.groupdict()
      id_number = int(group_dict["NUMBER"])
   else:
      logging.warning(f'does not match to the SWS name pattern - {line}')
   return id_number


def compare_and_create_diff(sws_id_list0,sws_id_list1):
   diff_sws_list0 = {}
   diff_sws_list0["name"] = sws_id_list0["name"]
   diff_sws_list1 = {}
   diff_sws_list1["name"] = sws_id_list1["name"]
   for sws_id_number in sws_id_list0.keys():
      if sws_id_number not in sws_id_list1.keys():
         logging.debug("found new SWS {sws_id_list0[sws_id_number]}")
         diff_sws_list0[sws_id_number] = sws_id_list0[sws_id_number]
   for sws_id_number in sws_id_list1.keys():
      if sws_id_number not in sws_id_list0.keys():
         logging.debug("found new SWS {sws_id_list1[sws_id_number]}")
         diff_sws_list1[sws_id_number] = sws_id_list1[sws_id_number]
   return diff_sws_list0,diff_sws_list1


if __name__  == "__main__":
   main()
