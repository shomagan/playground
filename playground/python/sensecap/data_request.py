import credentials
import sensecap_decode
import sys
import re
import optparse
import logging
__description__ = "simply adata handling from a helium io console"
import os
import requests

# Configuration.

# This must point to the API interface.
server = credentials.API_ENDPOINT

# The DevEUI for which you want to enqueue the downlink.
dev_eui = credentials.DEV_EUI_3

# The API token (retrieved using the web-interface).
api_token = credentials.API_HELIUM_IOT_XYZ

if __name__ == "__main__":
    logging.basicConfig(level="INFO")
    logging.info(__description__)
    payload = {'tenantId': '98b751a7-cbcb-4fb2-8a0c-dc5c0ebd7831'}
    payload_device_metrics = {'start': '2024-02-10T19:44:19.705Z', 
                              'end': '2024-02-19T19:44:19.705Z',
                              'aggregation':'DAY'}
    headers = {'accept': 'application/json', 'Grpc-Metadata-Authorization': f'Bearer {credentials.API_HELIUM_IOT_XYZ}'}
    r = requests.get(f'{credentials.API_HTTPS_ENDPOINT}/api/devices/{credentials.DEV_EUI_3}/metrics', params=payload_device_metrics,headers=headers)
    # Print the downlink id
    logging.info(r.json())




