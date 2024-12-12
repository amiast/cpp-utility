/**
 * bucket_sort.cpp
 */

#include "bucket_sort.h"
#include <vector>
#include <cmath>

template <typename Iterator>
void bucket_sort(Iterator begin, Iterator end, int64_t bucket_size) {
    int64_t n = std::distance(begin, end);
    if (n <= 1) return;

    if (bucket_size <= 0) {
        std::sort(begin, end);
        return;
    }

    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<std::vector<T>> buckets((n + bucket_size - 1) / bucket_size);

    T min = *begin;
    T max = *begin;
    auto iter = begin;
    iter++;
    while (iter != end) {
        if (*iter < min) min = *iter;
        if (*iter > max) max = *iter;
        iter++;
    }

    for (auto iter = begin; iter != end; iter++) {
        int64_t index;
        if (std::is_floating_point<T>::value) {
            index = std::floor((*iter - min) * (n - 1) / (max - min));
        } else {
            index = (*iter - min) * (n - 1) / (max - min) / bucket_size;
        }
        buckets.at(index).push_back(*iter);
    }

    iter = begin;
    for (auto &bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
        for (auto &value : bucket) {
            *iter = value;
            iter++;
        }
    }
}

template <typename Iterator>
void bucket_sort(Iterator begin, Iterator end) {
    int64_t n = std::distance(begin, end);

    if (n < (1 << 21)) {
        bucket_sort(begin, end, 16);
    } else {
        int log_val = 0;
        while (n > 1) {
            log_val++;
            n /= 2;
        }
        bucket_sort(begin, end, 1LL << (log_val - 16));
    }
}
