#ifndef SAME_TREE_HPP
#define SAME_TREE_HPP
#include <iostream>
#include <vector>
#include "common.hpp"
#include <cstdint>
/*Given the roots of two binary trees p and q, write a function to check if they are the same or not.

Two binary trees are considered the same if they are structurally identical, and the nodes have the same value.
struct TreeNode {
   int val;
   TreeNode *left;
   TreeNode *right;
   TreeNode() : val(0), left(nullptr), right(nullptr) {}
   TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
   TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};*/
class BinTree {
public:
   BinTree(){};
   ~BinTree(){};
   bool go_to_left_than_right(TreeNode* p, TreeNode* q){
      bool result = 1;
      if(q && p){
         if(p->val == q->val && ((p->left != nullptr) == (q->left != nullptr)) && ((p->right != nullptr) == (q->right != nullptr))){
            if(p->left && q->left){
               result = go_to_left_than_right(p->left, q->left);
            }
            if(result && p->right && q->right){
               result = go_to_left_than_right(p->right, q->right);
            }
         }else{
            result = 0;
         }
      }else if ((q == nullptr) != (p == nullptr) ){
         result = 0;
      }
      return result;
   }
   bool isSameTree(TreeNode* p, TreeNode* q) {
      return go_to_left_than_right(p, q);
   }
   int max_path_pass_left_to_right(TreeNode* root, int& max_sum){
      int branch_sum = 0;
      if(root){
         branch_sum = root->val;
         if(max_sum < branch_sum){
            max_sum = branch_sum;
         }
         int max_left = 0;
         int max_right = 0;
         if (root->left){
            max_left = max_path_pass_left_to_right(root->left, max_sum);
         }
         if (root->right){
            max_right = max_path_pass_left_to_right(root->right, max_sum);
         }
         if(max_sum < (max_right + max_left + branch_sum)){
            max_sum = (max_right + max_left + branch_sum);
         }
         if(max_sum < (max_right + branch_sum)){
            max_sum = (max_right + branch_sum);
         }
         if(max_sum < (max_left + branch_sum)){
            max_sum = (max_left + branch_sum);
         }
         if(max_sum < branch_sum){
            max_sum = branch_sum;
         }
         if(max_left > max_right){
            branch_sum = (branch_sum + max_left)>branch_sum?(branch_sum + max_left):branch_sum;
         }else{
            branch_sum = (branch_sum + max_right)>branch_sum?(branch_sum + max_right):branch_sum;
         }

      }
      return branch_sum;
   }
   /*A path in a binary tree is a sequence of nodes where each pair of adjacent nodes in the sequence has an edge connecting them.
     A node can only appear in the sequence at most once. Note that the path does not need to pass through the root.
     The path sum of a path is the sum of the node's values in the path.
     Given the root of a binary tree, return the maximum path sum of any non-empty path.*/
   int maxPathSum(TreeNode* root) {
      int max_sum = INT_MIN;
      max_path_pass_left_to_right(root, max_sum);
      return max_sum;
   }
};
#endif // BIN_TREE_HPP
