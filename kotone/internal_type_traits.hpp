#ifndef KOTONE_INTERNAL_TYPE_TRAITS_HPP
#define KOTONE_INTERNAL_TYPE_TRAITS_HPP 1

#include <concepts>

namespace kotone {

template <typename T> concept number = std::is_arithmetic_v<T>;

template <typename T> concept signed_number = std::signed_integral<T> || std::floating_point<T>;

}  // namespace kotone

#endif  // KOTONE_INTERNAL_TYPE_TRAITS
