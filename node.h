//
// Created by chenh on 2022/11/20.
//

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

template <typename K, typename V>
class Node {
public:
    Node<K, V> **next;
    int level;
    Node() {}
    Node(K k, V v, int);
    K get_key() const;
    V get_value() const;
    void set_value(V);
    ~Node();

private:
    K key;
    V value;

};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level){
    this->key = k;
    this->value = v;
    this->level = level;

    this->next = new Node<K, V>*[level + 1];

    memset(this->next, 0, sizeof(Node<K,V>*) * (level + 1));
    //std::fill(this->next, this->next+level+1, 0);
}

template<typename K, typename V>
Node<K, V>::~Node(){
    delete []next;
}

template<typename K, typename V>
K Node<K, V>::get_key() const{
    return this-> key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return this-> value;
}

template<typename K, typename V>
void Node<K, V>::set_value(V value){
    this->value = value;
}

#endif //SKIPLIST_NODE_H
