#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include "common.hpp"
/*use std vector*/
typedef long long ll;
using namespace std;
class Node{
public:
   int val;
   vector<Node*> neighbors;
   Node(){
      val = 0;
      neighbors = vector<Node*>();
   }
   Node(int _val){
      val = _val;
      neighbors = vector<Node*>();
   }
   Node(int _val, const vector<Node*>& _neighbors){
      val = _val;
      neighbors = _neighbors;
   }

};
class Graph {
public:
   Graph() {};
   ~Graph() = default;
    const int MOD = 1e9 + 7;
    int find_shorter(vector<pair <ll, ll>>& shortest_time){
        ll min = LLONG_MAX;
        int result = -1;
        for (int i = 0; i < shortest_time.size(); i++){
            if(!shortest_time[i].second){
                if(shortest_time[i].first < min){
                    min = shortest_time[i].first;
                    result = i;
                }
            }
        }
        return result;
    }
    void handle_node(vector<vector<int>>& roads, vector<pair <ll, ll>>& shortest_time, int node, vector<int>& ways){
        for(int i = 0; i < roads.size(); i++){
            if (roads[i][0] == node && !shortest_time[roads[i][1]].second){
                int neib = roads[i][1];
                if(shortest_time[node].first + roads[i][2] < shortest_time[neib].first){
                    shortest_time[neib].first = shortest_time[node].first + roads[i][2];
                    ways[neib] = ways[node];
                }else if(shortest_time[node].first + roads[i][2] == shortest_time[neib].first){
                    ways[neib] = (ways[neib] + ways[node]) % MOD;
                }
            }else if(roads[i][1] == node && !shortest_time[roads[i][0]].second){
                int neib = roads[i][0];
                if(shortest_time[node].first + roads[i][2] < shortest_time[neib].first){
                    shortest_time[neib].first = shortest_time[node].first + roads[i][2];
                    ways[neib] = ways[node];
                }else if(shortest_time[node].first + roads[i][2] == shortest_time[neib].first){
                    ways[neib] = (ways[neib] + ways[node]) % MOD;
                }
            }
        }
        shortest_time[node].second = 1;
    }
    int countPaths(int n, vector<vector<int>>& roads) {
        /*we can find shortest time with Dijkstra algorithm*/
        /*dp */
        vector<int> ways(n);
        ways[0] = 1;
        vector<pair<ll, ll>> shortest_time(n, {LLONG_MAX, 0});
        shortest_time[0].first = 0;
        shortest_time[0].second = 1;
        int next_short = 0;
        for (int i = 0;i < n; i++){
            handle_node(roads, shortest_time, next_short, ways);
            next_short = find_shorter(shortest_time);
            if(next_short == -1){
                break;
            }
        }
        return ways[n-1];
    };
    int countPaths_solution(int n, vector<vector<int>>& roads) {
        vector<vector<pair<ll, ll>>> graph(n);
        for(auto& road: roads) {
            ll u = road[0], v = road[1], time = road[2];
            graph[u].push_back({v, time});
            graph[v].push_back({u, time});
        }
        return dijkstra(graph, n, 0);
    }
    int dijkstra(const vector<vector<pair<ll, ll>>>& graph, int n, int src) {
        vector<ll> dist(n, LONG_MAX);
        vector<ll> ways(n);
        ways[src] = 1;
        dist[src] = 0;
        priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<>> minHeap;
        minHeap.push({0, 0}); // dist, src
        while (!minHeap.empty()) {
            auto[d, u] = minHeap.top(); minHeap.pop();
            if (d > dist[u]) continue; // Skip if `d` is not updated to latest version!
            for(auto [v, time] : graph[u]) {
                if (dist[v] > d + time) {
                    dist[v] = d + time;
                    ways[v] = ways[u];
                    minHeap.push({dist[v], v});
                } else if (dist[v] == d + time) {
                    ways[v] = (ways[v] + ways[u]) % MOD;
                }
            }
        }
        return ways[n-1];
    }
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        /* mix between bfs and bellman - ford algorithm
           it itterates each edge based on prev flight value and update only after cycle left
           if it reaches dst at any point we lower value of the price
        */
        /*[f,t,p]*/
        int price = -1;
        /*keep two prices first - for current flight, second - based on prev flight of info we have
          every time we calculate first only based on second value which we saved every cycle */
        vector<pair <int, int> > d_p(n, {INT_MAX , INT_MAX }); /*destination price*/
        d_p[src] = {0, 0};
        for (int i = 0; i < k+1 && i < n; i++){
            /*update graph similar to bellman ford algorithm, but not only n-1 times , less than k + 1 times as well*/
            for (int j = 0; j < flights.size(); j++){
                if(d_p[flights[j][0]].second < INT_MAX  && d_p[flights[j][1]].second > d_p[flights[j][0]].second + flights[j][2]){
                    if(d_p[flights[j][0]].second + flights[j][2] < d_p[flights[j][1]].first){
                        d_p[flights[j][1]].first = d_p[flights[j][0]].second + flights[j][2];
                        if(flights[j][1] == dst){
                            price = d_p[flights[j][1]].first;
                        }
                    }
                }
            }
            for (int j = 0; j < d_p.size(); j++){
                d_p[j].second = d_p[j].first;
            }
        }
        return price;
    }
    Node* clone(Node* node, unordered_map<Node*, Node*>& table){
        Node* node_new = new Node(node->val);
        table[node] = node_new;
        vector<Node*> neighbors;
        for(auto& i: node->neighbors){
            if(table.find(i) != table.end()){
                neighbors.push_back(table[i]);
            }else{
                neighbors.push_back(clone(i, table));
            }
        }
        node_new->neighbors = neighbors;
        return node_new;
    }
/*we need to copy value create a new pointer and add  it to each other*/
    Node* cloneGraph(Node* node) {
        Node* node_new = nullptr;
        if(node){
            unordered_map<Node*, Node*> node_table;/*old and new pair*/
            node_new = clone(node, node_table);
        }
        return node_new;

    }
};

#endif // GRAPH_HPP
