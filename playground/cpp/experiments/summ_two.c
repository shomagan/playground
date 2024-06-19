#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static int * summ_two(int* array, int array_size, int value);
int test_array[] = {-1,-2,-3,-4,-5};
int test_value = -8;
int main(int argc, const char * argv[]) {
   int * result_array = summ_two(test_array, sizeof(test_array)/sizeof(int), test_value);
   printf("values - %i, %i ",result_array[0],result_array[1]);
   return 0;
}

static int * summ_two(int* array, int array_size, int value) {
    int a,b;
    int * result = NULL;
    for(int i=0;i<array_size;i++){
        a = array[i];
        b = value - array[i];
        for(int j = i+1; j < array_size;j++){
            if(b == array[j]){
                result = malloc(sizeof(int)*2);
                result[0] = i;
                result[1] = j;
                return result;
            }
        }
    }
    return result;
}
