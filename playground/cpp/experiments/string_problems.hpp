#ifndef STRING_PROBLEMS_HPP
#define STRING_PROBLEMS_HPP
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std;
class StringProblems {
public:   StringProblems() {};
   ~StringProblems() = default;
    bool isPalindrome(string s) {
        int len = s.length();
        int left = 0;
        int right = len-1;
        /*abcba 5 2 abba 4 2 */
        while(left < right){
            if(!isalnum(s[left])){
                left++;
            }else if(!isalnum(s[right])){
                right--;
            }else{
                int l = s[left];
                int r = s[right];
                if(isalpha(s[left])){
                    l = tolower(s[left]);
                }
                if(isalpha(s[right])){
                    r = tolower(s[right]);
                }
                if(r!=l){
                    return false;
                }
                left++;
                right--;
            }
        }
        return true;
    }
};
#endif // STRING_PROBLEMS_HPP
