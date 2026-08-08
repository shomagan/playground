#include "bell202data.h"
#include "modbus.h"
#include <iterator>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
/*memset*/
#include <cstring>
using namespace std;
enum class State {
    IDLE,
    CAPTURE_LINE,
    START_BIT,
    DATA_BIT,
    STOP_BIT,
    LINE_ERROR
};
            //---modem BELL 202---
//  +-------------------------------+---------------------------------------T------------�
//  �                               � data start from start bit(state - 0)  �900 Hz      �
//  �     1200 Hz capture line      �state0 - 2200 Hz                       � sct        �
//  �                               �state1 - 1200 Hz                       � (soft      �
//  �                               �                                       �  carrier   �
//  �                               �                                       �  turn off) �
//  L-------------------------------+---------------------------------------+-------------
// 1)check line before receive first start bit(after capture signal)
// 2)check start bit '0', then check 12 data bits, then check stop bit '1'
// time between each data in raw_data_input is 1/1200/64 sec, so we can use 13 uS as time unit
/*      .        .
      .   .     .
    .      .   .
            . .
             .

                            */

class Bell202Modulation{
public:

    Bell202Modulation(const struct bell202_data data[PACKET_NUMBER]) {
        for (int i = 0; i < PACKET_NUMBER; ++i) {
            vector<uint16_t> packet(data[i].p_data, data[i].p_data + data[i].size);
            raw_data_input.push_back(packet);
        }
    }
    virtual ~Bell202Modulation() = default;

    void make_response(){
        demodulate();
        int count = 0;
        for(const auto& packet : demodulated_data_input){
            if(packet.size()){
                memset(modbus_packet, 0, sizeof(modbus_packet));
                memcpy(modbus_packet, packet.data(), packet.size());
                if(modbus_packet_for_me(modbus_packet, packet.size()) &&
                        it_modbus_request_check(modbus_packet, packet.size()) > 0){
                    size_t size_response = modbus_rtu_packet(modbus_packet, packet.size());
                    demodulated_data_output.push_back(vector<uint8_t>(modbus_packet, modbus_packet + size_response));
                    /*print out */
                    count++;
                }
            }
        }
        safe_to_file();
    }

private:
    const static int TIME_INTERVAL_US = 13;
    const static int DELTA_THRESHOLD = 100;
    const static int MAX_AMPLITUDE = 4095;
    const static int MIDLE_AMPLITUDE = 2048;
    const static int MIN_AMPLITUDE = 0;
    const static int HALF_1200_HZ = 416;
    const static int FULL_1200_HZ = 832;
    const static int HALF_2200_HZ = 227;
    const static int FULL_2200_HZ = 454;
    const static int BIT_NUMBER = 12;

    vector<vector<uint16_t>> raw_data_input;
    vector<vector<uint8_t>> demodulated_data_input;
    vector<vector<uint8_t>> demodulated_data_output;
    vector<vector<uint16_t>> raw_data_output;
    u8 modbus_packet[256];


    void safe_to_file() {/*output.txt */
        const std::string filename = "output.txt";
        FILE* file = fopen(filename.c_str(), "wb");
        if (file) {
            for (const auto& packet : demodulated_data_output) {
                for(const auto& byte : packet) {
                    /*comma separated hex values*/
                    string hex_value = to_string(static_cast<int>(byte));
                    fwrite(hex_value.c_str(), sizeof(char), hex_value.size(), file);
                    if(&byte != &packet.back()) {
                        fwrite(",", sizeof(char), 1, file);
                    }
                }
                fwrite("\n", sizeof(char), 1, file);
            }
            fclose(file);
        }
    }


    static bool hamming_decode_byte(uint16_t code, uint8_t& data){
        int syndrome = 0;
        auto check_parity = [&](int p) {
            int parity = 0;
            for (int bit = 1; bit <= 12; ++bit) {
                if ((bit & p) != 0) {
                    parity ^= ((code >> (bit - 1)) & 0x1u);
                }
            }
            return parity;
        };
        if (check_parity(1)) syndrome += 1;
        if (check_parity(2)) syndrome += 2;
        if (check_parity(4)) syndrome += 4;
        if (check_parity(8)) syndrome += 8;
        if (syndrome > 0 && syndrome <= 12) {
            code ^= (1u << (syndrome - 1)); // correct one-bit error
        } else if (syndrome != 0) {
            return false; // double-bit error detected, not corrected
        }
        data = 0;
        int out_bit = 0;
        for (int bit = 1; bit <= 12; ++bit) {
            if (bit == 1 || bit == 2 || bit == 4 || bit == 8) continue;
            data |= ((code >> (bit - 1)) & 0x1u) << out_bit;
            ++out_bit;
        }
        return true;
    }

    void demodulate(){
        cout<<"Demodulating BELL 202 data..."<<endl;
        for (const auto& packet : raw_data_input) {
            vector<uint8_t> demodulated_packet;
            bool up = false;
            uint16_t prev = 0;
            int max_index = 0;
            int min_index = 0;
            int click_index = 0;
            int global_bit = 0;
            State state = State::CAPTURE_LINE;
            if(packet.size()){
                prev = packet[0];
            }
            bool mark = false;
            uint16_t temp_byte = 0;
            int bit_number = 0;
            for (size_t i = 1; i < packet.size(); ++i) {
                switch(state){
                    case State::CAPTURE_LINE:
                        if(up){
                            if(packet[i] < prev){
                                if((((i-1) - min_index)*13 <= HALF_1200_HZ + TIME_INTERVAL_US*2 )&&
                                    (((i-1) - min_index)*13 >= HALF_1200_HZ - TIME_INTERVAL_US*2 )){
                                    state = State::START_BIT;
                                }
                                up = false;
                                max_index = i-1;
                            }
                        }else{
                            if(packet[i] > prev){
                                if((((i-1) - max_index)*13 <= HALF_1200_HZ + TIME_INTERVAL_US*2 )&&
                                    (((i-1) - max_index)*13 >= HALF_1200_HZ - TIME_INTERVAL_US*2 )){
                                    state = State::START_BIT;
                                }
                                up = true;
                                min_index = i-1;
                            }
                        }
                        break;
                    case State::START_BIT:
                        if(up){
                            if(packet[i] < prev){
                                if((((i-1) - min_index)*13 <= HALF_2200_HZ + TIME_INTERVAL_US )&&
                                    (((i-1) - min_index)*13 >= HALF_2200_HZ - TIME_INTERVAL_US )){
                                    state = State::DATA_BIT;
                                    bit_number = 0;
                                    temp_byte = 0;
                                    click_index = i-1;
                                    global_bit = 0;
                                }
                                up = false;
                                max_index = i-1;
                            }
                        }else{
                            if(packet[i] > prev){
                                if((((i-1) - max_index)*13 <= HALF_2200_HZ + TIME_INTERVAL_US )&&
                                    (((i-1) - max_index)*13 >= HALF_2200_HZ - TIME_INTERVAL_US )){
                                    state = State::DATA_BIT;
                                    bit_number = 0;
                                    temp_byte = 0;
                                    global_bit = 0;
                                    click_index = i-1;
                                }
                                up = true;
                                min_index = i-1;
                            }
                        }
                        break;
                    case State::DATA_BIT:
                    case State::STOP_BIT:
                    /*at least half wave passed  */
                        if(up){
                            if(packet[i] < prev){
                                if((((i-1) - min_index)*13 <= HALF_2200_HZ + TIME_INTERVAL_US)&&
                                    (((i-1) - min_index)*13 >= HALF_2200_HZ - TIME_INTERVAL_US )){
                                    mark = false;
                                }else if((((i-1) - min_index)*13 <= HALF_1200_HZ + TIME_INTERVAL_US)&&
                                    (((i-1) - min_index)*13 >= HALF_1200_HZ - TIME_INTERVAL_US)){
                                    mark = true;
                                }
                                up = false;
                                max_index = i-1;
                            }
                        }else{
                            if(packet[i] > prev){
                                if((((i-1) - max_index)*13 <= HALF_2200_HZ + TIME_INTERVAL_US)&&
                                    (((i-1) - max_index)*13 >= HALF_2200_HZ - TIME_INTERVAL_US)){
                                    mark = false;
                                }
                                if((((i-1) - max_index)*13 <= HALF_1200_HZ + TIME_INTERVAL_US)&&
                                    (((i-1) - max_index)*13 >= HALF_1200_HZ - TIME_INTERVAL_US)){
                                    mark = true;
                                }
                                up = true;
                                min_index = i-1;
                            }
                        }
                        if(((i - click_index)*13) >= ((bit_number+1)*FULL_1200_HZ+HALF_2200_HZ)){
                            if(bit_number<BIT_NUMBER){
                                if(mark){
                                    temp_byte |= (0x800 >> bit_number);
                                }else{
                                    temp_byte &= ~(0x800 >> bit_number);
                                }
                                bit_number++;
                            }else if(bit_number == BIT_NUMBER && mark == true){
                                uint8_t decoded_byte = 0;
                                uint16_t code = temp_byte;
                                if(hamming_decode_byte(code, decoded_byte)){
                                    demodulated_packet.push_back(decoded_byte);
                                }
                                state  = State::START_BIT;
                                bit_number = 0;
                            }else{
                                state = State::LINE_ERROR;
                            }
                        }
                        break;
                    case State::LINE_ERROR:
                        std::cout << "line error, reset state machine at " << i << std::endl;
                        break;
                    default:
                        std::cout << "incorrect modulate state" << std::endl;
                        break;
                }
                if(state == State::LINE_ERROR){
                    break;
                }
                prev = packet[i];
            }
            demodulated_data_input.push_back(demodulated_packet);
        }
    }
};
