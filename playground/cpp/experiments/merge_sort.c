#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>   
static int merge_sort(int* test_array, int test_array_size);
int array_to_sort[] = {3,1,5,0,13,10999,98779,-33,88,99,1000};
int main(int argc, const char * argv[]) {
   int array_size = sizeof(array_to_sort)/sizeof(int);
   for (int i =0;i < array_size;i++){
      printf("%i,"array_to_sort[i]);
   }
   printf("\n");

   merge_sort(array_to_sort, array_size);
   for (int i =0;i < array_size;i++){
      printf("%i,"array_to_sort[i]);
   }
   return 0;
}

static int merge_sort(int* test_array, int test_array_size){
    int result = 0;
    for (int i= 0;i<){
        if (test_array[i + 1] < test_array[i])  
    }
    return result;
}
