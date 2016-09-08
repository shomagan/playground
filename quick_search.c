#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
typedef  unsigned char uint8_t ;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t ;
int array_separate(int array[],int size,int spawn){
  if (size == 1){
    return spawn;
  }else{
    spawn++;
    char f = 1;
    char j = 0;
    char f_count,r_count;
    f_count = 0;
    r_count = 1;
    for (int i =1;i<size;i++){
      if(f){
        if(array[j]>array[size-1-f_count]){
          int temp;
          temp = array[size-1-f_count];
          array[size-1-f_count] = array[j];
          array[j]=temp;
          j=size-1-f_count;
          f=0;
        }
        f_count++;
      }else{
        if(array[j]<array[r_count]){
          int temp;
          temp = array[r_count];
          array[r_count] = array[j];
          array[j]=temp;
          j=r_count;
          f=1;
        }
        r_count++;
      }
    }
    if(j>0){
/*      printf("l\n");
      for (char i = 0;i<j;i++){
        printf("%02x;",array[i]);
      }*/
      spawn = array_separate(&array[0],j,spawn);

    }
    if(j<(size-1)){
/*      printf("h\n");
      for (char i = 0;i<(size-j-1);i++){
        printf("%02x;",array[j+1+i]);
      }*/
      spawn = array_separate(&array[j+1],size-j-1,spawn);
//      printf("spawn%02x\n",spawn);
    }
  }
  return spawn;
}

int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    int list[50];
    char line[1024];
    char list_len;
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char point,tab;
        unsigned char j;
        unsigned int i;
        list_len = 0;
        point = 0x2e;//"."
        tab = 0x20;  //" "
        int line_len = strlen(line);
        if (line_len>1){
          int len;
          list[list_len++] = strtol(&line[0], NULL, 16);
          for (i=0;list_len<=50 && i<line_len;i++){
            if (line[i]==tab){
              list[list_len++] = strtol(&line[i+1], NULL, 16);
            }
          }
          int spawn;
          spawn=0;
/*          for (j = 0;j<list_len;j++){
            printf("%02x;",list[j]);
          }
          printf("\n");*/
          spawn = array_separate(list,list_len,spawn);
/*          for (j = 0;j<list_len;j++){
            printf("%02x;",list[j]);
          }*/
          printf("%i\n",spawn);
//          printf("\n");
        }
    }
    return 0;
}
             