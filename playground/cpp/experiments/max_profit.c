#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static int max_profit(int* array, int array_size);
int test_array[] = {7,1,5,3,6,4};
int test_value = -8;
int main(int argc, const char * argv[]) {
   int result = max_profit(test_array, sizeof(test_array)/sizeof(int));
   printf("result - %i ",result);
   return 0;
}

static int max_profit(int* array, int array_size) {
   int result = 0;
   int * best_profit_array = calloc(sizeof(int),array_size);
   best_profit_array[0] = 0;
   for(int i=1; i<array_size; i++){
      best_profit_array[i] = best_profit_array[i-1];
      if (array[i]>array[i-1]){
         for (int j = 0; j < i; j++){
            int temp_profit = best_profit_array[j]+(array[i]-array[j]);
            if (temp_profit > best_profit_array[i]){
               best_profit_array[i] = temp_profit; 
            }
         } 
      }
   }
   result = best_profit_array[array_size-1];
   free(best_profit_array);
   return result;
}
                  