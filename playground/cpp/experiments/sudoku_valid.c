#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

char board[9][9] = {{'5','3','.','.','7','.','.','.','.'},{'6','.','.','1','9','5','.','.','.'},{'.','9','8','.','.','.','.','6','.'},{'8','.','.','.','6','.','.','.','3'},{'4','.','.','8','.','3','.','.','1'},{'7','.','.','.','2','.','.','.','6'},{'.','6','.','.','.','.','2','8','.'},{'.','.','.','4','1','9','.','.','5'},{'.','.','.','.','8','.','.','7','9'}};
int main(int argc, const char * argv[]) {
    int result = 1;
    //row
    for (int i = 0;i<9;i++){
        for(int j = 0;j<9;j++){
            if(board[i][j]!= '.'){
               for(int k = j+1;k<9;k++){
                  if(board[i][j]==board[i][k] ){
                    result = 0;
                  }
               }
            } 
        }
    }
//column
    for (int i = 0;i<9;i++){
        for(int j = 0;j<9;j++){
            if(board[j][i]!= '.'){
               for(int k = j+1;k<9;k++){
                  if(board[j][i]==board[k][i]){
                    result = 0;
                  }
               }
            } 
        }
    }
    //quadrant
    for (int r = 0;r<3;r++){
        for(int c = 0;c<3;c++){
            for(int j = 0;j<9;j++){
                if(board[(j/3)+3*r][(j%3)+3*c]!= '.'){
                    for(int k = j+1;k<9;k++){
                        if(board[(k/3)+3*r][(k%3)+3*c]==board[(j/3)+3*r][(j%3)+3*c] ){
                            result = 0;
                        }
                    }
                } 
            }
        }
    }
    printf("result - %i",result);
}

                  