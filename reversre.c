#include <stdio.h>
#include <math.h>
unsigned char reverse_bit_in_byte(unsigned char byte){
  byte = ((byte&0xf0)>>4)|((byte&0x0f)<<4);
  byte = ((byte&0xcc)>>2)|((byte&0x33)<<2);
  byte = ((byte&0xaa)>>1)|((byte&0x55)<<1);
  return byte;
}

int main(int argc, const char * argv[]) {
   // Do something with the line
    
    for (unsigned char i=0;i<255;i++){
      printf("%#02x - %#02x \n",i,reverse_bit_in_byte(i));
    }
 //       printf(distance);
    return 0;
}