/**
 * @file modbus.c
 * @defgroup free_rtos/src
 * @ingroup free_rtos/src
 * @version 0.1
 * @brief  TODO!!! write brief in
 */

#ifndef MODBUS_C
#define MODBUS_C 1
#include <stdio.h>
#include <string.h>
#include "modbus.h"
#ifndef BIT
#define BIT(nr)                 (1UL << (nr))
#endif

/*  "01 - Read Coils":                ('1',  'req_input', 2000, "BOOL",  1, "Q", "X", "Coil"),
    "02 - Read Input Discretes":      ('2',  'req_input', 2000, "BOOL",  1, "I", "X", "Input Discrete"),
    "03 - Read Holding Registers":    ('3',  'req_input',  125, "WORD", 16, "Q", "W", "Holding Register"),
    "04 - Read Input Registers":      ('4',  'req_input',  125, "WORD", 16, "I", "W", "Input Register"),
    "05 - Write Single coil":         ('5', 'req_output',    1, "BOOL",  1, "Q", "X", "Coil"),
    "06 - Write Single Register":     ('6', 'req_output',    1, "WORD", 16, "Q", "W", "Holding Register"),
   ?"15 - Write Multiple Coils":     ('15', 'req_output', 1968, "BOOL",  1, "Q", "X", "Coil"),
    "16 - Write Multiple Registers": ('16', 'req_output',  123, "WORD", 16, "Q", "W", "Holding Register")}
*/
#define OWN_VARIABLE_BLOK_SIZE 0
#define REG_AI_SHIFT 0

u16_t htons(u16_t n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

u16_t ntohs(u16_t n)
{
  return htons(n);
}

u32_t htonl(u32_t n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

u32_t ntohl(u32_t n)
{
  return htonl(n);
}

u8 htons_buff(u16 *buff,u8 word_numm){
    u8 i;
    for (i = 0;i<word_numm;i++){
        buff[i] = htons(buff[i]);
    }
    return i;
}

u16 modbus_crc16(u8* pckt, u16 len){
    u16  result;
    u16 i, j;
    len = len > 254?254:len;
    result = 0xFFFF;
    for (i = 0; i < len; i++)  {
        result ^= pckt[i];
        for (j = 0; j < 8; j++) {
            if ((result & 0x01) == 1){
                result = (result >> 1) ^ 0xA001;
            }else{
                result >>= 1;
            }
        }
    }
    return result;
}

u8 modbus_crc16_check(u8* pckt,u16 lenght){
    if (lenght < 3){
        return 0;
    }
    if (modbus_crc16 (pckt, lenght-2) != *(u16*)(void*)(pckt + lenght - 2)) {
        return 0;
    }
    return 1;
}

u8 modbus_packet_for_me(u8* pckt,u16 lenght){
    if (lenght){
        if ((pckt[0] != (u8)mdb_addr)  &&
            (pckt[0] != MODBUS_BROADCAST_ADDRESS)){
            return 0;
        }else{
            return 1;
        }
    }else{
        return 0;
    }
    return 0;
}

u16 modbus_tcp_packet (u8* pckt, u16 len){
    u16 reply_len;
    /*add fake crc for modbus_rtu_packet*/
    *(u16*)(void*)(&pckt[len]) = modbus_crc16((u8*)(pckt+MODBUS_TCP_HEADER_SIZE),\
                                        len-MODBUS_TCP_HEADER_SIZE);
    reply_len = modbus_rtu_packet(&pckt[MODBUS_TCP_HEADER_SIZE],\
                                  len-MODBUS_TCP_HEADER_SIZE+2);
    if(reply_len>4){
        reply_len -= 2;/*minos crc*/
        pckt[4] = (u8)(reply_len>>8);/*to header mdb_tcp*/
        pckt[5] = (u8)(reply_len);/*to header mdb_tcp*/
        reply_len = reply_len+MODBUS_TCP_HEADER_SIZE;/*mdb_tcp header*/
    }
    return reply_len;
}


u16 modbus_rtu_packet (u8* pckt,u16 len_in){
    u16 len_reply;
    u8 error, function;
    u16 start_address, regs_num, num_bit;
    u8 * p_value = NULL;
    (void)len_in;
    len_reply = 0;
    error = 0;
    function = pckt[1];
    start_address=0;
    switch (function) {
    case 1:
    case 2:
        {
            u16 coil_number;
            u16 bit_offset;
            u16 max_coils_number = 0;
            u16 written = 0;
            coil_number = (u16)(pckt[2] << 8) + pckt[3];		/* 2-3 bit start*/
            num_bit = (u16)(pckt[4] << 8) + pckt[5];		/* 4-5 num bit */
            len_reply = (u8)((num_bit+7)/8);
            while(num_bit){
              max_coils_number = regs_description_get_pointer_by_modbus_discrete(coil_number,function,&bit_offset, &p_value);
              if (p_value != NULL && max_coils_number > 0){
                u16 bit_to_read = num_bit > max_coils_number ? max_coils_number : num_bit;
                u8 bit_n = 0;
                u8 byte_n = 0;
                u16 i;
                for (i=0; i<bit_to_read; i++){
                    bit_n = (i+written)%8;
                    byte_n = (u8)((i+written)/8);
                    if(p_value[(bit_offset+i)/8] & BIT((bit_offset+i)%8)){
                        *(u8 *)(pckt+3+byte_n)|=BIT(bit_n);
                    }else{
                        *(u8 *)(pckt+3+byte_n)&=~BIT(bit_n);
                    }
                }
                if(bit_to_read && bit_n < 7){
                    *(u8 *)(pckt+3+byte_n)&=~(0xff<<(bit_n+1));
                }
                num_bit -= bit_to_read;
                coil_number += bit_to_read;
                written +=  bit_to_read;
              }else{
                  error = ILLEGAL_DATA_ADDRESS;
                  break;
              }
            }
            if (!error) {
              pckt[2] = (u8)len_reply;
              len_reply += 5;
            }
        }
        break;
    case 3:
    case 4:
        {/*read holding regs*/
          u16 max_byte_num = 0;
          start_address = (u16)(pckt[2] << 8) + pckt[3];
          regs_num = (u16)(pckt[4] << 8) + pckt[5];
          if ((regs_num < 1) || (regs_num>MODBUS_MAX_MODBUS_REGS_NUM)) {
              error = ILLEGAL_DATA_VALUE;
          } else {
            u16 written = 0;
            len_reply = (u8)(regs_num << 1);
            while(regs_num){
              max_byte_num = regs_description_get_pointer_by_modbus(start_address,function,&p_value);
              if (p_value == NULL || max_byte_num == 0){
                error = ILLEGAL_DATA_ADDRESS;
                break;
              }else{
                u16 regs_to_read = regs_num > (max_byte_num>>1) ? (max_byte_num>>1) : regs_num;
                if ((regs_num < 1) || (regs_num>MODBUS_MAX_MODBUS_REGS_NUM)) {
                    error = ILLEGAL_DATA_VALUE;
                    break;
                } else if(!error){
                  int res_temp;
                  res_temp = regs_get_buffer(p_value,(u8*)(pckt+3+written),regs_to_read*2);
                  if (res_temp!=0) {
                      if(res_temp == -ILLEGAL_DATA_ADDRESS){
                          error = ILLEGAL_DATA_ADDRESS;
                      }else if(res_temp == -ILLEGAL_DATA_VALUE){
                          error = ILLEGAL_DATA_VALUE;
                      }else{
                          error = ILLEGAL_DATA_ADDRESS;
                      }
                      break;
                  }
                  written += regs_to_read*2;
                  regs_num -= regs_to_read;
                  start_address += regs_to_read;
                }
              }
            }
            if (!error) {
              htons_buff((u16 *)(void*)(pckt+3), (u8)(written>>1));
              pckt[2] = (u8)len_reply;
              len_reply += 5;
            }
          }
        }
        break;
    /*case 5:
        {
            u16 coil_number,status,max_coils_number;
            u16 bit_offset = 0;
            u8 on;
            coil_number= (u16)((u16)pckt[2] << 8) + (u16)pckt[3];
            status  = (u16)((u16)pckt[4] << 8) + ((u16)pckt[5]);
            max_coils_number = regs_description_get_pointer_by_modbus_discrete(coil_number,RD_MDB_FUNCTION(GUID_USER_IS_01_MD),&bit_offset, &p_value);
            if (p_value != NULL && max_coils_number > 0){
              if(status == 0xff00){
                on = 1;
              }else if (status == 0x0000){
                on = 0;
              }else{
                  error = ILLEGAL_DATA_VALUE;
              }
              if (!error){
                  if(on){
                     p_value[bit_offset/8] |= BIT(bit_offset%8);
                  }else{
                     p_value[bit_offset/8] &= ~BIT(bit_offset%8);
                  }
              }
              len_reply = 8;
            }else{
                error = ILLEGAL_DATA_ADDRESS;
            }
        }
        break;
    case 6:
        {
            regs_access_t reg;
            u16 max_byte_num = 0;
            reg.flag = U16_REGS_FLAG;
            start_address = (u16)(pckt[2] << 8) + pckt[3];
            max_byte_num  =  regs_description_get_pointer_by_modbus(start_address, RD_MDB_FUNCTION(GUID_USER_IS_03_MD), &p_value);
            if (p_value==NULL || max_byte_num == 0){
                error = ILLEGAL_DATA_ADDRESS;
            }
            if (!error){
                int res_temp;
                reg.value.op_u16 = ((u16)(pckt[4] << 8) + pckt[5]);
                len_reply = 8;
                res_temp = regs_set(p_value, &reg);
                if (res_temp < 0) {
                    if(res_temp == -ILLEGAL_DATA_ADDRESS){
                        error = ILLEGAL_DATA_ADDRESS;
                    }else if(res_temp == -ILLEGAL_DATA_VALUE){
                        error = ILLEGAL_DATA_VALUE;
                    }else{
                        error = ILLEGAL_DATA_ADDRESS;
                    }
                }
            }
        }
        break;*/
    case 15:
    {
        u16 coil_number, max_coils_number;
        u16 written = 0;
        u16 bit_offset = 0;
        coil_number = (u16)(pckt[2] << 8) + pckt[3];		/* 2-3 bit start*/
        num_bit = (u16)(pckt[4] << 8) + pckt[5];		/* 4-5num bit*/
/*        bytes_num = pckt[6];*/
        if (num_bit == 0) {
          error = ILLEGAL_DATA_VALUE;
        } else{
          while(num_bit){
            max_coils_number = regs_description_get_pointer_by_modbus_discrete(coil_number, RD_MDB_FUNCTION(GUID_USER_IS_01_MD), &bit_offset, &p_value);
            if (p_value !=NULL && max_coils_number){
                u16 coils_to_write = num_bit > max_coils_number ? max_coils_number : num_bit;
                u16 i;
                /*writing*/
                for (i=0; i<coils_to_write; i++){
                  if(pckt[7+(i + written)/8] & BIT((i+written)%8)){
                    p_value[(bit_offset+i)/8] |= BIT((bit_offset+i)%8);
                  }else{
                    p_value[(bit_offset+i)/8] &= ~BIT((bit_offset+i)%8);
                  }
                }
                written += coils_to_write;
                num_bit -= coils_to_write;
                coil_number += coils_to_write;
            }else{
                error = ILLEGAL_DATA_ADDRESS;
            }
          }
          if (!error){
              len_reply = 8;
          }
        }
    }
    break;
    case 16:
        {
        /*write words*/
        u16 max_bytes = 0;
        u16 written = 0;
        start_address = (u16)(pckt[2] << 8) + pckt[3];
        regs_num = ((u16)(pckt[4] << 8) + pckt[5]);
        if((regs_num < 1) || (regs_num>MODBUS_MAX_MODBUS_REGS_NUM)){
            error = ILLEGAL_DATA_VALUE;
        }else{
          htons_buff((u16*)(void*)(pckt+7), (u8)regs_num);
          while(regs_num){
            max_bytes = regs_description_get_pointer_by_modbus(start_address, RD_MDB_FUNCTION(GUID_USER_IS_03_MD), &p_value);
            if (p_value == NULL || max_bytes == 0){
                error = ILLEGAL_DATA_ADDRESS;
                break;
            }else{
                int res_temp;
                u16 words_to_write = regs_num > (max_bytes>>1) ? (max_bytes>>1) : regs_num;
                res_temp = regs_set_buffer(p_value, (u8*)(pckt+7+written), words_to_write*2);
                if (res_temp != 0) {
                    if(res_temp == -ILLEGAL_DATA_ADDRESS){
                        error = ILLEGAL_DATA_ADDRESS;
                    }else if(res_temp == -ILLEGAL_DATA_VALUE){
                        error = ILLEGAL_DATA_VALUE;
                    }else{
                        error = ILLEGAL_DATA_ADDRESS;
                    }
                    break;
                }
                written += words_to_write * 2;
                regs_num -= words_to_write;
                start_address += words_to_write;
            }
          }
          if (!error){
              len_reply = 8;
          }
        }
        }
        break;
    default:
        error = ILLEGAL_FUNCTION;
        break;
    }
    if(error){
        pckt[1] |= 0x80;
        pckt[2] = error;
        len_reply = 5;
    }
    *((u16*)(void*)(&pckt[len_reply-2])) = modbus_crc16 (pckt, len_reply-2);
    return len_reply;
}
u8 generate_error_packet(u8* packet,u8 error_code){
    packet[0] = (u8)mdb_addr;
    packet[1] |=0x80;
    packet[2] = error_code;
    *(u16 *)(void*)(&packet[3]) = modbus_crc16 (packet, 3);
    return 5;
}
u8 modbus_err_packet_type(u8 rtu_flag,u8* packet,u8 error_code){
    u8 len;
    if(rtu_flag == MODBUS_TCP_PACKET){
        generate_error_packet(&packet[6],error_code);
        len = 9;
        packet[5] = len;
    }else{
        generate_error_packet(packet,error_code);
        len = 5;
    }
    return len;
}
u8 it_modbus_simply_check(u8* pckt,u16 length){
    if ((pckt[1]<=16)||(pckt[1]==0x41)){
        if (modbus_crc16_check(pckt, length)){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int it_modbus_request_check(u8* buff,u16 length){
    int result=0;
    u8 first,command;
    if(length <= 256){
        first = 0;
        command = buff[first+1];
        switch (command){
        case 0:
        case 1:
        case 2:
            {
                if(it_modbus_simply_check(buff,8)){
                    result = 1;
                }
            }
            break;
        case 3:
        case 4:
            {
                if(it_modbus_simply_check(buff,8)){
                    result = 1;
                }
            }
            break;
        case 5:
            {
                if(it_modbus_simply_check(buff,8)){
                    result = 1;
                }
            }
            break;
        case 6:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 15:
            {
                u8 coil_bytes;
                coil_bytes = buff[first+6];
                if(it_modbus_simply_check(buff,9+coil_bytes)){
                    result = 1;
                }
            }
            break;
        case 16:
            {
                u8 reg_quant_bytes;
                reg_quant_bytes = buff[first+6];
                if(it_modbus_simply_check(buff,9+reg_quant_bytes)){
                    result = 1;
                }
            }
            break;
        case 0x41:
            if(it_modbus_simply_check(buff,length)){
                result = 1;
            }
            break;
        default:
            result = -1;
            break;
        }
    }
    return result;
}
/**
 * @brief verbose check packet for modbus rtu, all used function
 *
 * */
int it_modbus_respond_check(u8* buff,u16 length){
    int result=0;
    u8 first,command;
    if (length<=256){
        first = 0;
        /*address = buff[first];*/
        command = buff[first+1];
        switch (command){
        case 0:
        case 1:
        case 2:
            {
                u8 byte_number;
                byte_number = buff[first+2];
                if(byte_number<251){
                    if(it_modbus_simply_check(buff,(byte_number + 5))){
                        result = 1;
                    }
                }
            }
            break;
        case 3:
        case 4:
            {
                u8 byte_number;
                byte_number = buff[first+2];
                if(byte_number<251){
                    if(it_modbus_simply_check(buff,(byte_number + 5))){
                        result = 1;
                    }
                }
            }
            break;
        case 5:
            {
                u16 status;
                status  = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                if((status == 0xff00) || (status == 0x0000)){
                    if(it_modbus_simply_check(buff,8)){
                        result = 1;
                    }
                }
            }
            break;
        case 6:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 15:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 16:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 0x41:
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
    }
    return result;
}
/**
 * @brief verbose check packet for modbus, all used function
 * TODO!!! add asserts for buff and length
 * @return 1 if checked
 * @ingroup modbus
 * */
int it_modbus_tcp_full_check(u8* buff,u16 len){
    /*add fake crc for it_modbus_full_check*/
    u8 temp_buff[256];
    memcpy(temp_buff,buff,len);
    *(u16*)(void*)(&temp_buff[len])=modbus_crc16((u8*)(temp_buff+MODBUS_TCP_HEADER_SIZE),\
                                               len-MODBUS_TCP_HEADER_SIZE);
    return (it_modbus_request_check(&temp_buff[MODBUS_TCP_HEADER_SIZE],\
                                 len-MODBUS_TCP_HEADER_SIZE+2)==1);
}

/**
 * @brief make packet from parametrs
 * @param slave_address simply will add to packet
 * @param function simply will add to packet
 * @param start_addr will add to packet with htons
 * @param reg_num will add to packet with htons for functions - 1,2,3,4,15,16
 * @param data_to_write pointer data from to for functions 15,16
 * @param packet where packet will safe
 * @return len made packet
 * @ingroup modbus
 * */
int modbus_make_packet (u8  slave_address,u8  function, u16 start_addr,
                         u16 reg_num, u8 * data_to_write, u8 * packet){
    int byte;
    union {
        u16 u16_field;
        u8  u8_field[2];
    } tmp;
    byte = 0;
    if(packet !=NULL){
        packet[byte++] = slave_address;
        packet[byte++] = function;
        tmp.u16_field = htons(start_addr);
        packet[byte++] = tmp.u8_field[0];
        packet[byte++] = tmp.u8_field[1];
        if (function == 3 || function == 4 || function == 1 || function == 2){
            tmp.u16_field = htons(reg_num);
            packet[byte++] = tmp.u8_field[0];
            packet[byte++] = tmp.u8_field[1];
        }else if((function == 16 ) && (reg_num <= 127)){
            if(data_to_write!=NULL){
                u8 i=0;
                u8 data_first;
                u8 byte_numm;
                tmp.u16_field = htons(reg_num);
                /*safe word num */
                packet[byte++] = tmp.u8_field[0];
                packet[byte++] = tmp.u8_field[1];
                byte_numm = (u8)(reg_num << 1);
                /*safe byte num use only in 16 command*/
                packet[byte++] = byte_numm;
                data_first = (u8)byte;
                /*safe data*/
                for (i=0;i<byte_numm; i++){
                    packet[byte++] = data_to_write[i];
                }
                /*change to modbus indian*/
                htons_buff((u16*)(void*)(&packet[data_first]),(u8)reg_num);
            }else{
                byte = -1;
            }
        }else if((function == 15) && (reg_num <= 254)){
            /*set bits,all coil  regs have bit to byte addreses*/
            if(data_to_write!=NULL){
                u8 byte_count,bit_n,byte_n;
                u8 i = 0;
                tmp.u16_field = htons(reg_num);
                /*safe word num */
                packet[byte++] = tmp.u8_field[0];
                packet[byte++] = tmp.u8_field[1];
                byte_count = (u8)((reg_num+7)/8);
                packet[byte++] = byte_count;
                for (i=0;i<reg_num; i++){
                    bit_n = i%8;
                    byte_n = i/8;
                    if (data_to_write[i]&BIT(0)){
                        packet[byte + byte_n] |= BIT(bit_n);
                    }else{
                        packet[byte + byte_n] &= ~BIT(bit_n);
                    }
                }
                byte += byte_count;
            }else{
                byte = -1;
            }
        }else if(function == 6){
            /*write word*/
            if(data_to_write!=NULL){
                packet[byte++] = data_to_write[1];
                packet[byte++] = data_to_write[0];
            }else{
                byte = -1;
            }
        }else if(function == 5){
            /*set single bits,all coil  regs have bit to byte addreses*/
            u16 status;
            if (data_to_write[0]&BIT(0)){
                status = 0xff00;
            }else{
                status = 0x0000;
            }
            packet[byte++] = ((status>>8)&0xff);
            packet[byte++] = (status&0xff);
        }else{
            byte = -1;
        }
        /*add modbus crc*/
        if(byte > 0){
            *(u16*)(void*)(&packet[byte]) = modbus_crc16(packet,(u16)byte);
            byte +=2;
        }
    }else{
        byte = -1;
    }
    return byte;
}

#endif /*MODBUS_C*/
