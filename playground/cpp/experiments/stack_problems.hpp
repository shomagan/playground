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
};
#endif /* STACK_PROBLEMS_HPP */
