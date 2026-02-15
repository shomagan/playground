#ifndef LONGEST_PALINDROMICS_HPP
#define LONGEST_PALINDROMICS_HPP
#include <string>
#include <cstdint>
/*one function class to find the longest palindromic substring*/
class LongestPalindromic {
public:
   LongestPalindromic(){};
   ~LongestPalindromic(){};
   std::string longest_palindromic(const std::string &input_string){
      uint32_t max_len = 1;
      uint32_t start_position = 0;
      std::string longes_palindromic = "";
      for (auto i = 0; i < input_string.length();++i ){
         int32_t left = i;
         int32_t rigth = i;
         while (left >= 0 && rigth <= input_string.length() && input_string[left] == input_string[rigth] ){
            if ((rigth - left + 1 ) > max_len){
               start_position = left;
               max_len = rigth - left + 1;
            }
            left -= 1;
            rigth += 1;
         }
         left = i;
         rigth = i + 1;
         while (left >= 0 && rigth <= input_string.length() && input_string[left] == input_string[rigth] ){
            if ((rigth - left + 1 ) > max_len){
               start_position = left;
               max_len = rigth - left + 1;
            }
            left -= 1;
            rigth += 1;
         }
      }
      longes_palindromic = input_string.substr(start_position, max_len);
      return longes_palindromic;
   };
};
#endif /*LONGEST_PALINDROMICS_HPP*/
