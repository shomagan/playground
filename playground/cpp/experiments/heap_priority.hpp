#ifndef HEAP_PRIORITY_HPP
#define HEAP_PRIORITY_HPP
#include <vector>
class HeapPriority {
public:   HeapPriority(){};
   ~HeapPriority(){};

   int findKthLargest_heap(std::vector<int>& nums, int k) {
      std::make_heap(nums.begin(), nums.end());
      for (int i = 0; i < k - 1; ++i) {
         std::pop_heap(nums.begin(), nums.end() - i);
      }
      return nums[0];
   }
   int findKthLargest(std::vector<int>& nums, int k) {
      for (int i=0;i<nums.size();i++){
         int index = 0;
         int equal = 0;
         for (int j=0;j<nums.size();j++){
            if(i==j){
               continue;
            }
            if(nums[i]<nums[j]){
               index++;
            }
            if(nums[i]==nums[j]){
               equal++;
            }
         }
         if((index==(k-1)) || ((index<(k-1)) && ((index+equal)>=(k-1)))){
            return nums[i];
         }
      }
      return nums[0];
   }
   int findKthLargest_move(std::vector<int>& nums, int k) {
      bool revert = 0;
      if(k>nums.size()/2+1){
         k = nums.size()-k+1;
         revert = 1;
      }
      if(revert){
         for (int i=0;i< k;i++){
            int extr = 100001;
            int extr_index = i;
            for (int j=i;j<nums.size();j++){
               if(nums[j]<extr){
                  extr = nums[j];
                  extr_index = j;
               }
            }
            int temp = nums[i];
            nums[i] = nums[extr_index];
            nums[extr_index] = temp;
         }
      }else{
         for (int i=0;i< k;i++){
            int extr = -100001;
            int extr_index = i;
            for (int j=i;j<nums.size();j++){
               if(nums[j]>extr){
                  extr = nums[j];
                  extr_index = j;
               }
            }
            int temp = nums[i];
            nums[i] = nums[extr_index];
            nums[extr_index] = temp;
         }
      }
      return nums[k-1];
   }
   void pop_heap_own(std::vector<int>& nums, int n){
      /*only root is missplaced */
      int parent = 0;
      int child = parent*2+1;
      while(child < n){
         /*move only to biggest child*/
         if(((child+1)<n) && (nums[child+1]>nums[child])){
            child++;
         }
         if(nums[parent] < nums[child]){
            int temp = nums[parent];
            nums[parent] = nums[child];
            nums[child] = temp;
            parent = child;
            child = parent*2+1;
         }else{
            /*if not than already sorted*/
            break;
         }
      }
   }
   void make_heap_own(std::vector<int>& nums, int n){
      /*start with last non-leaf node and go up(to first index)
      child leaf is 2i+1 and 2i+2
      parent is (i-1)/2,parent for n is (n-1)/2 */
      for (int i = (n-1)/2;i>=0;i--){
         /*now we move down large numbers, every time to down, deep down*/
         int parent = i;
         int child = parent*2+1;
         while(child < n){
            /*move only to biggest child*/
            if(((child+1)<n) && (nums[child+1]>nums[child])){
               child++;
            }
            if(nums[parent] < nums[child]){
               int temp = nums[parent];
               nums[parent] = nums[child];
               nums[child] = temp;
               parent = child;
               child = parent*2+1;
            }else{
               /*if not than already sorted*/
               break;
            }
         }
      }
   }
   int findKthLargest_own_heap(std::vector<int>& nums, int k) {
      make_heap_own(nums, nums.size());
      for (int i = 0; i < k - 1; ++i) {
         std::swap(nums[0], nums[nums.size() - 1 - i]);
         pop_heap_own(nums, nums.size() - 1 - i);
      }
      return nums[0];
   }
};
#endif
