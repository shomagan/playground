#ifndef STACK_PROBLEMS_HPP
#define STACK_PROBLEMS_HPP
#include <stack>
#include <string>
class StackProblems {
public:   StackProblems(){};
   ~StackProblems(){};
   /*Valid Parentheses
       Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
       An input string is valid if:
       Open brackets must be closed by the same type of brackets.
       Open brackets must be closed in the correct order.
       Every close bracket has a corresponding open bracket of the same type.
   */
   bool isValid(std::string s) {
      bool result = true;
      std::stack<char> s_stack;
      for (char c: s){
         if (c == '{' || c == '(' || c == '['){
            s_stack.push(c);
         }else{
            if (s_stack.empty()){return false;}
            char last = s_stack.top();
            s_stack.pop();
            if ((c == '}' && last != '{')||
                (c == ']' && last != '[')||
                (c == ')' && last != '(')){
                  return false;
            }
         }
      }

      if (s_stack.empty()){
        return true;
      }else{
        return false;
      }
   }
   /*The path must start with a single slash '/'.
     Directories within the path must be separated by exactly one slash '/'.
     The path must not end with a slash '/', unless it is the root directory.
     The path must not have any single or double periods ('.' and '..') used to denote current or parent directories.*/
   std::string simplifyPath(std::string path) {
      std::string simply(3000,'\0');
      int write_p = 0;
      if(path[0]!= '/'){
         simply = '/';
         write_p = 1;
      }
      path.resize(path.length() + 5);
      for(int i = path.length()- 5; i < path.length(); i++){
         path[i] = '\0';
      }
      /*keep pointers to path ,to initial character */
      std::stack<int> stack_path;
      int new_folder = 0;
      for(int i = 0; i < path.length(); i++){
         if(path[i] == '/'){
            if(write_p == 0 || simply[write_p-1] != '/'){
               simply[write_p++] = '/';
            }
            while(path[i] == '/' ){i++;};
            new_folder = 1;
         }
         if((i > 0 && path[i-1] == '/') &&
            (path[i] == '.') && (path[i+1] == '/')){
            i = i + 1;
         }else if((i > 0 && path[i-1] == '/') &&
                  (path[i] == '.') &&
                  (path[i+1] == '.') &&
                  (path[i+2] == '\0' || path[i+2] == '/')){
            if(stack_path.size()){
               write_p = stack_path.top();
               stack_path.pop();
            }
            i = i + 2;
         }else{
            if(path[i] == '\0'){
               break;
            }

            if(path[i-1] == '/' && path[i] == '.' && path[i+1] == '\0'){
               break;
            }
            if(new_folder){
               stack_path.push(write_p);
               new_folder = 0;
            }
            simply[write_p++] = path[i];

         }
      }
      if(write_p > 1){
         while(write_p > 1 && simply[write_p-1]=='/' ){
            simply.pop_back();
            write_p--;
         }
      }
      return simply.substr(0, write_p);
   }
};
#endif /* STACK_PROBLEMS_HPP */
