import last_version
import requests
import json
import time
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter
from telegram_api import send_message

CRC_BLOCK = 1 << 0
RTC_BLOCK = 1 << 1
DI_BLOCK = 1 << 2
DO_BLOCK = 1 << 3
AI_BLOCK = 1 << 4
INTERNAL_FLASH_BLOCK = 1 << 5
EXTERNAL_FLASH_BLOCK = 1 << 6
FREERTOS_BLOCK = 1 << 7
MATH_BLOCK = 1 << 8
UART_BLOCK = 1 << 9
MESO_BLOCK = 1 << 10
RANDOM_BLOCK = 1 << 11
CAN_BLOCK = 1 << 12
PACKET_BLOCK = 1 << 13
REPEATER_BLOCK = 1 << 14
LFS_BLOCK = 1 << 15
ETHERNET_BLOCK = 1 << 16
CHECK_TEST = 1 << 29
STRESS_TEST = 1 << 30
PERFORMANCE_TEST = 1 << 31


def main():
    result = "failure"
    s = requests.Session()
    retries = Retry(total=3, backoff_factor=0.5, status_forcelist=[500, 502, 503, 504])
    s.mount('http://', HTTPAdapter(max_retries=retries))

    test_flags = CHECK_TEST | CRC_BLOCK | RTC_BLOCK | DI_BLOCK | DO_BLOCK | AI_BLOCK | INTERNAL_FLASH_BLOCK |\
                 EXTERNAL_FLASH_BLOCK | FREERTOS_BLOCK | MATH_BLOCK | UART_BLOCK | MESO_BLOCK |\
                 RANDOM_BLOCK | CAN_BLOCK | PACKET_BLOCK | REPEATER_BLOCK | LFS_BLOCK | ETHERNET_BLOCK
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi',
                            headers={'Json': '{{"request":"regs_set","name":"sofi_test_blocks","value": {}}}'.format(test_flags)})
    time.sleep(0.1)
    for i in range(30):
        response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi',
                            headers={'Json': '{"request":"regs","reg_num":1,"get_type":"only_value","name":"run_test"}'})
        time.sleep(2)
        json_data = json.loads(response.content)
        if 'value' in json_data:
            if json_data['value'] == 0:
                break
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi',
                            headers={'Json': '{"request":"regs","reg_num":1,"get_type":"only_value","name":"sofi_test_result"}'})
    json_data = json.loads(response.content)
    if 'value' in json_data:
        test_flags_res = json_data['value'] | CHECK_TEST
        if test_flags_res == test_flags:
            send_message("tests passed successful - {}".format(hex(test_flags_res)))
            result = "success"
        else:
            send_message("tests passed failure , flags - {},result -{}".format(hex(test_flags), hex(test_flags_res)))
            result += "_tests- {},result - {} ".format(hex(test_flags_res), hex(test_flags))
    else:
        send_message("plc_testing: request content mistake")

    print(result)


if __name__ == "__main__":
    main()
