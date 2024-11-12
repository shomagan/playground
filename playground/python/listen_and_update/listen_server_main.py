import sys
import os
from os import walk
import json
import optparse
import threading
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter
import atexit
import time
import pathlib
import logging
import requests
import socket
try:
    import msvcrt
    PLATFORM = "win"
except ImportError:
    PLATFORM = "unix"
    import tty
    import termios
    from select import select

__description__ = 'listen and update'

DESCRIPTION_FILE_NAME = "devices_description.json"
SCRIPT_PATH = pathlib.Path(__file__).parent.resolve()
UDP_BROADCAST_PORT = 65500
UDP_BROADCAST_REQUEST_MESSAGE = "UDP_REQUEST"
def udp_list(stop, description_dic):
    '''used as a thread'''
    udp_port_self = 65500
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("", udp_port_self))
    sock.settimeout(0.250)
    print("start listening")
    packet_number = 0
    while 1:
        try:
            data, address = sock.recvfrom(1024)
            packet_number += 1
            #if not own ip address
            if len(data) and data != UDP_BROADCAST_REQUEST_MESSAGE.encode():
                logging.debug(f"time - {time.time()}")
                logging.debug(f"Address: {address}, Packet number: {packet_number}")
                logging.info(f"Data: {data}, Length: {len(data)}")
                data = data.decode(errors="ignore")
                data = data[:data.rfind('}')+1]
                logging.info(f"Data reduced: {data}, Length: {len(data)}")
                data_json = json.loads(data)
                if "device_name" in data_json:
                    for description in description_dic["devices"]:
                        if description["device_name"] == data_json["device_name"]:
                            logging.info(f"device_name - {data_json['device_name']}")
                            update_vars(str(address[0]),description)
                            break
        except socket.timeout:
            sock.sendto(UDP_BROADCAST_REQUEST_MESSAGE.encode(), ('<broadcast>', UDP_BROADCAST_PORT))
            continue  # Timeout occurred, continue the loop
        '''except Exception as e:
            logging.error(f"An error occurred: {e}")
            sock.close()
            logging.info(f"restart socket for the broadcast messages")
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            sock.bind(("", udp_port_self))
            sock.settimeout(0.250)
        '''
        if stop():
            break

def main():
    parser = optparse.OptionParser(
        usage="",
        description=__description__
    )
    group = optparse.OptionGroup(parser, "device")
    group.add_option("-d", "--dev_desc", dest="dev_desc", action="store",
                     help="json file with settings", default=DESCRIPTION_FILE_NAME)
    group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                     help="DEBUG, INFO, WARNING, ERROR, CRITICAL", default='DEBUG')
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    description_dic = {}
    get_description(options.dev_desc, description_dic)
    logging.info(description_dic)
    stop_thread = False
    udp_list_thread = threading.Thread(target = udp_list, args =(lambda : stop_thread, description_dic))
    udp_list_thread.start()
    while 1:
        q = get_ch()
        if len(q):
            logging.debug(q)
            if q == 'q':
                logging.info("exit")
                stop_thread = True
                udp_list_thread.join()
                time.sleep(0.3)
                sys.exit(1)

def get_ch():
    if PLATFORM == "win":
        ch = msvcrt.getch()
        return ch
    elif PLATFORM == "unix":
        fd = sys.stdin.fileno()
        old_setting = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            i, o, e = select([sys.stdin.fileno()], [], [], 5)
            if i:
                ch = sys.stdin.read(1)
            else:
                ch = ""
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_setting)
        return ch
    else:
        return ""


def get_description(description_file_name, description_dic):
    for (dirpath, dirnames, filenames) in walk(SCRIPT_PATH):
        for i in range(len(filenames)):
            if(description_file_name == filenames[i]):
                file_description = open(filenames[i], "r")
                description_dic.update(json.loads(file_description.read()))
                for description in description_dic["devices"]:
                    description["spaces_num"] = len(description["spaces"])
                return
    logging.info(f"did't find file description - {description_file_name}")


def send_json(address, json_data):
    http_session = requests.Session()
    retries = Retry(total=3, backoff_factor=0.5, status_forcelist=[500, 502, 503, 504])
    http_session.mount('http://', HTTPAdapter(max_retries=retries))
    try:    
        logging.debug(json_data)
        response = requests.get(f'http://{address}:80/get_json.cgi',
                                headers={'Json': json_data})
        json_data = json.loads(response.content)
        logging.debug(json_data)
        return json_data
    except Exception as e:
        logging.error(f"An error occurred: {e}")
        return None


def update_vars(address, description):
    for space in description["spaces"]:
        if "function" in space and "var_name" in space and "value" in space:
            if space["function"] == "write":
                result = send_json(address, f'{{"request":"regs","reg_num":1,"get_type":"only_value","name":"{space["var_name"]}"}}')
                logging.debug(f"read result - {result}")
                if "value" in result:
                    if result["value"] != space["value"]:
                        result = send_json(address, f'{{"request":"regs_set","reg_num":1,"name":"{space["var_name"]}","value":{space["value"]}}}')
                        logging.debug(f"write result - {result}")
            elif space["function"] == "read":
                send_json(str(address[0]))



if __name__ == "__main__":
    main()
