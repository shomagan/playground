#ifndef REVERSE_NODES_BY_GROUP_HPP
#define REVERSE_NODES_BY_GROUP_HPP
#include <vector>
#include "common.hpp"
/*Given the head of a linked list, reverse the nodes of the list k at a time,
 and return the modified list.
 k is a positive integer and is less than or equal to the length of the linked list.
 If the number of nodes is not a multiple of k then left-out nodes, in the end,
 should remain as it is.

You may not alter the values in the list's nodes, only nodes themselves may be changed.*/
class ReverseNodesByGroup {
public:   ReverseNodesByGroup(){};
   ~ReverseNodesByGroup(){};
   ListNode* reverse_nodes(ListNode* head, int k) {
      ListNode* last = head;
      if(head){
         ListNode* prev = head;
         ListNode* last_next = head;
         ListNode* current = head->next;

         while(--k){
            ListNode* next = current->next;
            last = current;
            last_next = current->next;
            current->next = prev;
            prev = current;
            current = next;
         }
         head->next = last_next;
      }
      return last;
   }

   ListNode* reverseKGroup(ListNode* head, int k) {
      if(k <= 1){
         return head;
      }
      int size = 0;
      ListNode * result = head;
      ListNode * dummy = head;
      while(dummy){
         size++;
         dummy = dummy->next;
      }
      dummy = head;
      ListNode* prev = nullptr;
      for (int i = 0; i < size/k;i++){
         ListNode* swapped = nullptr;
         swapped = reverse_nodes(dummy, k);
         if(i==0){
            result = swapped;
         }else{
            prev->next = swapped;
         }
         prev = dummy;
         dummy = dummy->next;

      }
      return result;
   }
};
#endif // REVERSE_NODES_BY_GROUP_HPP
