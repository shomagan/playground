#include <stdio.h>
#include <string.h>
#include <math.h>
typedef unsigned short uint16;
typedef unsigned long uint32;
static volatile uint32 where_to_delay_c0_hiprio = 0;
static volatile uint16 test_number_of_delayed_task_c0_hiprio = 0;
static volatile uint32 test_delay_counts_c0_hiprio = 0xffffff; 
static volatile float test_a_c0_hiprio,test_b_c0_hiprio,test_c_c0_hiprio;
static volatile uint16 test_number_of_delayed_task_mask_c0_hiprio = 0x0;
static void delay_functionc0_hiprio(void);
static void delay_functionc0_hiprio(void){
  uint32 test1;
  uint16 summ;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  printf("init: %lu %lu %u %u\n",(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  test_number_of_delayed_task_mask_c0_hiprio = 0x01;
  test_number_of_delayed_task_c0_hiprio = 0x01;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  printf("0x01 0x01: %lu %lu %u %u\n",(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  test_number_of_delayed_task_mask_c0_hiprio = 0x0c;
  test_number_of_delayed_task_c0_hiprio = 0x01;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  printf("0x0c 0x01: %lu %lu %u %u\n",(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  test_number_of_delayed_task_mask_c0_hiprio = 0x03;
  test_number_of_delayed_task_c0_hiprio = 0x01;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  printf("0x0c 0x01: %lu %lu %u %u\n",(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  test_number_of_delayed_task_mask_c0_hiprio = 0xc0;
  test_number_of_delayed_task_c0_hiprio = 0xc0;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  printf("0xc0 0xc0: %lu %lu %u %u\n",(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  test_number_of_delayed_task_mask_c0_hiprio = 0xff;
  test_number_of_delayed_task_c0_hiprio = 20;
  test1 = (~test_number_of_delayed_task_mask_c0_hiprio);
  summ = (~test_number_of_delayed_task_mask_c0_hiprio) & (uint32)test_number_of_delayed_task_c0_hiprio;
  for (;test_number_of_delayed_task_c0_hiprio;test_number_of_delayed_task_c0_hiprio--){
     printf("%u: %lu %lu %u %u\n",test_number_of_delayed_task_c0_hiprio,(~test_number_of_delayed_task_mask_c0_hiprio), test1,summ,(((~test_number_of_delayed_task_mask_c0_hiprio) & test_number_of_delayed_task_c0_hiprio) == test_number_of_delayed_task_c0_hiprio));
  }
}

int main(int argc, const char * argv[]) {
    delay_functionc0_hiprio();
    return 0;
}
  