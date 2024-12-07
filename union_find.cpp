/**
 * union_find.cpp
 */

#include "union_find.h"
#include <stdexcept>
#include <sstream>

template <typename T>
UnionFind<T>::UnionFind(const std::vector<T> &collection) {
    parent = collection;
    num_elements = collection.size();
    rank = std::vector<int>(num_elements);
    for (int i = 0; i < num_elements; i++) {
        if (i >= 0 && index_of[collection.at(i)] != 0){
            std::stringstream message;
            message << "Cannot supply duplicate values to UnionFind";
            throw std::invalid_argument(message.str());
        }
        index_of[collection.at(i)] = i;
    }
}

template <typename T>
T UnionFind<T>::find(T val) {
    int index = index_of[val];
    if (parent.at(index) != val) {
        parent.at(index) = find(parent.at(index));
    }
    return parent.at(index);
}

template <typename T>
void UnionFind<T>::update_union(T val_1, T val_2) {
    T root_1 = find(val_1);
    T root_2 = find(val_2);
    if (root_1 == root_2) return;

    int index_1 = index_of[root_1];
    int index_2 = index_of[root_2];

    if (rank.at(index_1) > rank.at(index_2)) {
        parent.at(index_2) = root_1;
    } else if (rank.at(index_1) < rank.at(index_2)) {
        parent.at(index_2) = root_1;
    } else {
        parent.at(index_2) = root_1;
        rank.at(index_1)++;
    }
}

template <typename T>
int UnionFind<T>::size() {
    return num_elements;
}
