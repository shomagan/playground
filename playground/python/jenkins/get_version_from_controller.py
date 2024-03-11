import last_version
import requests
import json
from urllib3.util.retry import Retry
from requests.adapters import HTTPAdapter

# Define your own exception
class Error(Exception):
    pass


def main():
    s = requests.Session()
    retries = Retry(total=3, backoff_factor=0.5, status_forcelist=[500, 502, 503, 504])
    s.mount('http://', HTTPAdapter(max_retries=retries))
    response = requests.get('http://'+last_version.plc_ip_address+':80/get_json.cgi', headers={'Json': '{"request":"os_param"}'})
    json_data = json.loads(response.content)
    if 'current' not in json_data or 'main' not in json_data or 'os_ver' not in json_data:
        raise Error('os param request content mistake')
    else:
        print(json_data["os_ver"])


if __name__ == "__main__":
    main()
