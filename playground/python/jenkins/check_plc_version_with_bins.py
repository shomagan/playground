import last_version
import requests
import json
from os import walk
import re
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter
from telegram_api import send_message


def main():
    result = "equal"
    s = requests.Session()
    retries = Retry(total=3, backoff_factor=0.5, status_forcelist=[500, 502, 503, 504])
    s.mount('http://', HTTPAdapter(max_retries=retries))

    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi', headers={'Json': '{"request":"os_param"}'})
    json_data = json.loads(response.content)
    if 'current' not in json_data or 'main' not in json_data or 'os_ver' not in json_data:
        print("error")
        return -1
    if json_data['current'] != json_data['main']:
        print("error")
        return -1
    os_version = json_data['os_ver']
    os_position = json_data['current']  # 1 or 2
    os_position_for_loading = 2 if os_position == 1 else 1  # inversion
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi',
                            headers={'Json': '{"request":"regs","reg_num":1,"get_type":"only_value","name":"board_ver"}'})
    json_data = json.loads(response.content)
    board_ver = int(json_data['value']) & 0x0f
    path_bins = last_version.sofi_plc_project_path + '/free_rtos/build'
    name_model_os = re.compile("sofi_(?P<V0>[0-9]+)_(?P<V1>[0-9]+)_(?P<V2>[0-9]+)_(?P<V3>[0-9]+)_rel_bv_"
                               + str(board_ver)+"_os"+str(os_position_for_loading)+"_crc\.bin")

    for (dirpath, dirnames, filenames) in walk(path_bins):
        for filename in filenames:
            match_result = name_model_os.match(filename)
            if match_result:
                res_dict = match_result.groupdict()
                if os_version[0] < int(res_dict['V0']):
                    result = "outdated"
                    break
                elif os_version[0] == int(res_dict['V0']):
                    if os_version[1] < int(res_dict['V1']):
                        result = "outdated"
                        break
                    elif os_version[1] == int(res_dict['V1']):
                        if os_version[2] < int(res_dict['V2']):
                            result = "outdated"
                            break
                        elif os_version[2] == int(res_dict['V2']):
                            if os_version[3] < int(res_dict['V3']):
                                result = "outdated"
                                break
        if result == "outdated":
            break
    if result == "outdated":
        send_message("check_plc_version_with_bins: plc_ver_outdated - {}".format(os_version))
    else:
        send_message("check_plc_version_with_bins: plc ver equal - {}".format(os_version))
    print(result)


if __name__ == "__main__":
    main()
