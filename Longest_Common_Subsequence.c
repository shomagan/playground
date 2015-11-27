#include <stdio.h>
#include <string.h>
#include <math.h>
  unsigned char point;
  char a_char[50];
  char common_sequence_char[50];
  char max_common_sequence_char[50];
  char b_char[50];
  char b_start_symbol;
  unsigned char point_a,point_b;
  unsigned char i,j,k,number_b_char,number_a_char,number_b_char_reduce,number_a_char_reduce,common_char_number,max_common_char_number;

static void find(unsigned char a_first_symbol,unsigned char b_first_symbol){
  unsigned char l,m;
  if (a_first_symbol==number_a_char_reduce){
    
//    if (common_char_number>max_common_char_number){
      max_common_char_number=common_char_number;
      unsigned char n;
      for (n = 0;n<(common_char_number);n++){
        max_common_sequence_char[n] = common_sequence_char[n];
        printf("%c",common_sequence_char[n]);
      }
        printf("\n");
  //  }

  }
  for (l=a_first_symbol;l<number_a_char_reduce;l++){
    for (m=b_first_symbol;m<number_b_char_reduce;m++){
      if (a_char[l]==b_char[m]){

        common_sequence_char[common_char_number] = b_char[m];
        common_char_number++;
        find((l+1),(m+1));
        common_char_number--;
      }
    }
  }
}

int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char c;
//        c = 0x2c;// ","
        c = 0x3b;// ";"
        b_start_symbol = 0;
        number_b_char=number_a_char=number_b_char_reduce=number_a_char_reduce=0;
        max_common_char_number = 0;
        int line_len = strlen(line);
        if (line_len>1){
            int len;
//                printf(" %i %i \n",line[n-1],line[n]);          
//--------------------first-----------------------
            for (i=0;i<(line_len);i++){ //find right string(i>0)
              if (line[i]==c){
                b_start_symbol = i+1;
              }else{
                if ((line[i]==0x20||line[i]==0x0a||line[i]==0x00)){
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
            }
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
/*            printf("%s\n",line);
            for (j = 0;j<(number_a_char_reduce);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char_reduce);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");*/

        find(0,0);
        unsigned char n;
        for (n = 0;n<(max_common_char_number);n++){
          printf("%c",max_common_sequence_char[n]);
        }
          printf("\n");

        }
//            printf("0\n");
    }
    return 0;
}
//            printf("number_a_char_reduce,number_b_char_reduce,%i%i",number_a_char_reduce,number_b_char_reduce);
/*            printf("%s\n",line);
            for (j = 0;j<(number_a_char_reduce);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char_reduce);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");*/

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
  