#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

double medium_arrays(int * nums1,int nums1Size,int *nums2,int nums2Size);
#define MIN_VALUE -10000000
#define MAX_VALUE 10000000
int min(int x, int y) { return (x<y)? x :y; }
int max(int x, int y) { return (x>y)? x :y; }


int nums1[] = {1,2};
int nums2[] = {3,4};
//merged = 1,2,12,13,24, 24,33,34,46,47
int main(int argc, const char * argv[]) {
   int nums1Size = sizeof(nums1)/sizeof(int); 
   int nums2Size = sizeof(nums2)/sizeof(int); 
   double medium = medium_arrays(nums1,nums1Size,nums2,nums2Size);
   printf("result - %f ",medium);
   return 0;
}

double medium_arrays(int * nums1,int nums1Size,int *nums2,int nums2Size){
   double result = 0.0;
   int * small_array;
   int * big_array;
   int small_array_size,big_array_size;
   
   if(nums1Size < nums2Size){
      small_array = nums1;
      big_array  = nums2;
      small_array_size = nums1Size;
      big_array_size = nums2Size;
   }else{
      small_array = nums2;
      big_array  = nums1;
      small_array_size = nums2Size;
      big_array_size = nums1Size;
   }
   int left = 0;
   int right = small_array_size;
   while (left<=right){
      int position_small = (left+right)/2;
      int position_big = (big_array_size+small_array_size+1)/2-position_small;
      int max_small = position_small==0? MIN_VALUE:small_array[position_small-1];
      int max_big = position_big==0? MIN_VALUE:big_array[position_big-1];
      int min_small = position_small>=small_array_size? MAX_VALUE:small_array[position_small];
      int min_big = position_big>=big_array_size? MAX_VALUE:big_array[position_big];
      printf("max_small : %i, max_big : %i, min_small : %i,min_big : %i",max_small, max_big, min_small,min_big);
      if(max_big <= min_small && max_small <= min_big ){
         if((small_array_size+big_array_size)%2==0){
            result = (double)(max(max_small,max_big)+min(min_small,min_big))/2;
         }else{
            result = max(max_small,max_big);
         }
         break;
      }else if(max_small > min_big){
         right = position_small;
      }else{
         left = position_small+1;
      }
   }
   
   return result;
}
                  