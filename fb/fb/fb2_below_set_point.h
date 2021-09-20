#include "type_def.h"
#define BELOW_SET_POINT_LOCAL_SIZE 8

typedef struct{
    double value;   /*current value*/
    double setpoint;    /*setpoint*/
    double hysteresis;  /*hysteresis*/
    u32 time_delay_ms;  /*time from a preview call*/
}fb2_below_set_point_input_t;

typedef union{
    struct{
        u64 time_ms;
    }vars;
    u8 array[BELOW_SET_POINT_LOCAL_SIZE];
}fb2_below_set_point_local_t;

typedef struct{
    u32 output;
}fb2_below_set_point_output_t;
typedef struct{
    fb2_below_set_point_input_t input;
    fb2_below_set_point_local_t local;
    fb2_below_set_point_output_t output;
}fb2_below_set_point_regs_t;

/**
 * @brief below_set_init - set default value if neccessery
 * @param regs
 * @return
 */
int fb2_below_set_init(fb2_below_set_point_regs_t * regs);
/**
 * @brief below_set_point cyclic calling function
 * @param regs
 * @return
 */
int fb2_below_set_point(fb2_below_set_point_regs_t * regs);

