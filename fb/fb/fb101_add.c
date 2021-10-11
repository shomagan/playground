/**
 * @file fb101_add.c
 * @author Shoma Gane <shomagan@gmail.com>
 *         
 * @defgroup fb
 * @ingroup fb
 * @version 0.1 
 * @brief  TODO!!! write brief in 
 */
#ifndef FB101_ADD_C
#define FB101_ADD_C 1
#include "fb101_add.h"


int fb101_add_init(fb101_add_input_t * input,
    fb101_add_local_t * local,
    fb101_add_output_t * output){
    int res = 0;
    output->summ = 0;
    return res;
}
int fb101_add(fb101_add_input_t * input,
    fb101_add_local_t * local,
    fb101_add_output_t * output){
    int res = 0;
    output->summ = input->a + input->b;
    return res;
}
#endif //FB101_ADD_C
