#ifndef MAX_SUM_FOR_K_SEQ_PAIR_HPP
#define MAX_SUM_FOR_K_SEQ_PAIR_HPP
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

/*You are given two integer arrays nums1 and nums2 of lengths n and m respectively, and an integer k.

You must choose exactly k pairs of indices (i1, j1), (i2, j2), ..., (ik, jk) such that:

   0 <= i1 < i2 < ... < ik < n
   0 <= j1 < j2 < ... < jk < m

For each chosen pair (i, j), you gain a score of nums1[i] * nums2[j].

The total score is the sum of the products of all selected pairs.

Return an integer representing the maximum achievable total score.*/
class DP {
public:
   DP(){};
   ~DP(){};

   int maxScore(std::vector<int>& nums1, std::vector<int>& nums2, int k){
      int n = nums1.size();
      int m = nums2.size();
      std::vector<std::vector<std::vector<long long>>> scores(n + 1,
         std::vector<std::vector<long long>>(m + 1,
            std::vector<long long>(k + 1, -1000000ll*1000000ll*100ll)));

      for(int i = 0; i <= n; ++i) {
         for(int j = 0; j <= m; ++j) {
            scores[i][j][0] = 0;
         }
      }

      for(int i = 1; i <= n; ++i) {
         for(int j = 1; j <= m; ++j) {
            for(int c = 0; c <= k; ++c) {
               scores[i][j][c] = std::max(scores[i][j][c], scores[i-1][j][c]);
               scores[i][j][c] = std::max(scores[i][j][c], scores[i][j-1][c]);
               if(c != 0) {
                  long long product = (long long)nums1[i-1] * (long long)nums2[j-1];
                  scores[i][j][c] = std::max(scores[i][j][c], scores[i-1][j-1][c-1] + product);
               }
            }
         }
      }
      return scores[n][m][k];
   }
   void get_down(int n, int &ways_number){
      if(n>=2){
         ways_number = ways_number+1;
         int n2 = n-2;
         get_down(n2, ways_number);
      }
      if(n>=1){
         int n1 = n-1;
         get_down(n1, ways_number);
      }
   }
   /*return number of ways to reach the top*/
   int climb_stairs(int n) {
      if(n<=1){
         return n;
      }
      int ways_number = 1;
      get_down(n, ways_number);
      return ways_number;
   }
   int climb_stairs_dp(int n) {
      int* ways_number = new int[n+1];
      if(n<=2){
         return n;
      }
      ways_number[1] = 1;
      ways_number[2] = 2;
      std::cout << ways_number[1] << " " << ways_number[2]<< "\n";
      for (int i = 3; i <= n; i++){
         ways_number[i] = ways_number[i-1] + ways_number[i-2];
         std::cout << ways_number[i] <<"\n";
      }
      return ways_number[n];
   }
   int minCostClimbingStairs(std::vector<int>& cost) {
      uint64_t steps = cost.size();
      std::vector<int> cost_sum(steps);
      if(steps <= 1){
         return 0;
      }else{
         cost_sum[0] = cost[0];
         cost_sum[1] = cost[1];
         for(int i = 2; i < steps; i++){
            cost_sum[i] = (cost_sum[i-2]+cost[i])<(cost_sum[i-1]+cost[i])?(cost_sum[i-2]+cost[i]):(cost_sum[i-1]+cost[i]);
         }
         return cost_sum[steps-1]<cost_sum[steps-2]?cost_sum[steps-1]:cost_sum[steps-2];
      }
   }
   /*house robber(adjacent houses cannot be robbed)
     You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed,
     the only constraint stopping you from robbing each of them is that adjacent houses have security systems connected and
     it will automatically contact the police if two adjacent houses were broken into on the same night.
     Given an integer array nums representing the amount of money of each house, return the maximum amount of money you can
     rob tonight without alerting the police.
   */
   int rob(std::vector<int>& nums) {
      int size = nums.size();
      if(size ==0){
         return 0;
      }else if(size ==1){
         return nums[0];
      }else {
         std::vector<int> maxes;
         maxes.push_back(nums[0]);
         maxes.push_back(nums[0]>nums[1]?nums[0]:nums[1]);
         for(int i = 2; i < size; i++){
            if((nums[i]+maxes[i-2])>maxes[i-1]){
               maxes.push_back(nums[i]+maxes[i-2]);
            }else{
               maxes.push_back(maxes[i-1]);
            }
         }
         return maxes[size-1];
      }
   }
   int rob_2(std::vector<int>& nums) {
      int size = nums.size();
      if(size ==0){
         return 0;
      }else if(size ==1){
         return nums[0];
      }else if (size == 2){
         return nums[0]>nums[1]?nums[0]:nums[1];
      }else if (size == 3){
         int temp = 0;
         temp = nums[0]>nums[1]?nums[0]:nums[1];
         temp = temp>nums[2]?temp:nums[2];
         return temp;
      }else {
         std::vector<int> maxes;
         maxes.push_back(nums[0]);
         maxes.push_back(nums[0]>nums[1]?nums[0]:nums[1]);
         for(int i = 2; i < size-1; i++){
            if((nums[i]+maxes[i-2])>maxes[i-1]){
               maxes.push_back(nums[i]+maxes[i-2]);
            }else{
               maxes.push_back(maxes[i-1]);
            }
         }
         maxes.push_back(0);
         int first = maxes[size-2];
         maxes[0] = 0;
         maxes[1] = nums[1];
         maxes[2] = (nums[1]>nums[2]?nums[1]:nums[2]);
         for(int i = 3; i < size; i++){
            if((nums[i]+maxes[i-2])>maxes[i-1]){
               maxes[i] = nums[i]+maxes[i-2];
            }else{
               maxes[i] = maxes[i-1];
            }
         }
         int second = maxes[size-1];
         return first>second?first:second;
      }
   }
   /*You have intercepted a secret message encoded as a string of numbers. The message is decoded via the following mapping:
     'A' -> 1
     'B' -> 2
     ...
     'Z' -> 26
     Given a string s containing only digits, return the number of ways to decode it.
   */
   int numDecodings(std::string s) {
      uint64_t size = s.size();
      std::vector<int> combination_number(size);
      if(s[0]=='0'){
         return 0;
      }else if(size==1){
         return 1;
      }
      combination_number[0] = 1;
      int added = 0;
      for(int i = 1; i < size; i++){
         if(s[i] == '0' && (s[i-1]>'2' || s[i-1]=='0')){
            return 0;
         }
         if(s[i]=='0' && added){
            if(i > 2){
               combination_number[i] = combination_number[i-1] - combination_number[i-3];
            }else{
               combination_number[i] = combination_number[i-1]-1;
            }
         }else if((s[i]!='0')&&(s[i-1]=='1' || (s[i-1]=='2' && s[i]<='6'))){
            if(i > 1){
               combination_number[i] = combination_number[i-1] + combination_number[i-2];
            }else{
               combination_number[i] = combination_number[i-1] + 1;
            }
            added = 1;
         }else{
            combination_number[i] = combination_number[i-1];
            added = 0;
         }
      }
      return combination_number[size-1];
   }

};
#endif // MAX_SUM_FOR_K_SEQ_PAIR_HPP
