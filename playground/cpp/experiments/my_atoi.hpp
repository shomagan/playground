#ifndef MY_ATOI_HPP
#define MY_ATOI_HPP
#include <cstdint>
#include <string>
#include <climits>
#include <cmath>
class MyAtoi {
public:   MyAtoi(){};
   ~MyAtoi(){};
   int32_t my_atoi(const std::string &input_string){
      int32_t result = 0;
      int32_t sign = 0;
      for(char c: input_string){
         if(sign == 0){
            if (c == '-'){
               sign = -1;
            }else if(c == '+'){
               sign = 1;
            }else if (c >= 0x30 && c <= 0x39){
               sign = 1;
               result = result * 10 * sign +  (c - 0x30)*sign;
            }else if(c != ' '){
               break;
            }
         }else if (c >= 0x30 && c <= 0x39){
            if ((sign > 0) && (result > (INT32_MAX-(c - 0x30))/10)){
               result = INT32_MAX;
               break;
            }
            if((sign < 0) && (result < (INT32_MIN-(c - 0x30)*sign)/10)){
               result = INT32_MIN;
               break;
            }
            result = result * 10  +  (c - 0x30)*sign;
         }else{
            break;
         }
      }
      return result;
   };
};
#endif /*MY_ATOI_HPP*/
