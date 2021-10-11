#include <stdio.h>
#include <math.h>

int main(int argc, const char * argv[]) {
   // Do something with the line
    float a = 25.0;
    float b = 50.25;
    float c = 75.6789;
    float d = 100.0;
    unsigned short e;
    e = (unsigned short)(a/0.002f);
    printf("value - %u",e);
    e = (unsigned short)(b/0.002f);
    printf("value - %u",e);
    e = (unsigned short)(c/0.002f);
    printf("value - %u",e);
    e = (unsigned short)(d/0.002f);
    printf("value - %u",e);

    return 0;
}