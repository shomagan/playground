#ifndef UNORDERED_SET_OR_MAP_HPP
#define UNORDERED_SET_OR_MAP_HPP
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
using namespace std;

class UnorderedSetOrMap {
public:   UnorderedSetOrMap(){};
   ~UnorderedSetOrMap(){};
   static int first_uniq_character(std::string s) {
      std::unordered_map <char, int> characters_set;
      for(char c: s){
         characters_set[c] = characters_set[c] + 1;
      }
      int index = 0;
      for(char c: s){
         if(characters_set[c] == 1){
            return index;
         }
         index++;
      }
      return -1;
   }
   static int majorityElement(std::vector<int>& nums) {
      std::unordered_map <int, int> map;
      for (int i: nums){
         map[i] = map[i] + 1;
         if(map[i] > nums.size()/2){
            return i;
         }
      }
   }
   /*Letter Combinations of a Phone Number
     Given a string containing digits from 2-9 inclusive, return all possible letter combinations that the number could represent.
     Return the answer in any order.
     A mapping of digits to letters (just like on the telephone buttons) is given below. Note that 1 does not map to any letters.
     2: "abc", 3: "def", 4: "ghi", 5: "jkl", 6: "mno", 7: "pqrs", 8: "tuv", 9: "wxyz"
     digits.length <= 4
   */
   std::vector<std::string> letterCombinations(std::string digits) {
      std::vector<std::string> result;
      std::unordered_map<char, std::string> buttons ={
         { '2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}};
      if(digits.length()){
         for(int i=0;i<buttons[digits[0]].length();i++){
            if(digits.length()>1){
               for(int j=0;j<buttons[digits[1]].length();j++){
                  if(digits.length()>2){
                     for(int k=0;k<buttons[digits[2]].length();k++){
                        if(digits.length()>3){
                           for(int l=0;l<buttons[digits[3]].length();l++){
                              result.push_back(std::string{buttons[digits[0]][i], buttons[digits[1]][j], buttons[digits[2]][k], buttons[digits[3]][l]});
                           }
                        }else{
                           result.push_back(std::string{buttons[digits[0]][i], buttons[digits[1]][j], buttons[digits[2]][k]});
                        }
                     }
                  }else{
                     result.push_back(std::string{buttons[digits[0]][i], buttons[digits[1]][j]});
                  }
               }
            }else{
               result.push_back(std::string(1, buttons[digits[0]][i]));
            }
         }
      }
      return result;
   }
   /*Group Anagrams
     Given an array of strings strs, group the together. You can return the answer in any order.
     Input: strs = ["eat","tea","tan","ate","nat","bat"]
     Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
   */
   vector<vector<string>> groupAnagrams(vector<string>& strs) {
      vector<vector<string>> result;
      unordered_map <string, int> sorted_map;
      int index = 0;
      for(const string& s: strs){
         string s_copy = s;
         sort(s_copy.begin(), s_copy.end());

         if(sorted_map.find(s_copy)==sorted_map.end()){
            /*create new*/
            sorted_map[s_copy] = index;
            vector<string> init;
            init.push_back(s);
            result.push_back(init);
            index++;
         }else{
            /*add to existed vector*/
            result[sorted_map[s_copy]].push_back(s);
         }
      }
      return result;
   }
   int vector_has_k_uniq(vector<int>& nums,int start, int end, int k){
      if((end + 1 - start)>=k){
         std::unordered_set<int> elements;
         for(int i = start; i < end + 1; i++){
            elements.insert(nums[i]);
            if(elements.size()>k){
               return 1;
            }
         }
         if(elements.size() == k){
            return 0;
         }
         return -1;
      }else{
         return -1;
      }
   }
   void count_back(vector<int>& nums, int k, int & left, int & right,int & number_good_arrays){
      /*left already moved*/
      int size = nums.size();
      while (right > left && (right -left +1)>k){
         /*anyway we need to check current array has k uniq or not */
         int res = vector_has_k_uniq(nums, left, right, k);
         if(res == 0){
            for (int i = left; i <= right ; i++){
               std::cout << nums[i] << " ";
            }
            std::cout << "\n";
            number_good_arrays++;
            right--;
         }else if(res == 1){
            right--;
         }else{
            break;
         }
      }
   }
   int subarraysWithKDistinct(vector<int>& nums, int k) {
      /*sliding window */
      int size = nums.size();
      int left = 0;
      int right = left + k - 1;
      int number_good_arrays = 0;
      while ((size - left >= k) && (right < size)){
         /*anyway we need to check current array has k uniq or not */
         int res = vector_has_k_uniq(nums, left, right, k);
         if(res == 0){
            for (int i = left; i <= right ; i++){
               std::cout << nums[i] << " ";
            }
            std::cout << "\n";

            number_good_arrays++;
            right++;
            if(right >= size){
               right--;
               left++;
               count_back(nums, k, left, right, number_good_arrays);
               left++;
            }
         }else if(res == 1){
            left++;
            count_back(nums, k, left, right, number_good_arrays);
            left++;
         }else{
            right++;
         }
      }
      return number_good_arrays;
   }
};
#endif /* UNORDERED_SET_OR_MAP_HPP */
