#include <cstdint>
#include <vector>
#include "common.hpp"

class LinkedListMerger {
public:
   LinkedListMerger(){};
   ~LinkedListMerger(){};
   ListNode* add_next(std::vector<ListNode*>& lists){
      ListNode * node = new ListNode(100000);
      int index = 0;
      for(int i = 0; i < lists.size();++i){
         if(lists[i]->val < node->val){
            index = i;
            node->val = lists[i]->val;
         }
      }
      if(lists[index] == nullptr){
         return nullptr;
      }
      if(lists[index] == nullptr || lists[index]->next == nullptr){
         /*last one, need to remove sub list*/
         lists.erase(lists.begin() + index);
      }else{
         /*move up */
         //ListNode * old = lists[index];
         lists[index] = lists[index]->next;
         //delete old;
      }
      return node;
   }
   ListNode* mergeKLists(std::vector<ListNode*>& lists) {
      for(int i = 0; i < lists.size();){
         if(lists[i] == nullptr){
            lists.erase(lists.begin() + i);
            i=0;
         }else{
            i++;
         }
      }
      ListNode* resultList = nullptr;
      ListNode* result = nullptr;
      while(lists.size()){
         if (resultList == nullptr){
            resultList = add_next(lists);
            result = resultList;
         }else{
            resultList->next = add_next(lists);
            resultList = resultList->next;
         }
      }
      return result;
   }
};
