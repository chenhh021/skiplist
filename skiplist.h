#include "node.h"
#include <string>
#include <cstring>
#include <fstream>
#include <mutex>
#include <iostream>
#include <algorithm>
#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

std::mutex mtx;
#define STORE_FILE "store/dumpFile"

template <typename K, typename V>
class skiplist {
public:
    skiplist(int);
    ~skiplist();
    int get_random_level();
    Node <K, V>* create_node(K, V, int);
    int insert_element(K, V, bool display = false);
    int delete_element(K, bool display = false);
    bool search_element(K, bool display = false);
    void display();
    void dump();
    void load();
    int size();

private:
    void get_data_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

private:
    int _max_level{};
    int _skip_list_level{};
    int _element_count{};
    Node<K, V> *_header;
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    std::string delimiter = ":";
};

template<typename K, typename V>
skiplist<K, V>::skiplist(int max_level) {
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
}

template<typename K, typename V>
int skiplist<K, V>::get_random_level() {
    int k = 1;
    while(rand()%2){
        ++k;
    }
    return k < _max_level ? k : _max_level;
}

template<typename K, typename V>
Node<K,V>* skiplist<K, V>::create_node(const K k, const V v, int level){
    auto *node = new Node<K, V>(k, v, level);
    return node;
}

template<typename K, typename V>
int skiplist<K, V>::insert_element(const K key, const V value, bool display) {
    mtx.lock();

    Node<K, V> *cur = this->_header;
    Node<K, V> *pre[_max_level + 1];
    memset(pre, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    for(int i = _skip_list_level; i>= 0; i--){
        while(cur->next[i] != NULL && cur->next[i]->get_key() < key){
            cur = cur -> next[i];
        }
        pre[i] = cur;
    }

    cur = cur->next[0];

    if(cur != NULL && cur->get_key() == key){
        if(display)
            std::cout << "key: " << key << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }

    if(cur == NULL || cur->get_key() != key){
        int random_level = get_random_level();

        if(random_level > _skip_list_level){
            for(int i = _skip_list_level+1; i < random_level+1; ++i){
                pre[i] = _header;
            }
            _skip_list_level = random_level;
        }

        Node<K, V>* inserted_node = create_node(key, value, random_level);

        for(int i=0; i <= random_level; ++i){
            inserted_node -> next[i] = pre[i] -> next[i];
            pre[i]->next[i] = inserted_node;
        }
        if(display)
            std::cout << "Insertion succeed. key:" << key << ", value:" << value << std::endl;
        ++_element_count;
    }
    mtx.unlock();
    return 0;
}

template<typename K, typename V>
int skiplist<K, V>::delete_element(K key, bool display) {
    mtx.lock();
    Node<K, V> *cur = this->_header;
    Node<K, V> *pre[_max_level+1];

    for(int i = _skip_list_level; i>= 0; --i){
        while(cur->next[i] != NULL && cur->next[i]->get_key() < key){
            cur = cur->next[i];
        }
        pre[i] = cur;
    }

    cur = cur->next[0];
    if(cur != NULL && cur->get_key() == key) {
        for (int i = 0; i <= _skip_list_level; ++i) {
            if (pre[i]->next[i] != cur) break;
            pre[i]->next[i] = cur->next[i];
        }
        delete cur;

        while (_skip_list_level > 0 && _header->next[_skip_list_level] == NULL) {
            --_skip_list_level;
        }

        if(display)
            std::cout << "Delete key:" << key << " completed"<< std::endl;
        _element_count--;
    } else{
        mtx.unlock();
        if(display)
            std::cout << "Delete key failed. Key:" << key << "doesn't exist."<<std::endl;
        return 1;
    }
    mtx.unlock();
    return 0;
}

template<typename K, typename V>
bool skiplist<K, V>::search_element(K key, bool display) {
    if(display)
        std::cout << "search element with key:" << key << std::endl;

    Node<K, V> *cur = this->_header;
    for(int i = _skip_list_level; i >= 0; --i){
        while(cur != NULL && cur->get_key() < key){
            cur = cur->next[i];
        }
    }

    if(cur != NULL && cur->get_key() == key){
        if(display)
            std::cout << "element found, value: " << cur->get_value() << std::endl;
        return true;
    }

    if(display)
        std::cout << "element not found" << std::endl;
    return false;
}

template<typename K, typename V>
void skiplist<K, V>::display() {

    std::cout << "\n*****skip list*****"<<"\n";
    for (int i = 0; i <= _skip_list_level; i++) {
        Node<K, V> *cur = this->_header->next[i];
        std::cout << "Level " << i << ": ";
        while (cur != NULL) {
            std::cout << cur->get_key() << ":" << cur->get_value() << "->";
            cur = cur->next[i];
        }
        std::cout << "end" << std::endl;
    }
}

template<typename K, typename V>
void skiplist<K, V>::dump() {
    std::cout << "start dump data" << std::endl;
    _file_writer.open(STORE_FILE);
    auto *cur = _header->next[0];

    while(cur != NULL){
        _file_writer << cur->get_key() << ":" << cur->get_value() << std::endl;
        cur = cur->next[0];
    }

    _file_writer.flush();
    _file_writer.close();
}

template<typename K, typename V>
void skiplist<K, V>::load() {

    _file_reader.open(STORE_FILE);
    std::cout << "start load data" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_data_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }

// only support <int, string> now
        insert_element(atoi(key->c_str()), *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    _file_reader.close();
}

template<typename K, typename V>
void skiplist<K, V>::get_data_from_string(const std::string& str, std::string* key, std::string* value) {

    if(!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1, str.length());
}

template<typename K, typename V>
bool skiplist<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

template<typename K, typename V>
int skiplist<K, V>::size(){
    return _element_count;
}

template<typename K, typename V>
skiplist<K, V>::~skiplist(){
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    delete _header;
}
#endif //SKIPLIST_SKIPLIST_H
