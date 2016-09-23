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
    char f = 1;//forward search
    char j = 0;
    char f_count,r_count;
    f_count = 0;
    r_count = 1;
    for (int i =1;i<size;i++){
      if(f){//forward search
        if(array[j]>array[size-1-f_count]){//from right value before give less then self
          int temp;
          temp = array[size-1-f_count];
          array[size-1-f_count] = array[j];
          array[j]=temp;
          j=size-1-f_count;
          f=0;
        }
        f_count++;
      }else{//from left 
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
      spawn = array_separate(&array[0],j,spawn);

    }
    if(j<(size-1)){
      spawn = array_separate(&array[j+1],size-j-1,spawn);
    }
  }
  return spawn;
}

int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    uint8_t line[1336];
    uint16_t line_len;
    while (fgets(line, 1336, file)) {
        // Do something with the line
        char point,tab;
        uint16_t j,i;
        uint8_t* p_labirint;
        uint8_t side_width;
        wall = 0x2a;//"*" wall
        floor_s = 0x20;  //" " floor
        start = 0x3b; 
        line_len = strlen(line);
        if (line_len>1){
          side_width = strtol(&line[0], NULL, 10);  //get squer side widht
          p_labirint = malloc(side_width*side_width*side_width*sizeof(char));//alloce array for labirint
          for (i=0;i<line_len;i++){
            
            if (line[i]==start){
              list[list_len++] = strtol(&line[i+1], NULL, 16);
            }
          }
          int spawn;
          spawn=0;
          spawn = array_separate(list,list_len,spawn);
          printf("%i\n",spawn);
        }
    }
    return 0;
}
             