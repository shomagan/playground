class Solution {
 public:
   /*
Given an array of distinct integers candidates and a target integer target, return a list of all unique combinations of candidates where the chosen numbers sum to target. You may return the combinations in any order.

The same number may be chosen from candidates an unlimited number of times. Two combinations are unique if the of at least one of the chosen numbers is different.

The test cases are generated such that the number of unique combinations that sum up to target is less than 150 combinations for the given input.

 */
   vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
      vector<vector<int>> result;
      vector<int> current;
      backtrack(candidates, target, 0, current, result);
      return result;
   }

 private:
   void backtrack(vector<int> &candidates, int target, int start, vector<int> &current, vector<vector<int>> &result) {
      if (target == 0) {
         result.push_back(current);
         return;
      }
      if (target < 0) {
         return;
      }
      for (int i = start; i < candidates.size(); ++i) {
         current.push_back(candidates[i]);
         backtrack(candidates, target - candidates[i], i, current, result);
         current.pop_back();
      }
   }
   /*Given a collection of candidate numbers (candidates) and a target number (target), find all unique combinations in candidates where the candidate numbers sum to target.
    Each number in candidates may only be used once in the combination.
    Note: The solution set must not contain duplicate combinations.*/
   vector<vector<int>> combinationSum2(vector<int> &candidates, int target) {
      vector<vector<int>> result;
      vector<int> current;
      sort(candidates.begin(), candidates.end());
      backtrack2(candidates, target, 0, current, result);
      return result;
   }
   void backtrack2(vector<int> &candidates, int target, int start, vector<int> &current, vector<vector<int>> &result) {
      if (target == 0) {
         result.push_back(current);
         return;
      }
      if (target < 0) {
         return;
      }
      for (int i = start; i < candidates.size(); ++i) {
         if (i > start && candidates[i] == candidates[i - 1])
            continue; // skip duplicates
         current.push_back(candidates[i]);
         backtrack2(candidates, target - candidates[i], i + 1, current, result);
         current.pop_back();
      }
   }
   vector<vector<int>> permute(vector<int> &nums) {
      vector<vector<int>> result;
      vector<int> current;
      int start = 0;
      backtrack_permute(nums, start, result, current);
      return result;
   }

 private:
   void backtrack_permute(vector<int> &nums, int start, vector<vector<int>> &result, vector<int> &current) {
      if (current.size() == nums.size()) {
         result.push_back(current);
         return;
      }
      for (int i = start; i < nums.size(); i++) {
         int temp = nums[start];
         nums[start] = nums[i];
         nums[i] = temp;
         current.push_back(nums[start]);
         backtrack(nums, start + 1, result, current);
         nums[i] = nums[start];
         nums[start] = temp;
         current.pop_back();
      }
   }
   vector<vector<int>> combine(int n, int k) {
      /*if we  take first element always 1, we should eliminate it for further use
        1 2 3, if n and k are equal we have only one combination
        */
      vector<vector<int>> result;
      vector<int> current;
      backtrack_combine(result, current, 0, 1, n, k);
      return result;
   }

 private:
   void backtrack_combine(vector<vector<int>> &result, vector<int> &current, int added, int current_index, int n, int k) {
      if (added == k) {
         result.push_back(current);
         return;
      }
      /*if(n - current_index < k - added ){
            return;
        }*/

      for (int i = current_index; i <= n; i++) {
         current.push_back(i);
         backtrack_combine(result, current, added + 1, i + 1, n, k);
         current.pop_back();
      }
   }
   void backtracking(vector<int> &nums, vector<int> &current, vector<vector<int>> &result, int from, int size) {
      if (current.size() == size) {
         result.push_back(current);
         return;
      }
      for (int i = from; i < nums.size(); i++) {
         current.push_back(nums[i]);
         backtracking(nums, current, result, i + 1, size);
         current.pop_back();
      }
   }
   vector<vector<int>> subsets(vector<int> &nums) {
      /* each element should be uniq [1, 2] thea same to [2, 1] */
      vector<vector<int>> result;
      vector<int> current;
      for (int i = 0; i <= nums.size(); i++) {
         current.clear();
         backtracking(nums, current, result, 0, i);
      }
      return result;
   }
};
