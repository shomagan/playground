#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>   

int main(int argc, const char * argv[]) {
   unsigned char a = 0;
   unsigned char b = 0xff;
   unsigned char c = a - b;
   std::cout << +c;
   return 0;
}
