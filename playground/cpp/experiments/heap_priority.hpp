#ifndef HEAP_PRIORITY_HPP
#define HEAP_PRIORITY_HPP
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <string>
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
   int nthUglyNumber(int n) {
      /*2 3 5*/
      vector<int> ugly(n);
      ugly[0] = 1;
      int i2 = 0;
      int i3 = 0;
      int i5 = 0;
      for(int i =1;i<n;i++){
         ugly[i] = std::min({ugly[i2]*2,ugly[i3]*3,ugly[i5]*5});
         if(ugly[i] == ugly[i2]*2){
            i2++;
         }else if(ugly[i] == ugly[i3]*3){
            i3++;
         }else if(ugly[i] == ugly[i5]*5){
            i5++;
         }
      }
      return ugly[n-1];
   }
   vector<int> topKFrequent(vector<int>& nums, int k) {
      /*what is the problem - it is not in order, and if we calculated number of repeat values we need to sort
         we can use counting and can sort
         or we can sort and then count and keep only k pairs*/
      std::unordered_map<int, int> map;
      for(int i: nums){
         map[i]++;
      }
      std::vector<std::pair<int, int>> pairs;
      for(auto & [value, count]: map){
         pairs.push_back({value, count});
      }
      std::sort(pairs.begin(), pairs.end(),
                [](const auto& a, const auto& b){
                     return a.second > b.second;
                });
      std::vector<int> result;
      for(int i = 0;i<k;i++){
         result.push_back(pairs[i].first);
      }
      return result;
   }
   vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
      std::vector<std::vector<int>> result(k);
      std::vector<int> indexes(nums1.size(), 0);
      int current_index = 0;
      for(int i = 0; i < k; i++){
         result[i] = {nums1[current_index], nums2[indexes[current_index]]};
         indexes[current_index]++;
         if(indexes[current_index]>=nums2.size()){
            //std::cout << "current_index" << current_index << "\n";
            current_index++;
         }
         /*what to chose */
         /*first compare with all preview indexes*/
         if(current_index >= nums1.size()){
            break;
         }
         int min_sum = nums1[current_index] + nums2[indexes[current_index]];
         //std::cout << "min_sum 0 " << min_sum << "\n";

         /*now compare only with one next(because all next will be bigger) if exist */
         for(int j = current_index + 1; j <= nums1.size() - 1 ; j++){
            //std::cout << "index+ sum" << nums1[current_index + 1] + nums2[indexes[current_index + 1]] << "\n";
            if((nums1[j] + nums2[indexes[j]]) < min_sum){
               min_sum = nums1[j] + nums2[indexes[j]];
               current_index = j;
            }
            if(indexes[j] == 0){
               break;
            }
         }


         //std::cout << "min_sum 1 " << min_sum << "\n";

         for(int j = current_index-1; j >=0 ; j--){
            if(indexes[j] >= nums2.size()){
               break;
            }
            if((nums1[j] + nums2[indexes[j]])<=min_sum){
               min_sum = nums1[j] + nums2[indexes[j]];
               current_index = j;
            }
         }
         //std::cout << "min_sum 2 " << min_sum << " a " << current_index << " b " << indexes[current_index]<< "\n";

      }
      return result;
   }
};
#endif
