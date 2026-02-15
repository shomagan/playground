#ifndef FLAG_SORT_HPP
#define FLAG_SORT_HPP
#include <vector>
class FlagSort {
public:
   FlagSort(){};
   ~FlagSort(){};
   void flag_sort(std::vector<int>& nums){
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
};
#endif // FLAG_SORT_HPP
