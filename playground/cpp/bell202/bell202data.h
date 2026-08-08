#ifndef __BELL202DATA_H__
#define __BELL202DATA_H__ 1
#define PACKET_NUMBER 34

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef const unsigned short uc16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char i8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef const unsigned short uc16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
typedef signed char i8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long long s64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
struct bell202_data {
    u16 * p_data;
    int size;
};
/* provides PACKET_NUMBER packets with general bell202 data
Each packet starts with capture-line signal and provides enough bytes for modbus request packet */
int bell202_get_data(struct bell202_data data[PACKET_NUMBER]);

#ifdef __cplusplus
}
#endif
#endif /* __BELL202DATA_H__ */
