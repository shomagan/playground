#include <cstdint>
#include <vector>
class GetSum {
 public:
   int get_sum(int a, int b) {
      /*without + -        */
      int result = 0;
      int reminder = 0;
      for (uint64_t i = 1; (i & (uint64_t)INT32_MIN << 1) == 0; i = i << 1) {
         if ((a & i) && (b & i)) {
            if (reminder) {
               result |= i;
            } else {
               result &= ~i;
            }
            reminder = 1;
         } else if ((a & i) || (b & i)) {
            if (reminder) {
               reminder = 1;
               result &= ~i;
            } else {
               reminder = 0;
               result |= i;
            }
         } else {
            if (reminder) {
               result |= i;
               reminder = 0;
            } else {
               /*keep reminder = 0 */
               result &= ~i;
            }
         }
      }
      return result;
   }
   int hammingWeight(int n) {
      /*should i check ever bit?
         0(32) */
      int weight = 0;
      while (n) {
         n = n & (n - 1);
         weight++;
      }
      return weight;
   }
   vector<int> minBitwiseArray(vector<int> &nums) {
      /* a or a+1 alway bigger or equal value
        should have 1 at lsb */
      vector<int> result;
      for (auto i : nums) {
         if ((i & 1) == 0) {
            result.push_back(-1);
         } else {
            /* alway can be (i - 1) but need to try to find lower
                100111 = 100011 | 100100 it is better than 100110 | 100111*/
            int sequence_of_ones = i ^ i + 1;
            sequence_of_ones = sequence_of_ones >> 1;
            int temp = sequence_of_ones ^ sequence_of_ones >> 1;
            int base = i & ~temp;
            result.push_back(base);
         }
      }
      return result;
   }
};
