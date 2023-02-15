//
// Created by chenh on 2023/1/27.
//
#include "skiplist.h"
#include <iostream>

int main(){
skiplist<int, std::string> skiplist(6);
skiplist.insert_element(2, "two", true);
skiplist.insert_element(3, "three", true);
skiplist.insert_element(11, "eleven", true);
skiplist.insert_element(13, "thirteen", true);
skiplist.insert_element(17, "seventeen", true);
skiplist.insert_element(19, "nineteen", true);
skiplist.insert_element(19, "nineteen", true);

std::cout << "skiplist size:" << skiplist.size() << std::endl;

skiplist.dump();

// skiplist.load_file();

skiplist.search_element(13, true);
skiplist.search_element(18, true);


skiplist.display();

skiplist.delete_element(11, true);
skiplist.delete_element(17, true);

std::cout << "skiplist size:" << skiplist.size() << std::endl;

skiplist.display();
}