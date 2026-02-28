#ifndef MATRIX_PROBLEMS_HPP
#define MATRIX_PROBLEMS_HPP
#include <vector>
class MatrixProblems {
public:   MatrixProblems(){};
   ~MatrixProblems(){};
   /*Rotate Image
     You are given an n x n 2D matrix representing an image, rotate the image by 90 degrees (clockwise).
     You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.
   */
   void rotate(std::vector<std::vector<int>>& matrix) {
      /*matrix[0][0] -> matrix[0][matrix[0].size()-1]*/
      int size = matrix.size();
      int last_item = size-2;
      int iteration_count = 0;
      std::cout << "last_item " << last_item << "\n";
      int r = 0;
      while (size > 1) {
         for(int l = r; l <= last_item; l++){
            int temp1 = matrix[r][l];
            int temp2 = matrix[l][last_item + 1];
            int temp3 = matrix[r + size - 1][r + size - 1 -(l-r)];
            int temp4 = matrix[r + size - 1 - (l-r)][r];
            matrix[r][l] = temp4;
            matrix[l][last_item + 1] = temp1;
            matrix[r + size - 1][r + size - 1 - (l-r)] = temp2;
            matrix[r + size - 1 - (l-r)][r] = temp3;
            std::cout << "indexes we change " << r + size - 1 - (l-r) << " " << r<< " "  << l<< " "  << last_item + 1 << " " <<r+size-1<< " \n"  ;
            iteration_count++;
            for(const auto& row : matrix) {
               for(int num : row) {
                  std::cout << num << " ";
               }
               std::cout << "\n";
            }

         }
         r++;
         size = size - 2;
         if(size <= 1){
            break;
         }else{
            last_item = r + size - 2;
            std::cout << "iteration_count " << iteration_count << "\n";
            std::cout << "last_item " << last_item << "\n";
         }
      }
      std::cout << "iteration_count " << iteration_count << "\n";
   }
   std::vector<int> matrix_walk(std::vector<std::vector<int>>& matrix){
      /*need to know where do we walk*/
      typedef enum{
         RIGHT = 0,
         DOWN = 1,
         LEFT = 2,
         UP = 3
      } path_t;
      path_t path = RIGHT;
      /*first turn index*/
      int turn_index = matrix[0].size() - 1;
      /*what would be the end , no more lines or rows*/
      int lines_left = matrix.size();
      int rows_left = matrix[0].size();
      int current_line = 0;
      int current_row = 0;
      int current = 0;
      int index = 0;
      std::vector<int> result(matrix.size() * matrix[0].size(), 0);
      while (((path == RIGHT || path == LEFT) && (rows_left)) || ((path == UP || path == DOWN) && (lines_left))){
         while (current != turn_index){
            result[index++] = matrix[current_line][current_row];
            switch(path){
               case RIGHT:
                  current++;
                  current_row = current;
                  break;
               case DOWN:
                  current++;
                  current_line = current;
                  break;
               case LEFT:
                  current--;
                  current_row = current;
                  break;
               case UP:
                  current--;
                  current_line = current;
                  break;
               default:
                  path = RIGHT;
                  break;
            }
         }
         if(current == turn_index){
            /*trun moment*/
            switch(path){
               case RIGHT:
                  path = DOWN;
                  lines_left--;
                  current = current_line;
                  turn_index = current_line + lines_left;
                  break;
               case DOWN:
                  path = LEFT;
                  rows_left--;
                  current = current_row;
                  turn_index = current_row - rows_left;
                  break;
               case LEFT:
                  path = UP;
                  lines_left--;
                  current = current_line;
                  turn_index = current_line - lines_left;
                  break;
               case UP:
                  path = RIGHT;
                  rows_left--;
                  current = current_row;
                  turn_index = current_row + rows_left;
                  break;
               default:
                  path = RIGHT;
                  break;
            }
         }
         if (current == turn_index){
            result[index++] = matrix[current_line][current_row];
         }
      }
      return result;
   }
};
#endif /* MATRIX_PROBLEMS_HPP */