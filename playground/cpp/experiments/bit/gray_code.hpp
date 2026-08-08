#include <vector>
using namespace std;
class GrayCode {
 public:
   vector<int> gray_code(int n) {
      /*only one bit , xor weigth gives 1 */
      /*a & (a -1)*/
      int max = 1 << n;
      vector<int> result;
      //result.reserve(max);
      for (int i = 0; i < max; i++) {
         result.push_back(i ^ i >> 1);
      }
      return result;
   }
};
