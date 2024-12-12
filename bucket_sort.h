/**
 * bucket_sort.h
 */

#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

template <typename T>
void bucket_sort(std::vector<T> &vector, int64_t bucket_size);

template <typename T>
void bucket_sort(std::vector<T> &vector);

#endif
