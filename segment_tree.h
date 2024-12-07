/**
 * segment_tree.h
 */

#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <functional>

template <typename T, typename U=T>
class SegmentTree {
    protected:
        std::vector<U> tree;
        int num_elements;
        std::function<U(U, U)> op;
        std::function<U(T)> eval;

        void add_nodes(const std::vector<T> &vector, int node, int begin, int end);
        U query(int node, int begin, int end, int left, int right);
        void update(int node, int begin, int end,int index, T val);

    public:
        SegmentTree(const std::vector<T> &vector, std::function<U(U, U)> operation, std::function<U(T)> evaluate);
        U query_range(int left, int right);
        void update(int index, T val);
};

#endif
