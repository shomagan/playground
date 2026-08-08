#ifndef MODBUS_H
#define MODBUS_H 1

/*add includes below */
#include "regs_description.h"
/*add includes before */
#ifdef __cplusplus
   extern "C" {
#endif
/*add functions and variable declarations below */
enum modbus_type{
   MODBUS_RTU_PACKET = 1,
   MODBUS_TCP_PACKET
};
#define MAX_MODBUS_PACKET 256
#define MODBUS_MAX_MODBUS_REGS_NUM 125
#define MODBUS_TCP_HEADER_SIZE 6u
#define ILLEGAL_FUNCTION 0x01
#define ILLEGAL_DATA_ADDRESS 0x02
#define ILLEGAL_DATA_VALUE 0x03
#define SLAVE_DEVICE_FAILURE 0x04
#define ACKNOWLEDGE 0x05
#define SLAVE_DEVICE_BUSY   0x06
#define NEGATIVE_ACKNOWLEDGE   0x07
#define MEMORY_PARITY_ERROR   0x08
#define GATEWAY_PATH_UNAVAILABLE   0x0A
#define GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND   0x0B
#define BEREMIZ_ARRAY_ADDRESS_SPACE_START 30000u
#define BEREMIZ_ADDRESS_SPACE_START       40000u
#define SELF_MDB_ADDRESS_SPACE_START      0u
#define MODBUS_BROADCAST_ADDRESS 255
#define MDB_COILS_RW 1
#define MDB_INPUT_DISCRETES_RO 2
#define MDB_HOLDING_REGS_RW 3
#define MDB_INPUT_REGS_RO 4
#define MODBUS_MAX_WORD_NUM 112
#define MODBUS_MAX_READ_BITS 240
#define MODBUS_PACKET_LEN 255
enum packet_socket_channels{
    PACKET_CHANNEL_UDP = 10 + 1,
    PACKET_CHANNEL_TCP
};


u8 htons_buff(u16 *buff, u8 word_numm);
u16 modbus_crc16(u8* pckt, u16 len);
u8 modbus_crc16_check(u8* pckt,u16 lenght);
u8 modbus_packet_for_me(u8* pckt,u16 lenght);
u16 modbus_rtu_packet (u8* pckt,u16 length);
u16 modbus_tcp_packet (u8* pckt,u16 length);
u8 generate_error_packet(u8* pckt,u8 error_code);
u8 modbus_err_packet_type(u8 rtu_flag,u8* pckt,u8 error_code);
u8 it_modbus_simply_check(u8* pckt,u16 lenght);
int it_modbus_request_check(u8* buff,u16 length);
int it_modbus_respond_check(u8* buff,u16 length);
int it_modbus_tcp_full_check(u8* buff,u16 len);
int modbus_make_packet (u8  slave_address,u8  function, u16 start_addr,
                         u16 reg_num, u8 * data_to_write, u8 * packet);


/*add functions and variable declarations before */
#ifdef __cplusplus
}
#endif
#endif /* MODBUS_H */
