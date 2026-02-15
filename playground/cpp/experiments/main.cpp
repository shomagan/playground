/*main function will call necessary functions based on arguments*/
#include <iostream>
#include <string>
#include <vector>
#include "longest_palindromics.hpp"
#include "zigzag.hpp"
#include "reverse_integer.hpp"
#include "my_atoi.hpp"
#include "merge_linked_lists.hpp"
#include "swap_node_in_pairs.hpp"
#include "maximum_score_for_k_pairs.hpp"
#include "dp.hpp"
#include "same_tree.hpp"
#include "common.hpp"
#include "my_pow.hpp"
#include "flag_sort.hpp"
#include "reverse_nodes_by_group.hpp"
#include "reverse_nodes.hpp"
#include "contains_duplicates.hpp"
#include "valid_anagram.hpp"
#include "unordered_set_or_map.hpp"
#include "sliding_window.hpp"
int main(int argc, const char * argv[]) {
   /*input arg handle*/
   if (argc >= 2) {
      std::string input_string = argv[1];
      if(argc == 3 && input_string.compare("longest_palindromic") == 0) {
         LongestPalindromic lp;
         std::cout << "longest palindrom is: ";
         std::cout << lp.longest_palindromic(argv[2]) << "\n" ;
      }else if(argc == 4 && input_string.compare("zigzag") == 0) {
         Zigzag zigzag;
         int rows = std::stoi(argv[3]);
         std::cout << "zigzag output is:";
         std::cout << zigzag.convert(argv[2], rows) << "\n";
      }else if(argc == 3 && input_string.compare("reverse_integer") == 0) {
         ReverseInteger reverseInteger;
         int number = std::stoi(argv[2]);
         std::cout << "reverse integer output is:";
         std::cout << reverseInteger.reverse(number) << "\n";
      }else if(argc == 3 && input_string.compare("my_atoi") == 0) {
         MyAtoi myAtoi;
         std::cout << "my atoi output is:";
         std::cout << myAtoi.my_atoi(argv[2]) << "\n";
      }else if(argc == 2 && input_string.compare("merge_linked_lists") == 0) {
         LinkedListMerger listMerger;
         ListNode a5(10, nullptr);
         ListNode a4(8, &a5);
         ListNode a3(6, &a4);
         ListNode a2(4, &a3);
         ListNode a1(2, &a2);
         ListNode a0(0, &a1);
         ListNode b5(9, nullptr);
         ListNode b4(7, &b5);
         ListNode b3(5, &b4);
         ListNode b2(3, &b3);
         ListNode b1(1, &b2);
         ListNode b0(0, &b1);
         ListNode c5(11, nullptr);
         ListNode c4(9, &c5);
         ListNode c3(7, &c4);
         ListNode c2(5, &c3);
         ListNode c1(3, &c2);
         ListNode c0(1, &c1);
         std::vector<ListNode*> lists = {&a0, &b0, &c0};
         ListNode * result;
         std::cout << "merge linked lists output is:";
         result = listMerger.mergeKLists(lists);
         ListNode* current = result;
         while(current != nullptr) {
            std::cout << current->val;
            if(current->next != nullptr) {
               std::cout << " -> ";
            }
            current = current->next;
         }
         std::cout << "\n";
      }else if(argc == 2 && input_string.compare("swap_node_in_pairs") == 0) {
         SwapNodeInPairs swapNodeInPairs;
         ListNode a5(10, nullptr);
         ListNode a4(8, nullptr);
         ListNode a3(6, &a4);
         ListNode a2(4, &a3);
         ListNode a1(2, &a2);
         ListNode a0(0, &a1);
         ListNode * result;
         std::cout << "swap node in pairs output is:";
         result = swapNodeInPairs.swapPairs(&a0);
         // Print the result
         ListNode* current = result;
         while(current != nullptr) {
            std::cout << current->val;
            if(current->next != nullptr) {
               std::cout << " -> ";
            }
            current = current->next;
         }
         std::cout << "\n";
      }else if(argc == 2 && input_string.compare("maximum_score_for_k_pairs") == 0) {
         MaximumScoreForKPairs maxScoreForKPairs;
         std::vector<int> nums1 = {1,3,2};
         std::vector<int> nums2 = {4,5,1};
         int k = 2;
         std::cout << "maximum score for k pairs output is:";
         std::cout << maxScoreForKPairs.maxScore(nums1, nums2, k) << "\n";
      }else if(argc == 2 && input_string.compare("maximum_score_for_k_seq_pairs") == 0) {
         DP maxScoreForKSeqPairs;
         std::vector<int> nums1 = {1,3,2};
         std::vector<int> nums2 = {4,5,1};
         int k = 2;
         std::cout << "maximum score for k seq pairs output is:";
         std::cout << maxScoreForKSeqPairs.maxScore(nums1, nums2, k) << "\n";
         DP climbStairs;
         int n = 4;
         std::cout << "climb ways " << climbStairs.climb_stairs_dp(n) << "\n";
      }else if(argc == 2 && input_string.compare("same_tree") == 0) {
         SameTree sameTree;
         TreeNode a3(3, nullptr, nullptr);
         TreeNode a2(2, nullptr, nullptr);
         TreeNode a1(1, &a2, &a3);
         TreeNode b3(3, nullptr, nullptr);
         TreeNode b2(2, nullptr, nullptr);
         TreeNode b1(1, &b2, &b3);
         std::cout << "same tree output is:";
         std::cout << sameTree.isSameTree(&a1, &b1) << "\n";
      }else if(argc == 4 && input_string.compare("my_pow") == 0) {
         MyPow myPow;
         double x = std::stod(argv[2]);
         int n = std::stoi(argv[3]);
         std::cout << x << "^" << n << "=" ;
         std::cout << myPow.my_pow(x, n) << "\n";
      }else if(argc == 2 && input_string.compare("flag_sort") == 0) {
         FlagSort flagSort;
         std::vector<int> nums = {2,0,1};
         std::cout << "flag sort output is:";
         flagSort.flag_sort(nums);
         for(int num : nums) {
            std::cout << num << " ";
         }
         std::cout << "\n";
      }else if(argc == 2 && input_string.compare("reverse_nodes_by_group") == 0) {
         ReverseNodesByGroup reverseNodesByGroup;
         ListNode a5(6, nullptr);
         ListNode a4(5, &a5);
         ListNode a3(4, &a4);
         ListNode a2(3, &a3);
         ListNode a1(2, &a2);
         ListNode a0(1, &a1);
         std::cout << "reverse nodes output is:";
         ListNode* result = reverseNodesByGroup.reverseKGroup(&a0, 2);
         ListNode* current = result;
         while(current != nullptr) {
            std::cout << current->val;
            if(current->next != nullptr) {
               std::cout << " -> ";
            }
            current = current->next;
         }
         std::cout << "\n";
      }else if(argc == 2 && input_string.compare("reverse_nodes") == 0) {
         ReverseNodes reverseNodes;
         ListNode a5(6, nullptr);
         ListNode a4(5, &a5);
         ListNode a3(4, &a4);
         ListNode a2(3, &a3);
         ListNode a1(2, &a2);
         ListNode a0(1, &a1);
         std::cout << "reverse nodes output is:";
         ListNode* result = reverseNodes.reverse_nodes(&a0);
         ListNode* current = result;
         while(current != nullptr) {
            std::cout << current->val;
            if(current->next != nullptr) {
               std::cout << " -> ";
            }
            current = current->next;
         }
         std::cout << "\n";
      }else if(argc == 2 && input_string.compare("contains_duplicates") == 0) {
         ContainsDuplicates containsDuplicates;
         std::vector<int> nums = {1,2,3,1};
         std::cout << "contains duplicates output is:";
         std::cout << containsDuplicates.contains_duplicates(nums) << "\n";
      }else if(argc == 2 && input_string.compare("valid_anagram") == 0) {
         ValidAnagram validAnagram;
         std::string s = "anagram";
         std::string t = "nagaram";
         std::cout << "valid anagram 1 output is:";
         std::cout << validAnagram.is_anagram(s, t) << "\n";
         std::cout << validAnagram.is_anagram_sort(s, t) << "\n";

         s = "rat";
         t = "car";
         std::cout << "valid anagram 2 output is:";
         std::cout << validAnagram.is_anagram(s, t) << "\n";
         std::cout << validAnagram.is_anagram_sort(s, t) << "\n";
      }else if(argc == 2 && input_string.compare("first_uniq_character") == 0) {
         UnorderedSetOrMap unorderedSetOrMap;
         std::string s = "leletcode";
         std::cout << "first unique character output is:";
         std::cout << unorderedSetOrMap.first_uniq_character(s) << "\n";
      }else if(argc == 2 && input_string.compare("letter_combination") == 0) {
         UnorderedSetOrMap unorderedSetOrMap;
         std::string test = "23";
         std::vector<std::string> result = unorderedSetOrMap.letterCombinations(test);
         for (std::string s: result){
            std::cout << s << "\n";
         }
      }else if(argc == 2 && input_string.compare("group_anagrams") == 0) {
         UnorderedSetOrMap unorderedSetOrMap;
         std::vector<std::string> strs = {"eat","tea","tan","ate","nat","bat"};
         std::vector<std::vector<std::string>> result = unorderedSetOrMap.groupAnagrams(strs);
         for (const auto& group : result) {
            for (const auto& s : group) {
               std::cout << s << " ";
            }
            std::cout << "\n";
         }
      }else if(argc == 2 && input_string.compare("sliding_window") == 0) {
         SlidingWindow slidingWindow;
         std::string s = "XADOBECODEBANC";
         std::cout << s << "\n";
         std::string t = "ABC";
         std::cout << "sliding window output is:";
         std::cout << slidingWindow.minWindow(s, t) << "\n";
      }else {
         return 0;
      }
   } else {
      return 0;
   }
   return 0;
}
