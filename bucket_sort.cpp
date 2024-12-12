/**
 * bucket_sort.cpp
 */

#include "bucket_sort.h"
#include <cmath>

template <typename T>
void bucket_sort(std::vector<T> &vector, int64_t bucket_size) {
    int64_t n = vector.size();
    if (n <= 1) return;

    if (bucket_size <= 0) {
        std::sort(vector.begin(), vector.end());
        return;
    }

    int64_t num_buckets = (n + bucket_size - 1) / bucket_size;
    std::vector<std::vector<T>> buckets(num_buckets);

    T min = vector.at(0);
    T max = vector.at(0);
    for (int64_t i = 1; i < vector.size(); i++) {
        if (vector.at(i) < min) min = vector.at(i);
        if (vector.at(i) > max) max = vector.at(i);
    }

    for (int64_t i = 0; i < vector.size(); i++) {
        int64_t index;
        if (std::is_floating_point_v<T>) {
            index = std::floor((vector.at(i) - min) * (n - 1) / (max - min) / bucket_size);
        } else {
            index = (vector.at(i) - min) * (n - 1) / (max - min) / bucket_size;
        }
        buckets.at(index).push_back(vector.at(i));
    }

    int64_t i = 0;
    for (auto &bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
        for (T value : bucket) {
            vector.at(i) = value;
            i++;
        }
    }
}

template <typename T>
void bucket_sort(std::vector<T> &vector) {
    int64_t n = std::distance(vector.begin(), vector.end());

    if (n < (1 << 21)) {
        bucket_sort(vector, 16);
    } else {
        int log_val = 0;
        while (n > 1) {
            log_val++;
            n /= 2;
        }
        bucket_sort(vector, 1LL << (log_val - 16));
    }
}
