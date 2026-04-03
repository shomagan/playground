#ifndef BINARY_SEARCH_ARRAY_HPP
#define BINARY_SEARCH_ARRAY_HPP
#include <vector>
#include <algorithm>
#include <string>
class BinarySearchArray {
public:
   BinarySearchArray(){};
   ~BinarySearchArray(){};
   /*Koko loves to eat bananas. There are n piles of bananas, the ith pile has piles[i] bananas.
     The guards have gone and will come back in h hours. Koko can decide her bananas-per-hour eating speed of k.
     Each hour, she chooses some pile of bananas and eats k bananas from that pile. If the pile has less than k bananas,
     she eats all of them instead and will not eat any more bananas during this hour.
     Koko likes to eat slowly but still wants to finish eating all the bananas before the guards return.
     qReturn the minimum integer k such that she can eat all the bananas within h hours.*/
   int minEatingSpeed(std::vector<int>& piles, int h) {
      int result =0;
      if(piles.size()){
         int max = *std::max_element(piles.begin(), piles.end());
         long long calc_hours = 0;
         int left = 1;
         int right = max;
         int temp_speed = (right+left)/2;
         result = temp_speed;
         while(left <= right) {
            calc_hours = 0;
            for(int pile:piles){
               calc_hours += pile/temp_speed;
               if(pile%temp_speed){
                  calc_hours++;
               }
            }
            if(calc_hours <= h){
               right = temp_speed - 1;
               result = temp_speed;
            }else{
               left = temp_speed + 1;
            }
            temp_speed = (right+left)/2;
         }
      }
      return result;
   }
   bool is_enough_flowers(std::vector<int>& bloomDay, int m, int k, int day){
      int in_a_row = 0;
      for(int i =0;i < bloomDay.size();i++){
         if(bloomDay[i]<= day){
            in_a_row++;
         }else{
            in_a_row=0;
         }
         if(in_a_row>=k){
            in_a_row=0;
            m--;
            if(m<=0){
               return true;
            }
         }
      }
      return false;
   }
   /*bloomday
   m bouquet, k adjacent flowers*/
   int minDays(std::vector<int>& bloomDay, int m, int k) {
      size_t number = (size_t)m*k;
      if(number > bloomDay.size()){
         return -1;
      }
      std::vector <int> bloomDaySorted = bloomDay;
      std::sort(bloomDaySorted.begin(), bloomDaySorted.end());
      int right = bloomDaySorted.size()-1;
      int left = 0;
      int current = (right+left)/2;
      int result = bloomDaySorted[right];
      while (left<= right){
         int day = bloomDaySorted[current];
         if(is_enough_flowers(bloomDay, m, k, day)){
            result = day;
            right = current-1;
         }else{
            left = current + 1;
         }
         current = (right+left)/2;
      }
      return result;
   }
   int maxArea(std::vector<int>& height) {
      /*need to find max min of pair on maximum distance between */
      int area = 0;
      if(height.size()<2){
         return area;
      }else{
         /*first two pair */
         int a = 0;
         int b = height.size()-1;
         area = (b-a) * std::min(height[a],height[b]);
         /*idea once we take wider from 0 to last ,we can improve situation only to make it higher
         then we should keep taller side and move another one */
         for(int i = 2; i < height.size(); i++){
            if(height[a] > height[b]){
               b--;
            }else{
               a++;
            }
            int new_area = (b-a) * std::min(height[a],height[b]);
            if(new_area > area){
               area = new_area;
            }
         }
         return area;
      }
   }
   // brute force correct answer
   int maxArea_brute(std::vector<int> height) {
      int area = 0;
      for(int i = 0; i<height.size(); i++)
         for(int j = i+1; j<height.size(); j++)
               area = std::max(area, (j-i)*std::min(height[i],height[j]));
      return area;
   }
};
#endif // BINARY_SEARCH_ARRAY_HPP
