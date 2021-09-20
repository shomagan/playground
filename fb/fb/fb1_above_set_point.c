#include "above_set_point.h"
int fb1_above_set_init(fb1_above_set_point_regs_t * regs){
    regs->local.vars.time_ms = 0;
    return 0;
}
int fb1_above_set_point(fb1_above_set_point_regs_t * regs){
    regs->local.vars.time_ms += regs->input.time_delay_ms;
    if (regs->input.value>regs->input.setpoint){
        regs->output.output=1;
    }else if(regs->output.output){
        if ((regs->input.value>(regs->input.setpoint+regs->input.hysteresis))\
                ||(regs->input.value>regs->input.setpoint)){
            regs->output.output=1;
        }else{
            regs->output.output=1;
        }
    }
    return 0;
}
