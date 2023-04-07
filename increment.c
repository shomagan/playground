#include <stdio.h>
#include <string.h>
#include <math.h>
static volatile unsigned int timeErrCounter = 1;
static volatile unsigned int timeErrCounterLast = 0;
static volatile unsigned int timeErrCounterStore = 0;
static volatile unsigned int timeErrCounterStoreMaxInTheRow = 0;

int main(int argc, const char * argv[]) {
    timeErrCounterStore = timeErrCounter > timeErrCounterLast?timeErrCounterStore+1:timeErrCounterStore;
    printf("value: %i\n",timeErrCounterStore);
    timeErrCounterStore = timeErrCounter > timeErrCounterLast?timeErrCounterStore++:timeErrCounterStore;
    printf("value: %i\n",timeErrCounterStore);
    return 0;
}
  