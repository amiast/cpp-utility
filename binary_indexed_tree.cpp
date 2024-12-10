/**
 * binary_indexed_tree.cpp
 */

#include "binary_indexed_tree.h"
#include <stdexcept>
#include <sstream>

template <typename T>
BITree<T>::BITree(int n) {
    num_elements = n;
    tree = std::vector<T>(n + 1);
}

template <typename T>
void BITree<T>::update(int index, T value) {
    if (index <= 0 || index > num_elements){
        std::stringstream message;
        message << "Invalid BITree index=" << index;
        throw std::invalid_argument(message.str());
    }

    while (index <= num_elements) {
        tree.at(index) += value;
        index += index & -index;
    }
}

template <typename T>
T BITree<T>::query(int index) {
    T sum = 0;
    while (index > 0) {
        sum += tree.at(index);
        index -= index & -index;
    }
    return sum;
}

template <typename T>
int BITree<T>::size() {
    return num_elements;
}
