#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
typedef  unsigned char uint8_t ;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t ;
typedef  unsigned char u8 ;
typedef  unsigned short u16;
typedef  unsigned int u32;
#define MAX_NEIGHBORS 11
typedef struct {
  u8 layer;
  u8 x_y[2];
}id_t;

typedef struct {
  node_t *neighbors;
  u16 length;
}bind_t;


typedef struct {
  id_t id;
  bind_t bind[MAX_NEIGHBORS];
  u16 neighbors_number;
}node_t;
make_graph(p_labirint){

}
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    uint8_t line[1336];
    uint16_t line_len;
    while (fgets(line, 1336, file)) {
        // Do something with the line
        char wall,floor_s,start;
        char *p_labirint;
        uint16_t j,i;
        uint8_t side_width;
        wall = 0x2a;//"*" wall
        floor_s = 0x20;  //" " floor
        start = 0x3b; //";"
        line_len = strlen(line);
        if (line_len>1){
          side_width = strtol(&line[0], NULL, 10);  //get squer side widht
          p_labirint = malloc(side_width*side_width*side_width*sizeof(char));//alloce array for labirint
          for (i=0;i<line_len;i++){
            if (line[i]==start){
              j=0;
              for (++i;i<line_len;i++){
                p_labirint[j++] = line[i];
              }
              break;
            }
          }
          make_graph(p_labirint);
          for(i=0;i<j;i++){
            printf("%c\n",p_labirint[i]);}
        }
    }
    return 0;
}
             