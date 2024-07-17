
/* Iterative C program for merge sort */
#include<stdlib.h>
#include<stdio.h>
 


int main(int argc, const char * argv[]) {
    unsigned int test_value = 0x10000008;
    printf("0x%08x\n",test_value);
    test_value = test_value >> 16 | test_value << 16;
    test_value = (test_value >> 8 & 0x00ff00ff)  | (test_value << 8 & 0xff00ff00);
    test_value = (test_value >> 4 & 0x0f0f0f0f) | (test_value << 4 & 0xf0f0f0f0);
    test_value = (test_value >> 2 & 0x33333333) | (test_value << 2 & 0xcccccccc);
    test_value = (test_value >> 1 & 0x55555555) | (test_value << 1 & 0xaaaaaaaa);


 
    printf("0x%08x\n",test_value);

    return 0;
}
