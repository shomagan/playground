#include <stdio.h>
#include <string.h>
#include <math.h>
int minimumTotal(int  triangle[][2], int triangleSize, int* triangleColSize) ;

int  array_to_search_1[2][2] = {{-1,0},{-2,-3}};

int  arrays_size[] = {1,2};

int minimumTotal(int triangle[][2], int triangleSize, int* triangleColSize) {
    int min_path=0x0fffffff;
    for(int i = 0;i<triangleSize;i++){
        for(int j=0;j<triangleColSize[i];j++){
            if(i<triangleSize-1){//not a last row
                if(j==0 && i==0){
                    triangle[i+1][j] = triangle[i][j]+triangle[i+1][j];
                    triangle[i+1][j+1] = triangle[i][j]+triangle[i+1][j+1];
                }else if(j==triangleColSize[i]-1){
                    triangle[i+1][j+1] = triangle[i][j]+triangle[i+1][j+1];
                    printf("last %i, %i",triangle[i][j],triangle[i+1][j+1]);
                }else{
                    if(j==0){
                        triangle[i+1][j] = triangle[i][j]+triangle[i+1][j];
                        printf("first %i, %i",triangle[i][j],triangle[i+1][j+1]);
                    }
                    if(triangle[i][j]<triangle[i][j+1]){
                        triangle[i+1][j+1] = triangle[i][j]+triangle[i+1][j+1];
                        printf("triangle[i][j] %i,triangle[i+1][j+1] %i\n",triangle[i][j],triangle[i+1][j+1]);
                    }else{
                        triangle[i+1][j+1] = triangle[i][j+1]+triangle[i+1][j+1];
                        printf("triangle[i][j+1] %i,triangle[i+1][j+1] %i\n",triangle[i][j+1],triangle[i+1][j+1]);
                    }
                }
            }else{
                if(min_path>triangle[i][j]){
                    min_path = triangle[i][j];
                }
                printf("triangle[i][j] %i",triangle[i][j]);
            }
        }
    }
    return min_path;
}
int main(int argc, const char * argv[]) {
   int result = minimumTotal(array_to_search_1, 2, arrays_size);
   printf("result - %i",result);

   return 0;
}

