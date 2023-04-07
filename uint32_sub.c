#include <stdio.h>
#include <string.h>
#include <math.h>
static volatile unsigned int a = 0;
static volatile unsigned int b = 0xffffffff;
static volatile unsigned int c = 0;
int main(int argc, const char * argv[]) {
    c = a - b;
    printf("value: %lu\n",c);
    return 0;
}
  