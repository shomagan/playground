import telegram_api
import os


def main():
    command_is_outdated = 0
    bot_message = telegram_api.get_updates_json()
    last_message_id = -1
    date_from_file = -1
    if len(bot_message['result']) != 0:
        last_message_id = last_message(bot_message)
    last_handled_date = "last_handled_command.txt"
    if not os.path.exists(last_handled_date):
        command_is_outdated = 1
        if last_message_id >= 0:
            last_handled_date_file = open(last_handled_date, 'w')
            last_handled_date_file.write(str(bot_message['result'][last_message_id]['message']['date']))
            last_handled_date_file.close()
    else:
        last_handled_date_file = open(last_handled_date, 'r')
        str_temp = last_handled_date_file.read()
        if str_temp.isdigit():
            date_from_file = int(str_temp)
        last_handled_date_file.close()
        if last_message_id >= 0:
            last_handled_date_file = open(last_handled_date, 'w')
            last_handled_date_file.write(str(bot_message['result'][last_message_id]['message']['date']))
            last_handled_date_file.close()
    if last_message_id >= 0:
        if date_from_file < int(bot_message['result'][last_message_id]['message']['date']):
            command_is_outdated = 1
        if command_is_outdated:
            print(bot_message['result'][last_message_id]['message']['text'])
            if "test" in bot_message['result'][last_message_id]['message']['text'].lower():
                telegram_api.send_message("sofi_plc_testing are starting")
            if "build" in bot_message['result'][last_message_id]['message']['text'].lower():
                telegram_api.send_message("sofi_plc_building are starting")
            if "load" in bot_message['result'][last_message_id]['message']['text'].lower():
                telegram_api.send_message("sofi_plc_loading are starting")
        else:
            print('none')


def last_message(data):
    last_date = 0
    last_id = -1
    for i in range(len(data['result'])):
        last_date_temp = int(data['result'][i]['message']['date'])
        if last_date_temp > last_date:
            last_date = last_date_temp
            last_id = i
    return last_id


if __name__ == "__main__":
    main()
