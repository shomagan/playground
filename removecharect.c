#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    signed int x1,y1,x2,y2;
    x1 = x2 = y1 = y2 =0;
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char c,tab;
        tab =0x20;
        c = 0x2c;
        char removing_char[64];
        char rest_char[1024];
        unsigned int i,number_remove_char,number_rest_char;
        number_remove_char =number_rest_char=0;
        int n = strlen(line);

        for (i=(n-1);i;i--){
          if (line[i]==c){
            break;
          }else if(line[i]!=tab){
            removing_char[number_remove_char] = line[i];
            number_remove_char++;
          }
        }

        for (int j = 0;j<i;j++){
          int l;
          for (l=0;l<number_remove_char;l++){
            if (line[j]==removing_char[l]){
              break;
            }
          }
          if (l>=number_remove_char){
            rest_char[number_rest_char] = line[j];
            number_rest_char++;
          }
        }
//        printf("%s\n",line);
        for (int n=0;n<number_rest_char;n++)
          printf("%c",rest_char[n]);
        printf("\n",rest_char);
    }
    return 0;
}
  