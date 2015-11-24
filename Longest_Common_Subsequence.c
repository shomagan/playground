#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char c;
//        c = 0x2c;// ","
        c = 0x3b;// ";"

        char a_char[50];
        char b_char[50];
        char b_start_symbol;
        b_start_symbol = 0;
        unsigned char i,j,k,number_b_char,number_a_char,number_b_char_reduce,number_a_char_reduce;
        number_b_char=number_a_char=number_b_char_reduce=number_a_char_reduce=0;
        int line_len = strlen(line);
        if (line_len>1){
            int len;
//                printf(" %i %i \n",line[n-1],line[n]);          
//--------------------first-----------------------
            for (i=0;i<(line_len);i++){ //find right string(i>0)
              if (line[i]==c){
                b_start_symbol = i+1;
              }else{
                if (b_start_symbol){
                  b_char[number_b_char] = line[i];
                  number_b_char++;
                }else{
                  a_char[number_a_char] = line[i];
                  number_a_char++;
                }
              }
            }
            // write symbol if we have in both string
//            printf("number_a_char,number_b_char,%i  %i",number_a_char,number_b_char);
/*            for (j = 0;j<(number_a_char);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");
*/
            for (j=0;j<number_a_char;j++){
              for (k=0;k<number_b_char;k++){
                if (line[j]==b_char[k]){
                  break;
                }
              }
              if (k<number_b_char){
                a_char[number_a_char_reduce] = line[j];
                number_a_char_reduce++;
              }
            }
            // write symbol if we have in both string
            for (j=0;j<number_b_char;j++){
              for (k=0;k<number_a_char;k++){
                if (line[j+b_start_symbol]==a_char[k]){
                  break;
                }
              }
              if (k<number_a_char){
                b_char[number_b_char_reduce] = line[j+b_start_symbol];
                number_b_char_reduce++;
              }
            }

//--------------------first_end-----------------------
//            printf("number_a_char_reduce,number_b_char_reduce,%i%i",number_a_char_reduce,number_b_char_reduce);
            printf("%s\n",line);
            for (j = 0;j<(number_a_char_reduce);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char_reduce);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");

        }else
            printf("0\n");
    }
    return 0;
}
  