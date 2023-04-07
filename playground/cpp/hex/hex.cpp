#include <stdio.h>
#include <string.h>
#include <math.h>
/*class get values form argv and 
   print out hex values in decimal format  */
class hex {
   /*unsigned uint64 value*/
    long long int dec_value;
    char input_string[48];
public:
      hex(int argc, const char * argv[]);
      void print();
   };
hex::hex(int argc, const char * argv[]) {
    if (argc == 2) {
         strcpy(input_string, argv[1]);
         /*if starts from 0x then handle as hex else as decimal*/
         if (input_string[0] == '0' && input_string[1] == 'x') {
            dec_value = strtoll(input_string, NULL, 16);
         } else {
            dec_value = strtoll(input_string, NULL, 10);
         }
    }   else {
        printf("Usage: hex <hex value> \n");
    }
}
void hex::print() {
   /*print dec_value in hex format dec_value could be bigger then long int*/
   printf("0x%llx - %lld\n", dec_value, dec_value);
   /*print dec_value byte by byte intepretation adaptive to value size*/
   for (int i = 0; ((long long int)1 << i*8) < dec_value; i++) {
      unsigned char current_byte = (dec_value >> (i*8)) & 0xff;
      printf("Byte %d: 0x%x - %u \n",i, current_byte, current_byte);
   }
}
   

int main(int argc, const char * argv[]) {
      hex hex1(argc, argv);
      hex1.print();
      return 0;
   }
