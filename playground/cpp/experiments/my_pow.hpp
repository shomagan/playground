#ifndef MY_POW_HPP
#define MY_POW_HPP
#include <cmath>
class MyPow {
public:
   MyPow(){};
   ~MyPow(){};
   double my_pow(double x, int n) {
      double result = 1.0;
      if(n == 0){
         return 1;
      }else if (x == 0){
         return 0;
      }else if (n > 0){
         while (n){
            if (n & 0x01){
               result *= x;
               n--;
            }
            x *= x;
            n = n>>1;
         }
      }else{
         signed long long ns = (signed long long)n *(-1);
         x = 1/x;
         while (ns){
            if (ns & 0x01){
               result *= x;
               ns--;
            }
            x *= x;
            ns = ns>>1;
         }
      }
      return result;
   }
};
#endif // MY_POW_HPP
