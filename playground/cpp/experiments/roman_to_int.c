#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
int translate(char s);
int romanToInt(char* s) ;

char* test_array = "III";
int main(int argc, const char * argv[]) {
   int result = romanToInt(test_array);
   printf("result - %i %s",result,test_array);
   return 0;
}

int romanToInt(char* s) {
    int size =0;
    while(s[size]!= 0){
        size++;
    }
    int result =0;
    int preview=0;
    printf("s- %s",test_array);

    for (int i = size-1;i>=0;i--){
        int val = translate(s[i]);
        printf("%i %c",val,s[i]);
        if(val<preview){
            result -=val;
            
        }else{
            result +=val;

        }
        preview =val; 
    }
    return result;
}
int translate(char s){
    int result=0; 
    switch(s){
        case('I'):
            result= 1;
        break;
        case('V'):
            result= 5;
        break;
        case('X'):
            result= 10;
        break;
        case('L'):
            result= 50;
        break;
        case('C'):
            result= 100;
        break;
        case('D'):
            result= 500;
        break;
        case('M'):
            result= 1000;
        break;
    }
    return result;
}                  