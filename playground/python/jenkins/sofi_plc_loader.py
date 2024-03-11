import last_version
from os import walk
import requests
import json
import re
import time
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter
from telegram_api import send_message


# Define your own exception
class Error(Exception):
    pass


def main():
    s = requests.Session()
    retries = Retry(total=3, backoff_factor=0.5, status_forcelist=[500, 502, 503, 504])
    s.mount('http://', HTTPAdapter(max_retries=retries))
    response = requests.get('http://{}:80/get_json.cgi'.format(last_version.plc_ip_address), headers={'Json': '{"request":"regs_set","reg_num":1,"name":"user_task_config","value":4'})
    json_data = json.loads(response.content);
    if json_data['status'] != 'ok':
        send_message("sofi_plc_loader: task stop error")
        raise Error('task stop content mistake')
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi', headers={'Json': '{"request":"os_param"}'})
    json_data = json.loads(response.content)
    if 'current' not in json_data or 'main' not in json_data or 'os_ver' not in json_data:
        send_message("sofi_plc_loader: os param request content mistake")
        raise Error('os param request content mistake')
    if json_data['current'] != json_data['main']:
        send_message("sofi_plc_loader: current os not main")
        raise Error('current os not main')
    os_version = json_data['os_ver']
    last_os = list(os_version)
    os_position = json_data['current']  # 1 or 2
    os_position_for_loading = 2 if os_position == 1 else 1  # inversion
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi',
                            headers={'Json': '{"request":"regs","reg_num":1,"get_type":"only_value","name":"board_ver"}'})
    json_data = json.loads(response.content)
    board_ver = int(json_data['value']) & 0x0f
    path_bins = last_version.sofi_plc_project_path + '/free_rtos/build'
    name_model_os = re.compile("sofi_(?P<V0>[0-9]+)_(?P<V1>[0-9]+)_(?P<V2>[0-9]+)_(?P<V3>[0-9]+)_rel_bv_"
                               + str(board_ver)+"_os"+str(os_position_for_loading)+"_crc\.bin")
    bin_file_name = ""
    for (dirpath, dirnames, filenames) in walk(path_bins):
        for i in range(len(filenames)):
            match_result = name_model_os.match(filenames[i])
            if match_result:
                file_template = match_result.groupdict()
                if last_os[0] < int(file_template['V0']):
                    last_os[0] = int(file_template['V0'])
                    last_os[1] = int(file_template['V1'])
                    last_os[2] = int(file_template['V2'])
                    last_os[3] = int(file_template['V3'])
                    bin_file_name = filenames[i]
                    continue
                elif last_os[0] == int(file_template['V0']):
                    if last_os[1] < int(file_template['V1']):
                        last_os[0] = int(file_template['V0'])
                        last_os[1] = int(file_template['V1'])
                        last_os[2] = int(file_template['V2'])
                        last_os[3] = int(file_template['V3'])
                        bin_file_name = filenames[i]
                        continue
                    elif last_os[1] == int(file_template['V1']):
                        if last_os[2] < int(file_template['V2']):
                            last_os[0] = int(file_template['V0'])
                            last_os[1] = int(file_template['V1'])
                            last_os[2] = int(file_template['V2'])
                            last_os[3] = int(file_template['V3'])
                            bin_file_name = filenames[i]
                            continue
                        elif last_os[2] == int(file_template['V2']):
                            if last_os[3] < int(file_template['V3']):
                                last_os[0] = int(file_template['V0'])
                                last_os[1] = int(file_template['V1'])
                                last_os[2] = int(file_template['V2'])
                                last_os[3] = int(file_template['V3'])
                                bin_file_name = filenames[i]
                                continue
    if len(bin_file_name):
        path_bin = last_version.sofi_plc_project_path + '/free_rtos/build/' + bin_file_name
        files = {'upload_file': open(path_bin, 'rb')}
        if os_position_for_loading == 1:
            send_message("sofi_plc_loader: loading os1")
            requests.post('http://' + last_version.plc_ip_address + '/download_os1.key', files=files)
            time.sleep(1.0)
            response = requests.get('http://{}:80/get_json.cgi'.format(last_version.plc_ip_address), headers={'Json': '{"request":"regs_set","reg_num":1,"name":"command","value":21767'})
        elif os_position_for_loading == 2:
            send_message("sofi_plc_loader: loading os2")
            requests.post('http://' + last_version.plc_ip_address + '/download_os2.key', files=files)
            time.sleep(1.0)
            response = requests.get('http://{}:80/get_json.cgi'.format(last_version.plc_ip_address), headers={'Json': '{"request":"regs_set","reg_num":1,"name":"command","value":21768'})
        time.sleep(5.0)
        response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi', headers={'Json': '{"request":"os_param"}'})
        json_data = json.loads(response.content)
        if 'current' not in json_data or 'main' not in json_data or 'os_ver' not in json_data:
            raise Error('os param request content mistake')
        else:
            if json_data['os_ver'][0] == last_os[0] and\
                 json_data['os_ver'][1] == last_os[1] and\
                 json_data['os_ver'][2] == last_os[2] and\
                 json_data['os_ver'][3] == last_os[3]:
                send_message("sofi_plc_loader: success loading last os - {}_{}_{}_{}".format(last_os[0],
                                                                                             last_os[1],
                                                                                             last_os[2],
                                                                                             last_os[3]))
                print('success')
            else:
                send_message("sofi_plc_loader: version mismatch did not load, plc_ver - {}_{}_{}_{}"
                             "bin_ver - {}_{}_{}_{} ".format(json_data['os_ver'][0], json_data['os_ver'][1],
                                                             json_data['os_ver'][2], json_data['os_ver'][3],
                                                             last_os[0], last_os[1], last_os[2], last_os[3]))

                raise Error('os version after loading is mismatch')


if __name__ == "__main__":
    main()
