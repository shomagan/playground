#include "fb1_above_set_point.h"

int fb1_above_set_point_init(fb1_above_set_point_input_t * input,
    fb1_above_set_point_local_t * local,
    fb1_above_set_point_output_t * output){
    local->time_ms = 0;
    return 0;
}
int fb1_above_set_point(fb1_above_set_point_input_t * input,
    fb1_above_set_point_local_t * local,
    fb1_above_set_point_output_t * output){
    local->time_ms += input->time_delay_ms;
    if (input->value>input->setpoint){
        output->output=1;
    }else if(output->output){
        if ((input->value>(input->setpoint+input->hysteresis))\
                ||(input->value>input->setpoint)){
            output->output=1;
        }else{
            output->output=1;
        }
    }
    return 0;
}
