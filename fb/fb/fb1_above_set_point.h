#include "type_def.h"
#define ABOVE_SET_POINT_LOCAL_SIZE 8

typedef struct{
    double value;   /*current value*/
    double setpoint;    /*setpoint*/
    double hysteresis;  /*hysteresis*/
    u32 time_delay_ms;  /*time from a preview call*/
}fb1_above_set_point_input_t;

typedef union{
    struct{
        u64 time_ms;
    }vars;
    u8 array[ABOVE_SET_POINT_LOCAL_SIZE];
}fb1_above_set_point_local_t;

typedef struct{
    u32 output;
}fb1_above_set_point_output_t;
typedef struct{
    fb1_above_set_point_input_t input;
    fb1_above_set_point_local_t local;
    fb1_above_set_point_output_t output;
}fb1_above_set_point_regs_t;
/**
 * @brief above_set_init - set default value if neccessery
 * @param above_set_point_regs
 * @return 0 if all rigth
 */
int fb1_above_set_init(fb1_above_set_point_regs_t * regs);
/**
 * @brief above_set_point cyclic calling function
 * @param above_set_point_regs
 * @return
 */
int fb1_above_set_point(fb1_above_set_point_regs_t * regs);

