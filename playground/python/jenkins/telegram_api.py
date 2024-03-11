import requests
import json
import credentials
url = credentials.url 
home_id = credentials.home_id


def get_updates_json():
    params = {'timeout': 100, 'offset': None}
    response = requests.get(url + 'getUpdates', data=params)
    json_data = json.loads(response.content)
    return json_data


def last_update(data):
    results = data['result']

    total_updates = len(results) - 1
    return results[total_updates]


def get_chat_id(update):
    chat_id = update['message']['chat']['id']
    return chat_id


def send_message(text, chat=home_id):
    params = {'chat_id': chat, 'text': text}
    response = requests.post(url + 'sendMessage', data=params)
    return response


def main():
    get_updates_json()


if __name__ == "__main__":
    main()

