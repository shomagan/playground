import credentials
import sensecap_decode
import sys
import re
import optparse
import logging
__description__ = "simply adata handling from a helium io console"
import os
import requests
import folium
from datetime import datetime, timedelta
try:
    import msvcrt
    PLATFORM = "win"
except ImportError:
    PLATFORM = "unix"
    import tty
    import termios
    from select import select

# Configuration.

# This must point to the API interface.
server = credentials.API_ENDPOINT

# The DevEUI for which you want to enqueue the downlink.
dev_eui = credentials.DEV_EUI_3

# The API token (retrieved using the web-interface).
api_token = credentials.API_HELIUM_IOT_XYZ

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

class SenseCapData:
    BATTERY_LEVEL_PERCENT = 'battery_level_percent'
    LATITUDE = 'latitude'
    LONGITUDE = 'longitude'
 
    def __init__(self):
        battery_level_percent = {'timestamps'}

def creat_map(latitudes,longitudes,timestamps):
    map = folium.Map(location=[latitudes[0], longitudes[0]], zoom_start=13)
    for i in range(len(latitudes)):
        folium.Marker([latitudes[i], longitude[i]], popup=timestamps[i],tooltip=timestamps[i]).add_to(map)
    map.save("locations.html")
    s = get_ch()

if __name__ == "__main__":
    logging.basicConfig(level="DEBUG")
    logging.info(__description__)
    payload = {'tenantId': '98b751a7-cbcb-4fb2-8a0c-dc5c0ebd7831'}
    delta_time = timedelta(days=5)
    now = datetime.now()
    end_time = now.strftime("%Y-%m-%dT%H:%M:%S.000Z")
# Print the local time from a week ago
    start_time = (now - delta_time).strftime("%Y-%m-%dT%H:%M:%S.000Z")
    payload_device_metrics = {'start': start_time, 
                              'end': end_time,
                              'aggregation':'HOUR'}
    headers = {'accept': 'application/json', 'Grpc-Metadata-Authorization': f'Bearer {credentials.API_HELIUM_IOT_XYZ}'}
    r = requests.get(f'{credentials.API_HTTPS_ENDPOINT}/api/devices/{credentials.DEV_EUI_3}/metrics', params=payload_device_metrics,headers=headers)
    # Print the downlink id
    logging.debug(r.json())
    if "metrics" in r.json():
        metrics = r.json()["metrics"]
        latitude = []
        longitude = []
        timestamps = []
        for i in range(len(metrics['latitude']['timestamps'])):
            time_str = metrics['latitude']['timestamps'][i]
            #w = re.compile('(?P<year>[\d]+)-(?P<month>[\d]+)-(?P<day>[\d]+)T(?P<hour>[\d]+):(?P<min>[\d]+):(?P<sec>[\d]+)\w',re.ASCII)
            #if w.match(time_str):

            if metrics['latitude']['datasets'][0]['data'][i] != 0 or metrics['longitude']['datasets'][0]['data'][i] != 0: 
                latitude.append(metrics['latitude']['datasets'][0]['data'][i])
                longitude.append(metrics['longitude']['datasets'][0]['data'][i])
                timestamps.append(metrics['latitude']['timestamps'][i])
        logging.debug(f"latitudes - {latitude}")
        if latitude:
            creat_map(latitude,longitude,timestamps)
            s = get_ch()
        else:
            logging.info(f"there is not locatations in time {start_time}-{end_time}")
    else:       
        logging.error(f"no key metrics , incorrect answer")



