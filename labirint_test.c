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
#define MAX_NEIGHBORS 121
typedef struct {
  u8 layer;
  u8 x_y[2];
}id_t;

typedef struct {
  id_t id;
  u16 length;
}bind_t;


typedef struct {
  id_t id;
  bind_t bind[MAX_NEIGHBORS];
  u16 neighbors_number;
}node_t;


u32 calculate_node_number(char* p_labirint,u8 side_width){
  char const wall=0x2a,floor_s=0x20,holl=0x6f;
  u8 layer_item,layer_item_cnt;
  u16 last_item,item_on_layer;
  u32 node_number;
  u8 *on_layer;
 // wall = 0x2a;             //"*" wall
 // floor_s = 0x20;          //" " floor
 // holl = 0x6f;             //"o" holl
  node_number =0;
  layer_item_cnt = 0;
  last_item = side_width*side_width*side_width-1;
  item_on_layer = side_width*side_width;
  layer_item = side_width;
  while(layer_item){
    on_layer = &p_labirint[last_item - layer_item_cnt*item_on_layer];
    layer_item_cnt++;
    for (u16 i=0;i<item_on_layer;i++){
      u8 edge;
      u16 mod_item_number;
      edge =0;
      mod_item_number = i % side_width;
      if(((mod_item_number==0)||((mod_item_number-1)==side_width)) ||
        ((i<side_width)||(i>=(item_on_layer-side_width)))){
        edge=1;
      }
      if(*(on_layer+i)==holl){
        printf("find holl %i on layer %i /n",*(on_layer),node_number);
        node_number++;
      }else if ((*(on_layer+i)==floor_s) && edge){
        node_number++;

        printf("find floor %i on layer %i /n",*(on_layer),node_number);
      }
//      print("node number on cube %i\n",*(on_layer+i));
    }
    layer_item--;
  }
  return node_number;
}
u32 init_node(char* p_labirint,u8 side_width,id_t *node){
  char const wall=0x2a,floor_s=0x20,holl=0x6f;
  u8 layer_item,layer_item_cnt;
  u16 last_item,item_on_layer;
  u32 node_number;
  u8 *on_layer;
 // wall = 0x2a;             //"*" wall
 // floor_s = 0x20;          //" " floor
 // holl = 0x6f;             //"o" holl
  node_number =0;
  layer_item_cnt = 0;
  last_item = side_width*side_width*side_width-1;
  item_on_layer = side_width*side_width;
  layer_item = side_width;
  while(layer_item){
    on_layer = &p_labirint[last_item - layer_item_cnt*item_on_layer];
    for (u16 i=0;i<item_on_layer;i++){
      u8 edge;
      u16 mod_item_number;
      edge =0;
      mod_item_number = i % side_width;
      if(((mod_item_number==0)||((mod_item_number-1)==side_width)) ||
        ((i<side_width)||(i>=(item_on_layer-side_width)))){
        edge=1;
      }
      if(*(on_layer+i)==holl){
        node[node_number].layer = layer_item_cnt;
        node[node_number].x_y[0] = mod_item_number;
        node[node_number].x_y[1] = i / side_width;
        node_number++;
      }else if ((*(on_layer+i)==floor_s) && edge){
        node[node_number].layer = layer_item_cnt;
        node[node_number].x_y[0] = mod_item_number;
        node[node_number].x_y[1] = i / side_width;
        node_number++;
      }
//      print("node number on cube %i\n",*(on_layer+i));
    }
    layer_item_cnt++;
    layer_item--;
  }
  return node_number;
}
u8 print_node(id_t *node,u16 node_number){
  u16 i=0;
  for (i=0;i<node_number;i++){
    printf("node numm %i\n",i);
    printf("node layer %i\n",node[i].layer);
    printf("node x,y [%i;%i]\n",node[i].x_y[0],node[i].x_y[1]);
  }
  return i;
}

u8 make_graph(char* p_labirint,u8 side_width){

}
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    uint8_t line[1336];
    uint16_t line_len;
    while (fgets(line, 1336, file)) {
        // Do something with the line
        const char start = 0x3b;
        char *p_labirint;
        id_t *node;
        uint16_t j,i,node_number;//input output and hole
        uint8_t side_width;
//        start = 0x3b;            //";"
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
          node_number=calculate_node_number(p_labirint,side_width);
          node = malloc(node_number*sizeof(id_t));
//          init_node(p_labirint,side_width,node);
  //        print_node(node,node_number);
//          make_graph(p_labirint,side_width);
  /*        for(i=0;i<j;i++){
            printf("%c\n",p_labirint[i]);}*/
          printf("node number on cube %i\n",node_number);
        }
        free(p_labirint);
    }
    return 0;
}
             