#ifndef REVERSE_INTEGER_HPP
#define REVERSE_INTEGER_HPP
#include <cstdint>
#include <vector>
#include <climits>
#include <cmath>

class ReverseInteger {
public:
   ReverseInteger(){};
   ~ReverseInteger(){};
   int32_t reverse(int32_t x){
      int32_t result = 0;
      int32_t remain = 0;
      while (x){
         remain = x%10;
         x = x/10;
         if((remain >= 0 &&(result > (INT32_MAX - remain)/10)) || (remain < 0 && (result < (INT32_MIN - remain)/10))){
            return 0;
         }
         result = result * 10 + remain;
      }
      return result;
   };
};
#endif /*REVERSE_INTEGER_HPP*/
