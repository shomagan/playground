#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define bit(i) (1<<i)
int  find_combinations_number(int * nums,int left,int right);
int numberOfSubarrays(int* nums, int numsSize, int k);

#define MIN_VALUE -10000000
#define MAX_VALUE 10000000
int min(int x, int y) { return (x<y)? x :y; }
int max(int x, int y) { return (x>y)? x :y; }


int nums[] = {2,2,2,1,2,2,1,2,2,2};
int main(int argc, const char * argv[]) {
   int numsSize = sizeof(nums)/sizeof(int); 
   int counter =  numberOfSubarrays(nums, numsSize, 2);

   printf("result - %i ",counter);
   return 0;
}



int numberOfSubarrays(int* nums, int numsSize, int k){
    int counter=0;
    int prefix=0;
    printf("array\n",counter);

    for(int l=0;l<numsSize;l++){
       printf("%i,",nums[l]);
    }
    printf("\n",counter);
    printf("\n",counter);

    for(int i = 0;i<numsSize;i++){
        nums[i] = nums[i]&bit(0)?1:0;
    }
    printf("modified\n",counter);

    for(int l=0;l<numsSize;l++){
       printf("%i,",nums[l]);
    }
    printf("\n",counter);
    printf("\n",counter);

    int left = 0;
    int right = 0;
    int sum = nums[right]; 
    while(left<numsSize && right<numsSize){
        if(sum == k){
            if(((right+1)==numsSize) || ((nums[right+1]+sum)!=k)){
                printf("combination\n",counter);

                for(int l=left;l<=right;l++){
                   printf("%i,",nums[l]);
                }
                printf("\n",counter);
                printf("\n",counter);

                counter += find_combinations_number(nums,left,right);               
            }
            right++;
            if (right<numsSize) sum +=nums[right]; 
            printf("right count : %i\n",sum);

        }else if(sum<k){

            right++;
            if (right<numsSize) sum +=nums[right]; 
            printf("right count : %i\n",sum);

        }else{
            if (left<numsSize) sum -=nums[left]; 

            left++;
            printf("left count : %i\n",sum);

        }
    }
    return counter;
}
int  find_combinations_number(int * nums,int left,int right){
    int left_side = 1;
    for(int i =left;i<=right;i++){
        if(nums[i]==0){
           left_side++;
        }else{
           break;
        }
    }
    int right_side = 1;
    for(int i =right;i>=left;i--){
        if(nums[i]==0){
           right_side++;
        }else{
           break;
        }
    }

    return left_side*right_side;
}                  