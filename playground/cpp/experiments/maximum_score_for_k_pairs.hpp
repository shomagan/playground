#ifndef MAXIMUM_SCORE_FOR_K_PAIRS_HPP
#define MAXIMUM_SCORE_FOR_K_PAIRS_HPP
#include <cstdint>
#include <vector>
#include <algorithm>
#include <math.h>
#include "common.hpp"
class MaximumScoreForKPairs {
public:
   MaximumScoreForKPairs(){};
   ~MaximumScoreForKPairs(){};
   uint64_t maxScore(std::vector<int>& nums1, std::vector<int>& nums2, int k) {
      uint64_t score = 0;
      std::sort(nums1.begin(), nums1.end(), std::greater<int>());
      std::sort(nums2.begin(), nums2.end(), std::greater<int>());
      /* firtst find  max from positive values*/
      for (;k;k--){
         if(((nums1[0]>0) && (nums2[0]>0)) && ((nums1[0]*nums2[0]) >= (nums1.back()*nums2.back()))){
            score += nums1[0]*nums2[0];
            nums1.erase(nums1.begin());
            nums2.erase(nums2.begin());
         /*find both negative max*/
         }else if ((((nums1.back()<0) && (nums2.back()<0)) && ((nums1[0]*nums2[0]) <= (nums1.back()*nums2.back())))){
            score += nums1.back()*nums2.back();
            nums1.pop_back();
            nums2.pop_back();
         }else{
            break;
         }
      }
      /*if something left , check for zero and eliminate biggest from another side
      goal is to have abs min value now*/
      std::transform(nums1.begin(), nums1.end(), nums1.begin(),[](int x) { return std::abs(x); });
      std::transform(nums2.begin(), nums2.end(), nums2.begin(),[](int x) { return std::abs(x); });
      std::sort(nums1.begin(), nums1.end());
      std::sort(nums2.begin(), nums2.end());

      for (;k; k--){
         if(nums1[0] == 0){
            nums1.erase(nums1.begin());
            nums2.pop_back();
         }else if(nums2[0] == 0){
            nums2.erase(nums2.begin());
            nums1.pop_back();
         }else{
            score -= nums1[0] * nums2[0];
            nums1.erase(nums1.begin());
            nums2.erase(nums2.begin());
         }
      }

      return score;
   }
};
#endif // MAXIMUM_SCORE_FOR_K_PAIRS_HPP
