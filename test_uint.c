#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    unsigned int a,b,c;
    int d = 50;
    a = 0xfffffffe;
    b = 100;
    c = b-a;
    printf("substraction value  %u\n",c);              
    if((b-a)>50){
        printf("substraction much then 50\n");
    }
    if((b-a)>d){
        printf("substraction much then %i\n",d);
    }

    return 0;
}
  