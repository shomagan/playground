#ifndef REVERSE_NODES_HPP
#define REVERSE_NODES_HPP
#include "common.hpp"
class ReverseNodes{
public:   ReverseNodes(){};
   ~ReverseNodes(){};
   ListNode* reverse_nodes(ListNode* head) {
      ListNode* last = head;
      if(head){
         ListNode* prev = head;
         ListNode* current = head->next;
         head->next = nullptr;
         while(current){
            ListNode* next = current->next;
            last = current;
            current->next = prev;
            prev = current;
            current = next;
         }
      }
      return last;
   }
};
#endif /* REVERSE_NODES_HPP */
