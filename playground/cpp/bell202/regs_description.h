#ifndef __REGS_DESCRIPTION_H__
#define __REGS_DESCRIPTION_H__ 1
#include "bell202data.h"

typedef enum{
    GUID_ADDRESS_MASK   =    0x0000FFFF,/*!< modbus address(u16 based) mask*/
    GUID_TYPE_MASK      =    0x0ff00000,
    GUID_OS_HEAD        =    0x00000000,
    GUID_USER_HEAD      =    0x10000000,
    GUID_USER_MDB_FIELD_MASK = 0x000F0000,/*!< modbus field mask*/
    GUID_USER_IS_01_MD  =      0x00010000,/*!< its belong modbus coils areas 01*/
    GUID_USER_IS_02_MD  =      0x00020000,/*!< its belong modbus input discretes areas 02*/
    GUID_USER_IS_03_MD  =      0x00030000,/*!< its belong modbus holdings regs areas 03*/
    GUID_USER_IS_04_MD  =      0x00040000/*!< its belong modbus input regs areas 04*/
}regs_description_guid;

typedef enum {
    U8_REGS_FLAG = (1<<0),/*!<unsigned char type(1 byte) */
    U16_REGS_FLAG = (1<<1),/*!<unsigned short type(2 byte) */
    U32_REGS_FLAG = (1<<2),/*!<unsigned int type(4 byte) */
    U64_REGS_FLAG = (1<<3),/*!<unsigned long long type(8 byte) */
    I8_REGS_FLAG = (1<<4),/*!<signed char type(1 byte) */
    S16_REGS_FLAG = (1<<5),/*!<signed short type(2 byte) */
    S32_REGS_FLAG = (1<<6),/*!<signed int type(4 byte) */
    S64_REGS_FLAG = (1<<7),/*!<signed long long type(8 byte) */
    FLOAT_REGS_FLAG = (1<<8),/*!<float type(4 byte) */
    DOUBLE_REGS_FLAG = (1<<9),/*!<double type(8 byte) */
    INT_REGS_FLAG = (1<<10),/*!<signed int type(4 byte) */
    TIME_REGS_FLAG = (1<<11)/*!<unsigned int type(4 byte) */
}regs_flag_t;

typedef union {
    float op_f;
    double op_d;
    int op_int;
    u8 op_u8;
    u16 op_u16;
    u32 op_u32;
    u64 op_u64;
    i8 op_i8;
    s16 op_i16;
    s32 op_s32;
    s64 op_s64;
} operand_t;

typedef struct {
    regs_flag_t flag;
    operand_t value;
} regs_access_t;

typedef struct {/*struct for reg description generic from regs.h*/
  u8* p_value;/*!<pointer to value */
  const char* description;  /*!<full name pointer */
  const char* name;  /*!<short uniq name */
  u16 type;   /*!<variable type u8 u16 u32 float double */
  u16 size;    /*!<array size in type value not in bytes */
  u32 guid;   /*!<see regs_description_guid */
} regs_description_t;
/** @brief use it for field modbus_description in regs_description_t*/
#define RD_MDB_ADDRESS(desc) (desc&0x0000ffff)
/** @brief use it for field modbus_description in regs_description_t*/
#define RD_MDB_FUNCTION(desc) ((((u32)desc)>>16)&0x0000000f)

extern u16 digital_input;
extern u16 digital_output;
extern u16 mdb_addr;/*modbus address we should respond to*/
extern u8 interface_ip[4];/*default ip address can not be changed*/
/*max value for each element is 1000, Illegal Data Value  should be handled if exceeded*/
extern u16 mdb_holding_regs[16];
extern regs_description_t const regs_description_user[];
u8 regs_size_in_byte(regs_flag_t type);
int regs_get(void *reg_address, regs_access_t *reg);
int regs_get_buffer(void * reg_address,u8* buffer_to,u16 byte_num);
int regs_set(void *reg_address, regs_access_t *reg);
int regs_set_buffer(void *reg_address, u8* buffer_from, u16 byte_num);
void regs_description_init(void);
u16 regs_description_get_pointer_by_modbus(u16 modbus_address, u8 modbus_function, u8 **p_value);
u16 regs_description_get_pointer_by_modbus_discrete(u16 modbus_address, u8 modbus_function, u16 *bit_offset, u8 **p_value);
#endif /* __REGS_DESCRIPTION_H__ */
