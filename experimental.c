#include <stdio.h>
#include <string.h>
#include <math.h>
typedef volatile unsigned int vuint32;
static volatile unsigned int timeErrCounter_1 = 12;
static volatile unsigned int timeErrCounter_2 = 13;

int main(int argc, const char * argv[]) {
    vuint32 DmaForDnvClkMonErrorData = timeErrCounter_1 = timeErrCounter_2;
    printf("value: %i %i %i\n",DmaForDnvClkMonErrorData,timeErrCounter_1,timeErrCounter_2);
    return 0;
}
  