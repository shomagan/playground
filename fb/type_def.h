#ifndef TYPE_DEF_H
#define TYPE_DEF_H
#include "stdint.h"
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

#define MCU_PACK __attribute__((packed))
#define delay_until vTaskDelayUntil
typedef signed long long int s64;
typedef signed long int s32;
typedef signed short s16;
typedef signed char s8;
typedef signed char i8;

typedef volatile signed long long int vs64;
typedef volatile signed long int vs32;
typedef volatile signed short vs16;
typedef volatile signed char vs8;
typedef volatile signed char vi8;

typedef uint64_t u64;
typedef unsigned long int u32;
typedef unsigned int ui32;//for get rid of warning messege
typedef unsigned short u16;
typedef unsigned char u8;

typedef const unsigned long long int uc64;
typedef const unsigned long int uc32;
typedef const unsigned short uc16;
typedef const unsigned char uc8;

typedef volatile unsigned long int vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char vu8;

typedef volatile unsigned long int const vuc32;
typedef volatile unsigned short const vuc16;
typedef volatile unsigned char const vuc8;
#ifndef size_t
#define size_t u32
#endif
/**
  * @brief sofi register types
  * @ingroup regs
  */
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
} reg_value_t;

/**
  * @brief sofi register type flag
  * @ingroup regs
  */
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
}reg_flag_t;
typedef struct{
    reg_value_t reg_value;
    reg_flag_t reg_flag;
}reg_t;
#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#define MCU_ROOT_CODE __attribute__((section(".priv_code")))
#define MCU_PRIV_CODE MCU_ROOT_CODE
#define SOFI_LEN_CODE __attribute__ ((section (".sofi_len")))
#define SOFI_LINK_CODE __attribute__ ((section (".sofi_link"))) 
#define BKSRAM_CODE __attribute__ ((section (".bksram"))) 
#define TASK_CODE __attribute__ ((section (".task_text")))
#define INLINE __attribute__((always_inline)) __STATIC_INLINE


#define asm __asm


#define MCU_NAKED __attribute__((naked))
#define MCU_ALIGN(x) __attribute__((aligned (x)))
#define MCU_ALWAYS_INLINE __attribute__((always_inline))
#define MCU_NEVER_INLINE __attribute__((noinline))
#define FNCT_NO_RETURN __attribute__((noreturn))


#endif// TYPE_DEF_H
