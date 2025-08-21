/**
 * binary_indexed_tree.h
 */

#ifndef BINARY_INDEXED_TREE_H
#define BINARY_INDEXED_TREE_H

#include <vector>

template <typename T>
class BITree {
    protected:
        int num_elements;
        std::vector<T> tree;

    public:
        BITree(int n);
        void update(int index, T value);
        T query(int index);
        int size();
};

#endif
