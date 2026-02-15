#ifndef SLIDING_WINDOW_HPP
#define SLIDING_WINDOW_HPP
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <cstdint>
#include <unordered_map>
class SlidingWindow {
public:   SlidingWindow(){};
   ~SlidingWindow(){};
   std::string minWindow(std::string s, std::string t) {
      int index = -1;
      int size = INT32_MAX;
      int left =0;
      int right = 0;
      int length_s = s.length();
      int length_t = t.length();
      std::string result = "";
      std::unordered_map<char, int> t_table;
      for(char c: t){
         t_table[c] = t_table[c] + 1;
      }
      int symbols_left = length_t;
      if(symbols_left && length_s>=length_t){
         while (right < length_s){
            if(t_table.find(s[right]) != t_table.end()){
               if(t_table[s[right]]>0){
                  symbols_left--;
               }
               t_table[s[right]] = t_table[s[right]] - 1;
               if(symbols_left==0){
                  std::cout << t_table['A'] << " " << t_table['B'] << " " << t_table['C'] << "\n";
                  std::cout<<"found a window: "<<s.substr(left, right-left+1)<<"\n";
                  while(symbols_left==0 && left<=right){
                     if((right-left+1) < size){
                        std::cout<<"found a smaller window: "<<s.substr(left, right-left+1)<<"\n";
                        size = (right-left+1);
                        index = left;
                     }
                     if(t_table.find(s[left]) != t_table.end()){
                        if(t_table[s[left]]==0){
                           symbols_left++;
                        }
                        t_table[s[left]] = t_table[s[left]] + 1;
                     }
                     left++;
                  }
               }
            }
            right++;
         }
         if(index != -1){
            result = s.substr(index, size);
         }
      }
      return result;

   }
};
#endif /* SLIDING_WINDOW_HPP */
