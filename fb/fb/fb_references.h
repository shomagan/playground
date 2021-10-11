/**
 * @file fb_references.h
 * @author Shoma Gane <shomagan@gmail.com>
 *         
 * @defgroup fb
 * @ingroup fb
 * @version 0.1 
 * @brief  TODO!!! write brief in 
 */
#ifndef FB_REFERENCES_H
#define FB_REFERENCES_H 1
 
/*add includes below */
#include "type_def.h"
#include "fb1_above_set_point.h"
#include "fb2_below_set_point.h"
#include "fb101_add.h"

/*add includes before */
#ifdef __cplusplus 
   extern "C" {
#endif
/*add functions and variable declarations below */


typedef struct{
   u32 number;
   char * name;
   size_t input_size;
   size_t input_regs;
   u32 * input_flags;
   size_t local_size;
   size_t local_regs;
   u32 * local_flags;
   size_t output_size;
   size_t output_regs;
   u32 * output_flags;
   int (*fb_init_func)(void * input,void * local,void * output);
   int (*fb_func)(void * input,void * local,void * output);
}fb_cals_t;
#define  FB_NUMBER  3
extern const fb_cals_t fb_cals[];
/*add functions and variable declarations before */
#ifdef __cplusplus
}
#endif
#endif //FB_REFERENCES_H
