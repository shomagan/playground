#include <stdio.h>       
#include <string.h>
#include <math.h>
#include <stdlib.h>
static struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2);

struct ListNode {
    int val;
    struct ListNode *next;
};
struct ListNode l1_1 = {.val = 1,.next = NULL};
struct ListNode l1_0 = {.val = 4,.next = &l1_1};
struct ListNode l1 = {.val = 2,.next = &l1_0};
struct ListNode l2_1 = {.val = 4,.next = NULL};
struct ListNode l2_0 = {.val = 6,.next = &l2_1};
struct ListNode l2 = {.val = 5,.next = &l2_0};

int main(int argc, const char * argv[]) {
   struct ListNode* r = addTwoNumbers(&l1,&l2);
   printf(" %i ",r->val);
   do {
      r = r->next;
      printf(" %i ",r->val);
   }while(r->next != NULL);
   return 0;
}


static struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode * result = malloc(sizeof(struct ListNode));
    struct ListNode * r = result;
    result->next = NULL;
    int val = l1->val + l2->val;
    r->val = val % 10;
    int move_1 = val >= 10?1:0;
    int interructions = 0; 
    while (l1->next != NULL || l2->next != NULL || move_1){
        interructions++;
        int val1,val2;
        if (l1->next != NULL){
            l1 = l1->next;
            val1 = l1->val; 
        }else{
            val1 = 0;
        }
        if (l2->next != NULL){
            l2 = l2->next;
            val2 = l2->val;
        }else{
            val2 = 0;
        }
        printf("val1 %i, val2 %i, move_1 %i",val1,val2,move_1);
        val = val1 + val2 + move_1;
        r->next = malloc(sizeof(struct ListNode));
        r = r->next;
        r->next = NULL;
        r->val = val % 10;
        move_1 = val >= 10?1:0; 
    }
    printf("interructions %i",interructions);
    return result;
}