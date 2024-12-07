/**
 * segment_tree.h
 */

#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <sstream>

template <typename T, typename U=T>
class SegmentTree {
    protected:
        std::vector<U> tree;
        int num_elements;
        std::function<U(U, U)> op;
        std::function<U(T)> eval;

        void add_nodes(const std::vector<T> &vector, int node, int begin, int end) {
            if (begin == end) {
                tree.at(node) = eval(vector.at(begin));
            } else {
                int mid = (begin + end) / 2;
                add_nodes(vector, node * 2 + 1, begin, mid);
                add_nodes(vector, node * 2 + 2, mid + 1, end);
                tree.at(node) = op(tree.at(node * 2 + 1), tree.at(node * 2 + 2));
            }
        }

        U query(int node, int begin, int end, int left, int right) {
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

        void update(int node, int begin, int end,int index, T val) {
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

    public:
        SegmentTree(const std::vector<T> &vector, std::function<U(U, U)> operation, std::function<U(T)> evaluate) {
            num_elements = vector.size();
            op = operation;
            eval = evaluate_function;
            tree = std::vector<U>(num_elements * 3);
            add_nodes(vector, 0, 0, num_elements - 1);
        }

        U query_range(int left, int right) {
            if (left < 0) {
                std::stringstream message;
                message << "Invalid range with left=" << left;
                throw std::invalid_argument(message.str());
            }
            if (left > right) {
                std::stringstream message;
                message << "Invalid range with left=" << left << " and right=" << right;
                throw std::invalid_argument(message.str());
            }
            if (right >= num_elements) {
                std::stringstream message;
                message << "Invalid range with right=" << right;
                throw std::invalid_argument(message.str());
            }
            return query(0, 0, num_elements - 1, left, right);
        }

        void update(int index, T val) {
            if (index < 0 || 0 >= num_elements) {
                std::stringstream message;
                message << "Invalid range with index=" << index;
                throw std::invalid_argument(message.str());
            }
            update(0, 0, num_elements - 1, index, val);
        }
};

#endif
