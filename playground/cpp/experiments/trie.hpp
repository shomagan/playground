#ifndef TRIE_HPP
#define TRIE_HPP
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include "common.hpp"
#include <string>
#include <memory>
using namespace std;
class Trie {
public:
    struct Node{
        char val;
        bool end;
        unordered_map<char, unique_ptr<Node> > map;
        Node():val('\0'), end(false){};
        Node(char _val): val(_val), end(false){};
    };
    unique_ptr<Node> empty;
    Trie(){
       empty = make_unique<Node>();
       empty->end = true;
    }
    void insert(string word,  int index, Node* node){
        if(word.length() > index){
            if(node->map.find(word[index]) == node->map.end()){
                unique_ptr<Node> node_new = make_unique<Node>(word[index]);
                node->map[word[index]] = std::move(node_new);
            }
            if(word.length()-1 == index){
                node->map[word[index]]->end = true;
            }
            insert(word, index+1, node->map[word[index]].get());
        }
    }
    void insert(string word) {
        insert(word, 0, empty.get());
    }
    bool search(string word,  int index, Node* node, bool full){
        if(word.length() > index){
            if(node->map.find(word[index]) == node->map.end()){
                return false;
            }else{
                return search(word, index+1, node->map[word[index]].get(), full);
            }
        }else{
            if(!full || (node->end)){
                return true;
            }
        }
        return false;
    }

    bool search(string word) {
        return search(word, 0, empty.get(), true);
    }

    bool startsWith(string prefix) {
        return search(prefix, 0, empty.get(), false);
    }
};
#endif /* TRIE_HPP */
