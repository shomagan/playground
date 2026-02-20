#ifndef SORTING_HPP
#define SORTING_HPP
#include <vector>
#include <algorithm>
class Sorting {
public:
   Sorting(){};
   ~Sorting(){};
   void flag_sort(std::vector<int>& nums){
      /*nums contains only 0, 1, 2
      idea add to first and last buffer by "swapping"
      until it not crossed */
      int low = 0;
      int mid = 0;
      int high = nums.size()-1;
      while (mid <= high && mid < nums.size() && high >= 0) {
         if(nums[mid] == 0){
            nums[mid] = nums[low];
            nums[low] = 0;
            low++;
            mid++;
         }else if(nums[mid]==1){
            mid++;
         }else {
            nums[mid] = nums[high];
            nums[high] = 2;
            high--;
         }
      }
   }
   int arrayPairSum(std::vector<int>& nums) {
        /*we have 2*n pairs - 1,2,3,4,5,6
        need to combine it with max sum of min(1,2) etc
        can try to combine max values together*/
        std::sort(nums.begin(), nums.end());
        int sum = 0;
        for(int i = 0; i < nums.size(); i = i + 2){
            sum += std::min(nums[i],nums[i+1]);
        }
        return sum;
   }
   void counting_sort(std::vector<int>& nums) {
      int max_value = 0;
      for(int i: nums){
         if (i > max_value){
            max_value = i;
         }
      }
      std::vector<int> out(nums.size(), 0);
      std::vector<int> summary(max_value + 1, 0);
      for(int i: nums){
         summary[i] += 1;
      }
      for (int i = 1;i < max_value+1;i++){
         summary[i] += summary[i-1];
      }
      for (int i: nums){
         summary[i]--;
         out[summary[i]] = i;
      }
      nums = out;
   }
   int hIndex(std::vector<int>& citations) {
      std::sort(citations.begin(), citations.end());
      int result = 0;
      for(int i = citations.size() - 1; i>=0;i--){
         if(citations[i] > (citations.size() - i - 1)){
            result = i;
         }else{
            break;
         }
      }
      return result;
   }
};
#endif // SORTING_HPP
