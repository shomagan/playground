#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    int a =10;
    int b =11;

    int * const p = &a; 
    printf("value: %i\n",*p);
    *p =11;
    printf("value: %i\n",*p);
    p =&b;
    return 0;
}
  