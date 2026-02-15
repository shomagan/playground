#ifndef CONTAINS_DUPLICATES_HPP
#define CONTAINS_DUPLICATES_HPP
#include <vector>
#include <unordered_set>

class ContainsDuplicates {
public:   ContainsDuplicates(){};
   ~ContainsDuplicates(){};
   bool contains_duplicates(std::vector<int>& nums) {
      std::unordered_set<int> seen;
      for(int num : nums) {
         if(seen.find(num) != seen.end()) {
            return true;
         }
         seen.insert(num);
      }
      return false;

   }
};
#endif /* CONTAINS_DUPLICATES_HPP */
