/**
 * union_find.h
 */

#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>
#include <unordered_map>

template <typename T>
class UnionFind {
    protected:
        std::vector<T> parent;
        std::vector<int> rank;
        std::unordered_map<T, int> index_of;
        int num_elements;

    public:
        UnionFind(const std::vector<T> &collection);
        T find(T val);
        void update_union(T val_1, T val_2);
        int size();
};

#endif
