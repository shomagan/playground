import sys
import os
from os import walk
import json
import optparse
import threading
from queue import Queue 
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter
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
LOG_DIR = "data_log"


def main():
    parser = optparse.OptionParser(
        usage="",
        description=__description__
    )
    group = optparse.OptionGroup(parser, "device")
    group.add_option("-d", "--dev_desc", dest="dev_desc", action="store",
                     help="json file with settings", default=DESCRIPTION_FILE_NAME)
    group.add_option("-l", "--logging_level", dest="loglevel", action="store",
                     help="DEBUG, INFO, WARNING, ERROR, CRITICAL", default='INFO')
    parser.add_option_group(group)
    (options, args) = parser.parse_args()
    logging.basicConfig(level=options.loglevel.upper())
    description_dic = {}
    get_description(options.dev_desc, description_dic)
    logging.info(description_dic)
    if LOG_DIR not in os.listdir():
        os.mkdir(LOG_DIR)
    description_queue = Queue()
    udp_list_thread = threading.Thread(target = udp_list, args =(description_queue,))
    description_queue.put(description_dic)
    udp_list_thread.start()
    last_time = time.time()
    while 1:
        q = get_ch()
        if len(q):
            logging.debug(q)
            if q == 'q':
                logging.info("exit")
                description_dic["running"] = 0 
                description_queue.put(description_dic)
                time.sleep(0.3)
                sys.exit(1)
        #only every 5 seconds
        current_time = time.time()
        if abs(current_time - last_time) >= 5:
            description_dic_new = {}
            get_description(options.dev_desc, description_dic_new)
            if(description_dic_new != description_dic):
                description_dic = description_dic_new
                description_queue.put(description_dic)
            last_time = current_time
        


def udp_list(description_queue):
    '''used as a thread'''
    udp_port_self = 65500
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("", udp_port_self))
    sock.settimeout(0.250)
    print("start listening")
    packet_number = 0
    description_dic = description_queue.get()
    while 1:
        if not description_queue.empty():
            description_dic = description_queue.get()
        try:
            data, address = sock.recvfrom(1024)
            packet_number += 1
            #if not own ip address
            if len(data) and data != UDP_BROADCAST_REQUEST_MESSAGE.encode():
                logging.debug(f"time - {time.time()}")
                logging.debug(f"Address: {address}, Packet number: {packet_number}")
                logging.debug(f"Data: {data}, Length: {len(data)}")
                data = data.decode(errors="ignore")
                data = data[:data.rfind('}')+1]
                logging.debug(f"Data reduced: {data}, Length: {len(data)}")
                data_json = json.loads(data)
                if "device_name" in data_json:
                    for description in description_dic["devices"]:
                        if description["device_name"] == data_json["device_name"]:
                            logging.debug(f"device_name - {data_json['device_name']}")
                            update_vars(str(address[0]),description)
        except socket.timeout:
            sock.sendto(UDP_BROADCAST_REQUEST_MESSAGE.encode(), ('<broadcast>', UDP_BROADCAST_PORT))
            continue  # Timeout occurred, continue the loop
        except Exception as e:
            logging.error(f"An error occurred: {e}")
            sock.close()
            logging.info(f"restart socket for the broadcast messages")
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            sock.bind(("", udp_port_self))
            sock.settimeout(0.250)
        
        if description_dic["running"] == 0:
            break


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
                description_dic["running"] = 1
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
                file_name = f'{description["device_name"]}_{description["mdb_address"]}_{space["var_name"]}.csv'
                update = 0
                if file_name in os.listdir(LOG_DIR):
                    log_file = open(f'{LOG_DIR}/{file_name}', "r")
                    lines = log_file.readlines()
                    log_file.close()
                    if len(lines):
                        last_line = lines[-1]
                        last_line = last_line.split(';')
                        last_time = float(last_line[0])
                        logging.debug(f"last_time - {last_time}")
                        if abs(time.time() - last_time) > space["interval_sec"]:
                            update = 1
                else:
                    update = 1
                if update:
                    result = send_json(address, f'{{"request":"regs","reg_num":1,"get_type":"only_value","name":"{space["var_name"]}"}}')
                    logging.debug(f"read result - {result}")
                    if "value" in result:
                        log_file = open(f'{LOG_DIR}/{file_name}', "a")
                        log_file.write(f"{time.time()}; {result['value']}\n")
                        log_file.close()



if __name__ == "__main__":
    main()
