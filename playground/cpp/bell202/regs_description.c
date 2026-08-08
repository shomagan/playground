#include "regs_description.h"
#include "string.h"
#include "modbus.h"
u16 digital_input_0 = 0xeffe;
u8 digital_input_1 = 0x55;
u8 digital_input_2 = 0xAA;
u16 digital_output = 0x0010;
u16 mdb_addr = 0x0003;/*modbus address we should respond to*/
u8 interface_ip[4] = {192, 168, 7, 2};/*default ip address can not be changed*/
/*max value for each element is 1000, Illegal Data Value  should be handled if exceeded*/
u16 mdb_holding_regs[16] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
                        0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 0x0010};

regs_description_t const regs_description_user[7]={
    { (u8*)&mdb_addr, "modbus address","mdb_addr", U16_REGS_FLAG, 1, GUID_USER_IS_03_MD|32},/*!<"modbus address"*/
    { (u8*)&interface_ip[0], "device ip address, warning!!! ","ip", U8_REGS_FLAG, 4,  GUID_USER_IS_04_MD|1},/*!<"device ip address, warning!!! "*/
    { (u8*)&mdb_holding_regs[0], "modbus holding regs","mdb_holding_regs", U16_REGS_FLAG, 16, GUID_USER_IS_03_MD|0},/*!<"modbus holding regs"*/
    { (u8*)&digital_input_0, "modbus digital input","digital_input", U16_REGS_FLAG, 1, GUID_USER_IS_02_MD|0},/*!<"modbus digital input"*/
    { (u8*)&digital_input_1, "modbus digital input","digital_input", U8_REGS_FLAG, 1, GUID_USER_IS_02_MD|16},/*!<"modbus digital input"*/
    { (u8*)&digital_input_2, "modbus digital input","digital_input", U8_REGS_FLAG, 1, GUID_USER_IS_02_MD|24},/*!<"modbus digital input"*/
    { (u8*)&digital_output, "modbus digital output","digital_output", U16_REGS_FLAG, 1, GUID_USER_IS_01_MD|0}/*!<"modbus digital output"*/
};
u8 regs_size_in_byte(regs_flag_t type){
    u8 result = 0;
    switch(type){
    case U8_REGS_FLAG:
    case I8_REGS_FLAG:
        result = 1;
        break;
    case U16_REGS_FLAG:
    case S16_REGS_FLAG:
        result = 2;
        break;
    case U32_REGS_FLAG:
    case S32_REGS_FLAG:
    case INT_REGS_FLAG:
    case TIME_REGS_FLAG:
    case FLOAT_REGS_FLAG:
        result = 4;
        break;
    case U64_REGS_FLAG:
    case S64_REGS_FLAG:
    case DOUBLE_REGS_FLAG:
        result = 8;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

int regs_get(void *reg_address, regs_access_t *reg) {
   int result = 0;
   reg->value.op_u64 = 0;
   memcpy(&reg->value.op_u64, reg_address, regs_size_in_byte(reg->flag));
   return result;
}
int regs_set(void *reg_address, regs_access_t *reg){
  int result = 0;
  if(reg_address == NULL && reg == NULL){
    result = -ILLEGAL_DATA_ADDRESS;
  }else{
    if(reg->value.op_u16 > 1000){
      result = -ILLEGAL_DATA_VALUE;
      return result;
    }
    memcpy(reg_address, &reg->value.op_u64, regs_size_in_byte(reg->flag));
  }
  return result;
}
int regs_set_buffer(void *reg_address, u8* buffer_from, u16 byte_num){
  int result = 0;
  if(reg_address == NULL && buffer_from == NULL ){
    result = -ILLEGAL_DATA_ADDRESS;
  }else if(byte_num == 0 || (byte_num & 1)){
    result = -ILLEGAL_DATA_VALUE;
  }else{
    u16 i = 0;
    for(i = 0; i < byte_num; i += 2){
      u16 data = buffer_from[i] | (buffer_from[i + 1] << 8);
      if(data > 1000){
        result = -ILLEGAL_DATA_VALUE;
        break;
      }
      reg_address = (u8*)reg_address + i;
      memcpy(reg_address, &data, 2);
    }
  }
  return result;
}
int regs_get_buffer(void * reg_address,u8* buffer_to,u16 byte_num){
  int result = -1;
  if(reg_address != NULL && buffer_to != NULL && byte_num != 0){
    memcpy(buffer_to, reg_address, byte_num);
    result = 0;
  }
  return result;
}

void regs_description_init(void){
}
u16 regs_description_get_pointer_by_modbus(u16 modbus_address, u8 modbus_function, u8 **p_value){
  u16 max_byte_num = 0;
  u16 i = 0;
  *p_value = NULL;
  for (i = 0; i < sizeof(regs_description_user)/sizeof(regs_description_user[0]); i++){
      u8 reg_mdb_function = RD_MDB_FUNCTION(regs_description_user[i].guid);
      u16 reg_mdb_address = RD_MDB_ADDRESS(regs_description_user[i].guid);
      u16 reg_size_in_word = (regs_size_in_byte((regs_flag_t)regs_description_user[i].type)*regs_description_user[i].size)/2;
      if ((modbus_function == reg_mdb_function) &&
          ((modbus_address == reg_mdb_address)||((modbus_address > reg_mdb_address) &&
                    (modbus_address < (reg_mdb_address + reg_size_in_word))))){
          if (modbus_address == reg_mdb_address){
              *p_value = regs_description_user[i].p_value;
          }else {
              *p_value = regs_description_user[i].p_value + (modbus_address - reg_mdb_address) * 2;
          }
          max_byte_num = (reg_size_in_word - (modbus_address - reg_mdb_address)) * 2;
          break;
      }
  }
  return max_byte_num;
}
u16 regs_description_get_pointer_by_modbus_discrete(u16 modbus_address, u8 modbus_function, u16 *bit_offset, u8 **p_value){
  int max_coils_number = 0;
  u16 i = 0;
  for (i = 0; i < sizeof(regs_description_user)/sizeof(regs_description_user[0]); i++){
      u8 reg_mdb_function = RD_MDB_FUNCTION(regs_description_user[i].guid);
      u16 reg_mdb_address = RD_MDB_ADDRESS(regs_description_user[i].guid);
      u16 reg_size_in_coils = (regs_size_in_byte((regs_flag_t)regs_description_user[i].type)*regs_description_user[i].size)*8;
      if ((modbus_function == reg_mdb_function) &&
          ((modbus_address == reg_mdb_address)||((modbus_address > reg_mdb_address) &&
                    (modbus_address < (reg_mdb_address + reg_size_in_coils))))){
          *p_value = regs_description_user[i].p_value;
          if (bit_offset != NULL){
              *bit_offset = modbus_address - reg_mdb_address;
              max_coils_number = reg_size_in_coils - *bit_offset;
          }
          break;
      }
  }
  return max_coils_number;
}
