#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
char* addBinary(char* a, char* b) {
    int a_s = 0;
    while(a[a_s]!=0){
        a_s++;
    }
    int b_s = 0;
    while(b[b_s]!=0){
        b_s++;
    }
    char * c;
    int size,calc;
    if(a_s>b_s){
        size = a_s;
    }else{
        size = b_s;
    }
    c = malloc(size+1);
    c[size] = 0;
    calc = size;
    char temp=0;
    while(a_s>0 && b_s>0){
        a_s--;
        b_s--;
        calc--;
        temp = (a[a_s]+b[b_s]+(temp>>1))&0x03;
        printf(" %i %i %i %i ",a[a_s],b[b_s],a[a_s]+b[b_s],temp);
        if(temp&0x01){
            c[calc] = '1';
        }else{
            c[calc] = '0';
        }
    }
    printf("c - %s, size - %i %i %i \n",c,size,a_s,b_s);

    for(int i = calc-1;i>=0;i--){
        temp = (c[i]+(temp>>1))&0x03;
        if(temp&0x01){
            c[i] = '1';
        }else{
            c[i] = '0';
        }
    }
    printf("c2 - %s, size - %i \n",c,size);

    if(temp&0x02){
        char * d = malloc(size+2);
        memcpy(&d[1],c,size+1);
        free(c);
        c = d;
        c[0]='1';
    }
    printf("c3 - %s, size - %i \n",c,size);

    return c;
}
char * a = "11";
char * b = "1";
int main(int argc, const char * argv[]) {
   char * result = addBinary(a,b);
   printf("result - %s ",result);
   return 0;
}
                  