#ifndef QUEUE_DEQUE_HPP
#define QUEUE_DEQUE_HPP
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>
class QueueDeque {
public:
   int last = 0;
   std::priority_queue<int, std::vector<int>, std::greater<int>> stream;

   QueueDeque(int k, std::vector<int>& nums){
      int i = 0;
      last = k;
      for (int i: nums){
         if (stream.size() == k){
            if(stream.top() < i){
               stream.pop();
               stream.push(i);
            }
         }else{
            stream.push(i);
         }
      }
   };
   int add(int val) {

      if (stream.size() == last){
         if(stream.top() < val){
            stream.pop();
            stream.push(val);
         }
      }else{
         stream.push(val);
      }
      return stream.top();
   }
   ~QueueDeque(){};
   std::vector<int> deckRevealedIncreasing(std::vector<int>& deck) {
      /*Input: deck = [7,6,5,1,2,3,4]
      Output: [1,6,2,5,3,7,4]*/
      /*thats interesting
      every odd number seems to be in increasing order
      first we can sort
      what we do -
      take first card
      move next to last-
      1,6,2,5,3,7,4 ->
      2 5 3 7 4 6 : 1 ->
      3 7 4 6 5 : 2 ->
      4 6 5 7: 3 ->
      5 7 6: 4 ->
      6 7: 5 ->
      7: 6->
      : 7
      then we can do back logic?
      move last to first - not swap
      put next to first
      7:
      6: 7 ->
      5: 6 7 ->
      4: 5 7 6 ->
      3: 4 6 5 7 ->
      2: 3 7 4 6 5
      */
      std::sort(deck.begin(), deck.end(),
                [](const auto& a, const auto& b){
                     return a > b;
                });
      std::deque<int> result_deque;
      std::vector<int> result(deck.size());
      for (int i : deck){
         if (!result_deque.empty()){
            int last = result_deque.back();
            result_deque.pop_back();
            result_deque.push_front(last);
         }
         result_deque.push_front(i);
      }
      for (int i = 0;i < deck.size(); i++){
         result[i] = result_deque[i];
      }
      return result;
   }
   std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
      std::vector<pair<int,int>> points_t= {{1,0},{2,0},{3,0}};

      std::sort(points_t.begin(), points_t.end(), [](const auto& a, const auto& b){
         return (a.first*a.first + a.second*a.second) > (b.first*b.first + b.second*b.second);
      } );

      auto quad = [](std::pair<int, int>& a, std::pair<int, int>& b){
         return ((a.first*a.first + a.second*a.second)>(b.first*b.first + b.second*b.second));
      };
      std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, decltype(quad)> sorted(quad);
      for(std::vector<int>& point: points){
         std::pair<int,int> temp(point[0],point[1]);
         sorted.push(temp);
      }
      std::vector<std::vector<int>> result(k);
      for (int i = 0;i < k;i++){
         std::pair<int,int> temp_pair = sorted.top();
         sorted.pop();
         std::vector<int> temp = {temp_pair.first, temp_pair.second};
         result[i] = temp;
      }
      return result;
   }
};
#endif /* QUEUE_DEQUE_HPP */
