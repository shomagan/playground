#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <algorithm>
using namespace std;
class Matrix {
public:   Matrix() {};
   ~Matrix() = default;
   vector<vector<int>> reverseSubmatrix(vector<vector<int>>& grid, int x, int y, int k) {
      /*
         a1 a2 a3 a4 a5 a6 ... an
         ........................
         am1 am2 ..............amn
         first grid[x][y]                     grid[x][y+k-1]
               grid[x+1][y]                   grid[x+1][y+k-1]
               ...............................................
               grid[x+k-1-1][y]               grid[x+k-1-1][y+k-1-1]
               grid[x+k-1][y]                 grid[x+k-1][y+k-1]
         column by column because we need to change in rows
         */
      for(int c = y; c < y+k; c++){
         int i = 0;
         for(int r = x;r < x+k/2; r++){
               int temp = grid[r][c];
               grid[r][c] = grid[x+k-1-i][c];
               grid[x+k-1-i][c] = temp;
               i++;
         }
      }
      return grid;
   }
   vector<int> zigzagTraversal(vector<vector<int>>& grid) {
      /*we should keep result,
      we have columns number  = grid[0].size()
      we have rows number = grid.size()
      we can do one by one just toggle alternative bit
      */
      bool off = false;/*if true skip the value*/
      bool back = false;/*direction*/
      int columns = grid[0].size();
      int rows = grid.size();
      vector<int> alternate;
      alternate.reserve(columns*rows/2+1);
      int r = 0;
      while(r < rows){
         for(int c = 0;c<columns;c++){
               int temp_c = c;
               if(back){
                  temp_c = columns - c - 1;
               }
               if(!off){
                  alternate.push_back(grid[r][temp_c]);
               }
               off = !off;
         }
         back = !back;
         r++;
      }
      return alternate;
   }
   int minimumOperations(vector<vector<int>>& grid) {
      /*increasing in columns only
         1 2 3
         1 2 3
         1 2 3
         if we go up to down we dont have to return back , it is always increasing
         but need to update values on time
         */
      int columns = grid[0].size();
      int rows = grid.size();
      int operations = 0;
      for(int c = 0;c<columns;c++){
         int prev;
         for(int r=0;r<rows;r++){
               if(r>0 && grid[r][c] <= prev){
                  operations += (prev-grid[r][c]+1);/*2 1 */
                  grid[r][c] = prev+1;
               }
               prev = grid[r][c];
         }
      }
      return operations;
   }

};
#endif // MATRIX_HPP
