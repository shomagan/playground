#ifndef SAME_TREE_HPP
#define SAME_TREE_HPP
#include <iostream>
#include <vector>
#include "common.hpp"
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
class SameTree {
public:
   SameTree(){};
   ~SameTree(){};
   bool go_to_left_than_right(TreeNode* p, TreeNode* q){
      bool result = 1;
      std::cout << p->val << " " << q->val << "\n";
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
};
#endif // SAME_TREE_HPP
