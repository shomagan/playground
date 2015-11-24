#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char c;
        c = 0x2c;
        char b_char[64];
        unsigned int i,number_b_char;
        number_b_char=0;
        int n = strlen(line);
        if (n>1){
            int len;
//                printf(" %i %i \n",line[n-1],line[n]);          
            for (i=n-1;i;i--){
              if (line[i]==c){
                break;
              }else{
                if ((number_b_char==0) && (line[i]==0x20||line[i]==0x0a||line[i]==0x00)){
                }else{
                  b_char[number_b_char] = line[i];
//                printf("%i%c\n",number_b_char,b_char[number_b_char]);
                  number_b_char++;
                }
              }
            }
            int j,k = 0;
            for (j = (i-1);j&&number_b_char;j--){
              if (line[j]!=b_char[k]){
//                  printf("%i%c\n",j,b_char[k]);
                  break;
              }else{
                number_b_char--;
                k++;
              }
            }
            if (j&&number_b_char)
                printf("0\n");
            else
                printf("1\n");
        }else
            printf("0\n");
    }
    return 0;
}
  