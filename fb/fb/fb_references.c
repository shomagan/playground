/**
 * @file fb_references.c
 * @author Shoma Gane <shomagan@gmail.com>
 *         
 * @defgroup fb
 * @ingroup fb
 * @version 0.1 
 * @brief  TODO!!! write brief in 
 */
#ifndef FB_REFERENCES_C
#define FB_REFERENCES_C 1
#include "fb_references.h"
static u32 fb101_add_input_flags[] = {DOUBLE_REGS_FLAG,DOUBLE_REGS_FLAG};
#define fb101_add_reg_number (sizeof(fb101_add_input_flags)/sizeof(u32))
static u32 fb101_add_local_flags[] = {DOUBLE_REGS_FLAG};
static u32 fb101_add_output_flags[] = {DOUBLE_REGS_FLAG};
static u32 fb2_below_set_point_input_flags[] = {DOUBLE_REGS_FLAG,DOUBLE_REGS_FLAG,DOUBLE_REGS_FLAG,U32_REGS_FLAG};
static u32 fb2_below_set_point_local_flags[] = {U64_REGS_FLAG};
static u32 fb2_below_set_point_output_flags[] = {U32_REGS_FLAG};
static u32 fb1_above_set_point_input_flags[] = {DOUBLE_REGS_FLAG,DOUBLE_REGS_FLAG,DOUBLE_REGS_FLAG,U32_REGS_FLAG};
static u32 fb1_above_set_point_local_flags[] = {U64_REGS_FLAG};
static u32 fb1_above_set_point_output_flags[] = {U32_REGS_FLAG};

const fb_cals_t fb_cals[]={
   {101,"add",   sizeof(fb101_add_input_t),fb101_add_reg_number,fb101_add_input_flags,
   sizeof(fb101_add_local_t),sizeof(fb101_add_local_flags)/sizeof(u32),fb101_add_local_flags,
   sizeof(fb101_add_output_t),sizeof(fb101_add_output_flags)/sizeof(u32),fb101_add_output_flags,
   fb101_add_init, fb101_add},
   {1,"above_set_point",   sizeof(fb1_above_set_point_input_t),sizeof(fb1_above_set_point_input_flags)/sizeof(u32),fb1_above_set_point_input_flags,
   sizeof(fb1_above_set_point_local_t),sizeof(fb1_above_set_point_local_flags)/sizeof(u32),fb1_above_set_point_local_flags,
   sizeof(fb1_above_set_point_output_t),sizeof(fb1_above_set_point_output_flags)/sizeof(u32),fb1_above_set_point_output_flags,
   fb1_above_set_point_init, fb1_above_set_point},
   {2,"below_set_point",   sizeof(fb2_below_set_point_input_t),sizeof(fb2_below_set_point_input_flags)/sizeof(u32),fb2_below_set_point_input_flags,
   sizeof(fb2_below_set_point_local_t),sizeof(fb2_below_set_point_local_flags)/sizeof(u32),fb2_below_set_point_local_flags,
   sizeof(fb2_below_set_point_output_t),sizeof(fb2_below_set_point_output_flags)/sizeof(u32),fb2_below_set_point_output_flags,
   fb2_below_set_point_init, fb2_below_set_point},
};
const size_t FB_NUMBER = (sizeof(fb_cals)/sizeof(fb_cals_t));
#endif //FB_REFERENCES_C
