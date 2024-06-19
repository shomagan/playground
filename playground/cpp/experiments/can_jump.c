#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static int can_jump(int* array, int array_size);
int test_array[] = {3,2,1,0,4};
int main(int argc, const char * argv[]) {
   int result = can_jump(test_array, sizeof(test_array)/sizeof(int));
   printf("result - %i ",result);
   return 0;
}

static int can_jump(int* nums, int numsSize) {
   int result = 0;
   int jumper = 0;
   int max_index = 0;
   for(int i=0; (i <= max_index)&&(i < numsSize);i++){
      if ((nums[i]+i) > max_index){
         max_index = nums[i]+i;
      }
   }
   if (max_index >= numsSize-1){
      result = 1;
   }
   return result;
}
                  