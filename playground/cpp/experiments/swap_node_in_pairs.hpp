#include <cstdint>
#include <vector>
#include "common.hpp"


class SwapNodeInPairs {
public:
   SwapNodeInPairs(){};
   ~SwapNodeInPairs(){};
   void swapPairsRecurcive(ListNode* head){
      if(head != nullptr){
         if(head->next != nullptr){
            ListNode* down;
            ListNode* down_next;
            down = head;
            head = head->next;
            down_next = head->next;
            head->next = down;
            down->next = down_next->next;
            swapPairsRecurcive(down->next);
         }
      }
   }
   ListNode* swapPairs(ListNode* head) {
      ListNode * head_copy = new ListNode(0);
      head_copy->next = head;
      if(head && head->next){
         head_copy->next = head->next;
      }
      ListNode * prev = nullptr;
      while (head != nullptr &&  head->next != nullptr){
         ListNode * down = head->next;
         head->next = down->next;
         down->next = head;
         if(prev != nullptr){
            prev->next = down;
         }
         prev = head;
         head = head->next;
      }


      return head_copy->next;
   }
};
