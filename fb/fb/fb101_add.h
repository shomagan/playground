/**
 * @file fb101_add.h
 * @author Shoma Gane <shomagan@gmail.com>
 *         
 * @defgroup fb
 * @ingroup fb
 * @version 0.1 
 * @brief  TODO!!! write brief in 
 */
#ifndef FB101_ADD_H
#define FB101_ADD_H 1
 
/*add includes below */
#include "type_def.h"

/*add includes before */
#ifdef __cplusplus 
   extern "C" {
#endif
/*add functions and variable declarations below */

typedef struct{
    double a;
    double b;
}fb101_add_input_t;

typedef struct{
    double unused;
}fb101_add_local_t;

typedef struct{
    double summ;
}fb101_add_output_t;
/**
 * @brief fb101_add - set default value if neccessery
 * @param regs
 * @return
 */
int fb101_add_init(    fb101_add_input_t * input,
    fb101_add_local_t * local,
    fb101_add_output_t * output
);
/**
 * @brief fb101_add cyclic calling function
 * @param regs
 * @return
 */
int fb101_add(fb101_add_input_t * input,
    fb101_add_local_t * local,
    fb101_add_output_t * output);

/*add functions and variable declarations before */
#ifdef __cplusplus
}
#endif
#endif //FB101_ADD_H
