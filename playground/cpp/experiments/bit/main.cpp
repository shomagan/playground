/*main function will call necessary functions based on arguments*/
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "gray_code.hpp"

using namespace std;
int main(int argc, const char * argv[]) {
   /*input arg handle*/

   if (argc >= 2) {
      std::string input_string = argv[1];
      if(argc == 3 && input_string.compare("gray_code") == 0) {
         int number = std::stoi(argv[2]);
         GrayCode gc;
         /*time benchmark */
         auto start = std::chrono::high_resolution_clock::now();
         vector<int> result = gc.gray_code(number);
         auto end = std::chrono::high_resolution_clock::now();
         std::chrono::duration<double, std::milli> duration = end - start;
         auto first = result[0];
         auto last = result[result.size() - 1];
         std::cout << "First element: " << first << "\n";
         std::cout << "Last element: " << last << "\n";
         std::cout << "Execution time: " << duration.count() << " ms\n";
      } else {
         return 0;
      }
   } else {
      return 0;
   }
   return 0;
}
