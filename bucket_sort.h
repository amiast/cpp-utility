/**
 * bucket_sort.h
 */

#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

template <typename Iterator>
void bucket_sort(Iterator begin, Iterator end, int64_t bucket_size);

template <typename Iterator>
void bucket_sort(Iterator begin, Iterator end);

#endif
