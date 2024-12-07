/**
 * segment_tree.cpp
 */

#include "segment_tree.h"
#include <stdexcept>
#include <sstream>

template <typename T, typename U>
void SegmentTree<T, U>::add_nodes(const std::vector<T> &vector, int node, int begin, int end) {
    if (begin == end) {
        tree.at(node) = eval(vector.at(begin));
    } else {
        int mid = (begin + end) / 2;
        add_nodes(vector, node * 2 + 1, begin, mid);
        add_nodes(vector, node * 2 + 2, mid + 1, end);
        tree.at(node) = op(tree.at(node * 2 + 1), tree.at(node * 2 + 2));
    }
}

template <typename T, typename U>
U SegmentTree<T, U>::query(int node, int begin, int end, int left, int right) {
    if (left <= begin && end <= right) {
        return tree.at(node);
    }
    int mid = (begin + end) / 2;
    if (right <= mid) {
        return query(node * 2 + 1, begin, mid, left, right);
    } else if (left > mid) {
        return query(node * 2 + 2, mid + 1, end, left, right);
    } else {
        U left_query = query(node * 2 + 1, begin, mid, left, right);
        U right_query = query(node * 2 + 2, mid + 1, end, left, right);
        return op(left_query, right_query);
    }
}

template <typename T, typename U>
void SegmentTree<T, U>::update(int node, int begin, int end,int index, T val) {
    if (begin == end) {
        tree.at(node) = eval(val);
    } else {
        int mid = (begin + end) / 2;
        if (begin <= index && index <= mid) {
            update(node * 2 + 1, begin, mid, index, val);
        } else {
            update(node * 2 + 2, mid + 1, end, index, val);
        }
        tree.at(node) = op(tree.at(node * 2 + 1), tree.at(node * 2 + 2));
    }
}

template <typename T, typename U>
SegmentTree<T, U>::SegmentTree(const std::vector<T> &vector, std::function<U(U, U)> operation, std::function<U(T)> evaluate) {
    num_elements = vector.size();
    op = operation;
    eval = evaluate;
    tree = std::vector<U>(num_elements * 3);
    add_nodes(vector, 0, 0, num_elements - 1);
}

template <typename T, typename U>
U SegmentTree<T, U>::query_range(int left, int right) {
    if (left > right) {
        std::stringstream message;
        message << "Invalid range with left=" << left << " and right=" << right;
        throw std::invalid_argument(message.str());
    }
    return query(0, 0, num_elements - 1, left, right);
}

template <typename T, typename U>
void SegmentTree<T, U>::update(int index, T val) {
    if (index < 0 || 0 >= num_elements) {
        std::stringstream message;
        message << "Invalid range with index=" << index;
        throw std::invalid_argument(message.str());
    }
    update(0, 0, num_elements - 1, index, val);
}
