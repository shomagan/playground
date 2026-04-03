#include <cstdint>
#include <vector>
#include "common.hpp"
#include <vector>
#include <algorithm>

using namespace std;
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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* current = nullptr;
        ListNode* head = nullptr;
        if(list1 && list2){
            if(list1->val < list2->val ){
                current = list1;
                list1 = list1->next;
            }else{
                current = list2;
                list2 = list2->next;
            }
        }else if(list1){
            return list1;
        }else if(list2){
            return list2;
        }else{
            return nullptr;
        }
        head = current;
        while(list1 || list2){
            if(list1 && list2){
                if(list1->val < list2->val ){
                    current->next = list1;
                    list1 = list1->next;
                }else{
                    current->next = list2;
                    list2 = list2->next;
                }
                current = current->next;
            }else if(list1){
                current->next = list1;
                break;
            }else if(list2){
                current->next = list2;
                break;
            }
        }
        return head;
    }
    ListNode* rotateRight(ListNode* head, int k) {
      /*we can simply al the time go down and move last to first*/
      /*or we can connect last and first then move head and make a break*/
      int counter = 0;
      ListNode* head_new = head;
      if(head){
         /*go down*/
         while(head_new->next){
            counter++;
            head_new = head_new->next;
         }
         counter++;
         head_new->next = head;/*now it is circle*/
         k = k%counter;
         ListNode* prev_node = head;
         head_new = head;
         for(int i = 0; i < counter - k; i++){
            prev_node = head_new;
            head_new = head_new->next;
         }
         prev_node->next = nullptr;
      }
      return head_new;
    }
    ListNode* modifiedList(vector<int>& nums, ListNode* head) {
        sort(nums.begin(), nums.end());
        ListNode* prev = nullptr;
        ListNode* current = head;
        while(current){
            if(binary_search(nums.begin(), nums.end(), current->val)){
                if(prev == nullptr){
                    head = current->next;
                }else{
                    prev->next = nullptr;
                }

                /*deallocate if needed*/
            }else{
                if(prev != nullptr){
                    prev->next = current;
                }
                prev = current;
            }
            current = current->next;
        }
        return head;
    }
    ListNode* doubleIt(ListNode* head) {
        /*revese the direction
        start multiply from new head
        then reverse back*/
        ListNode* prev = nullptr;
        ListNode* current = head;
        while (current){
            ListNode* next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        /*prev points to last */
        current = prev;
        prev = nullptr;
        int add = 0;
        while (current){
            ListNode* next = current->next;
            int new_val = (current->val*2)%10+add;
            add = (current->val*2)/10;
            current->val = new_val;
            current->next = prev;
            prev = current;
            current = next;
        }
        if(add){
            ListNode* additional = new(ListNode);
            additional->val = 1;
            additional->next = prev;
            return additional;
        }else{
            return prev;
        }
    }
    int gcd(int a, int b) {
        if(a<b){
            int temp = a;
            a = b % a;
            b = temp;

        }
        if(a != 0){
            while (b != 0) {
                int temp = b;
                b = a % b;
                a = temp;
            }
        }else{
            return b;
        }
        return a;
    }

    template<typename T>
    T* insertGreatestCommonDivisors(T* head) {
        T* current = head;
        try{
            while(current && current->next){
                  T* insert = new(T);
                  insert->val = gcd(current->val, current->next->val);
                  T* next = current->next;
                  current->next = insert;
                  insert->next = next;
                  current = next;
            }
        }
        catch(const char * e){
            std::cerr << "Exception: " << e << std::endl;
            /*handle memory leak if needed*/
        }
        return head;

    }
    int getDecimalValue(ListNode* head) {
        vector<int> bin;
        bin.reserve(30);
        ListNode* current = head;
        while(current){
            if(current->val == 0 && bin.size()){
                bin.push_back(current->val);
            }else{
                bin.push_back(current->val);
            }
            current = current->next;
        }
        int size = bin.size();
        int value = 0;
        for(int i = 0;i<size;i++){
            value += bin[i]<<(size-i-1);
        }
        return value;
    }
    ListNode* merge(ListNode* left,ListNode* right){
        if(left == nullptr && right == nullptr ){
            return nullptr;
        }else if(left == nullptr){
            return right;
        }else if(right == nullptr){
            return left;
        }
        ListNode * head;
        ListNode * current;
        if(left->val > right->val){
            head = right;
            current = right;
            right = right->next;
        }else{
            head = left;
            current = left;
            left = left->next;
        }
        while(left && right){
            if(left->val > right->val){
                current->next = right;
                right = right->next;
            }else{
                current->next = left;
                left = left->next;
            }
            current = current->next;
        }
        if(left){
            current->next = left;
        }else if(right){
            current->next = right;
        }

        return head;

    }
    ListNode* sortList(ListNode* head) {
        /*first count size and merge niegbors
        recursively split by 2 , until 1 to 1
        and merge , need to have merge function of sorted arrays
        recursion have two branches always */
        if(head && head->next){
            ListNode* left = head;
            ListNode* right = head;
            ListNode* prev = head;
            while(right && right->next){
                prev = left;
                left = left->next;
                right = right->next->next;
            }
            prev->next = nullptr;
            ListNode * l1 = sortList(head);
            ListNode * l2 = sortList(left);
            return merge (l1, l2);
        }

        return head;
    }
};
