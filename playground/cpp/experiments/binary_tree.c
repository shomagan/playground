#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//is not complited
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
void go_to_right_low_leaf(struct TreeNode* root, int * summ);

struct TreeNode* bstToGst(struct TreeNode* root) {
    int summ = 0;
    go_to_right_low_leaf(root,&summ);
    return root;
}
void go_to_right_low_leaf(struct TreeNode* root, int * summ){
    if (root->right!=NULL){
        go_to_right_low_leaf(root->right,summ);
    }
    *summ += root->val;
    root->val = *summ;
    if (root->left != NULL) {
        go_to_right_low_leaf(root->left,summ);
    }
    return;
}
int main(int argc, const char * argv[]) {
   int result = can_jump(test_array, sizeof(test_array)/sizeof(int));
   printf("result - %i ",result);
   return 0;
}
                  