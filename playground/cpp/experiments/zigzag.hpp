#ifndef ZIGZAG_HPP
#define ZIGZAG_HPP
#include <cstdint>
#include <string>
#include <vector>
class Zigzag {
public:
   Zigzag(){};
   ~Zigzag(){};
   std::string convert(const std::string & input, int rows){
      std::string converted = "";
      std::vector<std::string> zigzag_rows(rows);
      int row = 0;
      int up = 0;
      for(char c: input){
         zigzag_rows[row] += c;
         if (row == (rows - 1)){
            if(row != 0){
               row--;
            }
            up = 1;
         }else if(row == 0){
            row++;
            up = 0;
         }else if(up){
            row--;
         }else{
            row++;
         }
      }
      for(const std::string& row: zigzag_rows){
         converted += row;
      }
      return converted;
   }
};
#endif /*ZIGZAG_HPP*/
