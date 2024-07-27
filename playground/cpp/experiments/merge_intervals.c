#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>

static void merge_arrays(int** arr, int l, int m, int r);
static void mergeSort(int **arr, int n);
int min(int x, int y) { return (x<y)? x :y; }

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
   int ** return_array = calloc(sizeof(int*),intervalsSize);
   *returnColumnSizes = calloc(sizeof(int*),intervalsSize); 
   *intervalsColSize = 2; 
   int merge_index = -1;
   int return_size = 0;


   mergeSort(intervals, intervalsSize);
   if(intervalsSize){
      merge_index = 0;
      return_array[merge_index] = malloc(sizeof(int)*2);
      return_array[merge_index][0] = intervals[0][0];
      return_array[merge_index][1] = intervals[0][1];
      returnColumnSizes[0][merge_index] = 2;
   }
   for (int i = 0; i<intervalsSize-1; i++){
      if(intervals[i+1][0]<=return_array[merge_index][1]){
         if ( intervals[i+1][1]>return_array[merge_index][1]){
            return_array[merge_index][1] =intervals[i+1][1];
         }
      }else{
         merge_index++;

         returnColumnSizes[0][merge_index] = 2;
         return_array[merge_index] = malloc(sizeof(int)*2);
         return_array[merge_index][0] = intervals[i+1][0];
         return_array[merge_index][1] = intervals[i+1][1];

      }
   }
   *returnSize = merge_index+1;
   return return_array;

}
static void mergeSort(int **arr, int n){
   int curr_size;  
   int left_start; 
   for (curr_size=1; curr_size<n; curr_size = 2*curr_size){
       for (left_start=0; left_start<n-1; left_start += 2*curr_size) {
           int mid = min(left_start + curr_size - 1, n-1);
           int right_end = min(left_start + 2*curr_size - 1, n-1);
           merge_arrays(arr, left_start, mid, right_end);
       }
   }
}
 
static void merge_arrays(int** arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int L[n1+1][2], R[n2+1][2];
    for (i = 0; i < n1; i++){
        L[i][0] = arr[l + i][0];
        L[i][1] = arr[l + i][1];
    }
    for (j = 0; j < n2; j++){
        R[j][0] = arr[m + 1+ j][0];
        R[j][1] = arr[m + 1+ j][1];
    }
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2){
        if (L[i][0] <= R[j][0]){
            arr[k][0] = L[i][0];
            arr[k][1] = L[i][1];

            i++;
        }else{
            arr[k][0] = R[j][0];
            arr[k][1] = R[j][1];
            j++;
        }
        k++;
    }
    while (i < n1){
        arr[k][0] = L[i][0];
        arr[k][1] = L[i][1];
        i++;
        k++;
    }
    while (j < n2){
        arr[k][0] = R[j][0];
        arr[k][1] = R[j][1];
        j++;
        k++;
    }
}
