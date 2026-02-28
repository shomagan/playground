#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP
#include <vector>
#include <cstdint>
class BitOperations {
public:   BitOperations(){};
   ~BitOperations(){};
   /*
   -231 <= dividend, divisor <= 231 - 1
   divisor != 0*/
   int divide(int dividend, int divisor) {
      int quotient  = 0;
      if (dividend == divisor){
         return 1;
      }
      int sign = 1;
      if ((dividend < 0) != (divisor < 0)){
         sign = -1;
      }
      uint32_t abs_divider = 0;
      uint32_t abs_divisor = 0;
      uint32_t abs_quotient = 0;
      if (dividend < 0){
         if (dividend == INT32_MIN){
            abs_divider = (uint32_t)INT32_MAX + 1;
         }else{
            abs_divider = 0 -  dividend;
         }
      }else{
         abs_divider = dividend;
      }
      if (divisor < 0){
         if (divisor == INT32_MIN){
            abs_divisor = (uint32_t)INT32_MAX + 1;
         }else{
            abs_divisor = 0 -  divisor;
         }
      }else{
         abs_divisor = divisor;
      }
      uint32_t remainder = abs_divider;
      while (remainder >= abs_divisor ){
         int i = 0;
         while (remainder > (abs_divisor<<(i+1))){
            i++;
         }
         abs_quotient += (1<<i);
         remainder = remainder - (abs_divisor<<i);
      }
      if (sign == 1){
         if (abs_quotient > INT32_MAX){
            quotient = INT32_MAX;
         }else{
            quotient = abs_quotient;
         }
      }else{
         if (abs_quotient > (uint32_t)INT32_MAX+1){
            quotient = INT32_MIN;
         }else{
            quotient = 0 - abs_quotient;
         }
      }
      return quotient;

   }
};
#endif /* BIT_OPERATIONS_HPP */