#ifndef VALID_ANAGRAM_HPP
#define VALID_ANAGRAM_HPP
#include <string>
#include <algorithm>
#include <unordered_map>
class ValidAnagram {
public:   ValidAnagram(){};
   ~ValidAnagram(){};
   static bool is_anagram(std::string s, std::string t) {
      std::unordered_map <char, int> amount;
      if(s.size() != t.size()){
         return false;
      }
      for(char c: s){
         amount[c] =amount[c] + 1;
      }
      for(char c: t){
         if(amount.find(c)==amount.end() || amount[c] == 0){
            return false;
         }
         amount[c] = amount[c] - 1;
      }
      return true;
   }
   static bool is_anagram_sort(std::string s, std::string t) {
      if(s.size() != t.size()){
         return false;
      }
      std::sort(s.begin(),s.end());
      std::sort(t.begin(),t.end());
      for(int i = 0; i < s.size(); i++){
         if(s[i]!=t[i]){
            return false;
         }
      }
      return true;
   }

};
#endif /* VALID_ANAGRAM_HPP */
