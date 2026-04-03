#ifndef UNORDERED_SET_OR_MAP_HPP
#define UNORDERED_SET_OR_MAP_HPP
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
using namespace std;
/*std::unordered_set<int> seen;
for(int num : nums) {
   if(seen.find(num) != seen.end()) {
      return true;
   }
   seen.insert(num);
}
return false;
count() - most readable for sets (returns 0 or 1)
contains() - clearest intent (C++20+)
find() != end() - traditional, works everywhere
*/
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
   int find_distinct_up_to_k(vector<int>& nums, int k){
      int size = nums.size();
      int n = 0;
      std::unordered_map<int, int> map;
      int left  = 0;
      int right = 0;
      for (int right = 0; right < size; right++){
         map[nums[right]]++;
         while (map.size() > k){
            if(map[nums[left]] == 1){
               map.erase(nums[left]);
            }else{
               map[nums[left]]--;
            }
            left++;
         }
         n += right - left + 1;
      }
      return n;
   }
   int subarraysWithKDistinct(vector<int>& nums, int k) {

      return find_distinct_up_to_k(nums, k) - find_distinct_up_to_k(nums, (k-1));
   }
   bool find_next_start(std::unordered_set<std::string>& dict, std::string s, vector<bool>& seen, int start){
      bool result = false;
      if(s.length()==0){
         return true;
      }
      for(int i = 1; i <= s.length(); i++){
         if(dict.count(s.substr(0, i))){
            std::cout << "check: " << s.substr( i) << "\n";
            if(!seen[start + i]){
               std::cout << "find next start for: " << s.substr(i) << "\n";
               result = find_next_start(dict, s.substr(i), seen, start + i);
            }else {
               std::cout << "already seen: " << s.substr(i) << "\n";
            }
            if(result){
               break;
            }
            seen[start + i] = true;
         }else{
            std::cout << "not found in dict: " << s.substr(0, i) << "\n";
         }
      }
      return result;
   }
   bool wordBreak(std::string s, vector<string>& wordDict) {
      std::unordered_set<string> dict(wordDict.begin(), wordDict.end());
      vector<bool> seen(s.length() + 1, false);
      int start = 0;
      return find_next_start(dict, s, seen, start);
   }
   int missingMultiple(vector<int>& nums, int k) {
      /*100 elements [1;100]
      k - start alwaye from k to k*1 k*2..k*n < 100 */
      /*unordered_set<int>  will not return min*/
      int n = 1;
      unordered_set<int> nums_set;
      for(auto& i: nums){
         if(i%k==0){
            nums_set.insert(i);
         }
      }
      while(k*n <= 200){
         if(nums_set.find(k*n)==nums_set.end()){
               return k*n;
         }
         n++;
      }
      return 0;
   }
   string mergeCharacters(string s, int k) {
      /*we can convert string to vector and manipulate it*/
      /*merging seems to be a removing
         aaabbbccc k = 1 aabbbccc abbbccc
         abcabca*/
      vector<char> array_s;
      array_s.reserve(s.length());
      /*translate to vector*/
      for(auto& c:s){
         array_s.push_back(c);
      }
      /*handle vector*/
      int size = array_s.size();
      int skip = 0;
      for(int i = 0;i<size;i++){
         if(array_s[i]!='\0'){
               unordered_set<char> dont_check;
               dont_check.insert(array_s[i]);
               skip = 0;
               for(int j = 1; j<=k+skip && j+i < size;j++){
                  if(array_s[i+j] == '\0'){
                     skip++;
                  }else if(dont_check.find(array_s[i+j]) != dont_check.end()){
                     array_s[i+j] = '\0';
                     skip++;
                  }else{
                     dont_check.insert(array_s[i+j]);
                  }
               }
         }
      }
      string merged;
      merged.reserve(100);
      for(auto& i: array_s){
         if(i != '\0' ){
               merged+=i;
         }
      }
      return merged;
   }
   vector<int> minDistinctFreqPair(vector<int>& nums) {
      vector<int> counts(101);
      for(auto& i:nums){
         counts[i]++;
      }
      int freq_min = 0;
      int freq_max = 0;
      for(int i = 1;i<counts.size();i++){
         if(counts[i] !=0){
               freq_min = counts[i];
               for(int j = i+1;j<counts.size();j++){
                  if(counts[j] > 0 && counts[j]!=counts[i]){
                     return vector<int>({i, j});
                  }
               }
         }
      }
      return {-1,-1};
   }
   bool isIsomorphic(string s, string t) {
   /*the same length
   aabbccdd -> eeffggii
   abcb - cdcd
   if we have replaced pair the same should be in all cases */
      unordered_map<char, char> connection;

      if(s.length()!= t.length()){
         return false;
      }else{
         for(int i = 0;i < s.length();i++){
               if(connection.find(s[i])==connection.end()){
                  connection[s[i]] = t[i];
               }else{
                  if (t[i] != connection[s[i]]){
                     return false;
                  }
               }
         }
         connection.clear();/*keep capacity*/
         for(int i = 0;i < t.length();i++){
            if(connection.find(t[i])==connection.end()){
               connection[t[i]] = s[i];
            }else{
               if (s[i] != connection[t[i]]){
                  return false;
               }
            }
         }
         return true;
      }
   }
};
#endif /* UNORDERED_SET_OR_MAP_HPP */
