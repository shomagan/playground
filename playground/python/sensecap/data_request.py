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
import json
from datetime import datetime, timedelta
import asyncio
import telegram
try:
    import msvcrt
    PLATFORM = "win"
except ImportError:
    PLATFORM = "unix"
    import tty
    import termios
    from select import select

# Configuration.


async def main():
    bot = telegram.Bot(credentials.tel_bot_api)
    latitude, longitude, timestamps, battery_percent = get_tracker_data(credentials.DEV_EUI_3)
    if latitude:
        create_map("tracker_3", latitude, longitude, timestamps)
        file_to_send = open("tracker_3.html", "rb")
        await bot.send_message(credentials.tel_home_id,
                               f"got new locations for tracker 3, battery - {battery_percent}")
        await bot.send_document(credentials.tel_home_id, file_to_send)
        file_to_send.close()
    else:
        await bot.send_message(credentials.tel_home_id,
                               f"there is no locations update for tracker 3, battery - {battery_percent}")
        logging.info(f"there is no locations update for tracker 3, battery - {battery_percent}")
    latitude, longitude, timestamps, battery_percent = get_tracker_data(credentials.DEV_EUI_2)
    if latitude:
        create_map("tracker_2", latitude, longitude, timestamps)
        file_to_send = open("tracker_2.html", "rb")
        await bot.send_message(credentials.tel_home_id,
                               f"got new locations for tracker 2, battery - {battery_percent}")
        await bot.send_document(credentials.tel_home_id, file_to_send)
        file_to_send.close()
    else:
        await bot.send_message(credentials.tel_home_id,
                               f"there is no locations update for tracker 2, battery - {battery_percent}")
        logging.info(f"there is not locations in time for tracker 2 , battery - {battery_percent}")
    return


def create_map(dev_name, latitudes, longitudes, timestamps):
    map_f = folium.Map(location=[latitudes[0], longitudes[0]], zoom_start=13)
    for i in range(len(latitudes)):
        folium.Marker([latitudes[i], longitudes[i]], popup=timestamps[i], tooltip=timestamps[i]).add_to(map_f)
    map_f.save(f"{dev_name}.html")


def get_tracker_data(dev_eui):
    delta_time = timedelta(days=2)
    now = datetime.now()
    end_time = now.strftime("%Y-%m-%dT%H:%M:%S.000Z")
    # Print the local time from a week ago
    start_time = (now - delta_time).strftime("%Y-%m-%dT%H:%M:%S.000Z")
    payload_device_metrics = {'start': start_time,
                              'end': end_time,
                              'aggregation': 'HOUR'}
    headers = {'accept': 'application/json', 'Grpc-Metadata-Authorization': f'Bearer {credentials.API_HELIUM_IOT_XYZ}'}
    r = requests.get(f'{credentials.API_HTTPS_ENDPOINT}/api/devices/{dev_eui}/metrics',
                     params=payload_device_metrics, headers=headers)
    # Print the downlink id
    logging.debug(r.json())
    latitude = []
    longitude = []
    timestamps = []
    battery_percent = 0.0
    if "metrics" in r.json():
        metrics = r.json()["metrics"]
        for i in range(len(metrics['latitude']['timestamps'])):
            if (metrics['latitude']['datasets'][0]['data'][i] != 0 or
                    metrics['longitude']['datasets'][0]['data'][i] != 0):
                latitude.append(metrics['latitude']['datasets'][0]['data'][i])
                longitude.append(metrics['longitude']['datasets'][0]['data'][i])
                timestamps.append(metrics['latitude']['timestamps'][i])
        logging.debug(f"latitudes - {latitude}")
        for battery in (metrics['battery']['datasets'][0]['data']):
            if float(battery) > 0.1:
                battery_percent = float(battery)


    else:
        logging.error(f"no key metrics , incorrect answer")
    return latitude, longitude, timestamps, battery_percent


if __name__ == "__main__":
    logging.basicConfig(level="INFO")
    logging.info(__description__)
    asyncio.run(main())




