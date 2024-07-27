#include <stdio.h>       
#include <string.h>
#include <math.h>
#include <stdlib.h>
void int_to_position(int index,  int * result,int boardSize){
    result[0] =(boardSize-1) - (index-1)/boardSize; 
    if(((index-1)/boardSize)&0x01){
        result[1] = (boardSize-1) - (index-1) % boardSize; 
    }else{
        result[1] = (index-1) % boardSize;
    }
}

int snakesAndLadders(int** board, int boardSize, int* boardColSize) {
    int items = boardSize*boardSize;
    int linear_array[items];
    for(int i = 0;i<items;i++){
        linear_array[i] = -1;
    }
    int moves = 0;
    linear_array[0] = 0;
    int added= 1;
    while(added){
        added = 0;
        for(int i =1;i<items;i++){
            //printf("linear_array[i] %i,%i\n",i,linear_array[i-1]);
            if(linear_array[i-1]==moves){
                for (int j=1;j<7;j++){
                    int temp_index = i + j;
                    int position[2];
                    int_to_position(temp_index, position, boardSize);
                    //printf("position %i,%i %i\n",position[0],position[1],temp_index);
                    if(board[position[0]][position[1]]!=-1){
                        printf("short cut to %i from %i\n",board[position[0]][position[1]],temp_index);
                        temp_index = board[position[0]][position[1]];
                    }
                    if(temp_index == items){
                        return moves + 1;
                    }
                    if(linear_array[temp_index-1] ==-1){
                        added = 1;
                        linear_array[temp_index-1] = moves + 1;
                    }
                }
            }
        }
        moves++;
    }
    return -1;
}
