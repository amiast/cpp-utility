#ifndef KOTONE_INTERNAL_TYPE_TRAITS_HPP
#define KOTONE_INTERNAL_TYPE_TRAITS_HPP 1

#include <concepts>

namespace kotone {

template <typename T> concept number = std::is_arithmetic_v<T>;

template <typename T> concept signed_number = std::signed_integral<T> || std::floating_point<T>;

template <typename mint> concept compatible_modint = requires(mint a, mint b, int64_t x) {
    { mint::mod() } -> std::convertible_to<int>;
    mint{};
    mint{1};
    mint{x};

    { a + b }       -> std::same_as<mint>;
    { a - b }       -> std::same_as<mint>;
    { -a }          -> std::same_as<mint>;
    { a * b }       -> std::same_as<mint>;
    { a / b }       -> std::same_as<mint>;
    { a.inv() }     -> std::same_as<mint>;
    { a.pow(x) }    -> std::same_as<mint>;

    { a += b }      -> std::same_as<mint&>;
    { a -= b }      -> std::same_as<mint&>;
    { a *= b }      -> std::same_as<mint&>;
    { a /= b }      -> std::same_as<mint&>;

    { a == b }      -> std::convertible_to<bool>;
    { a != b }      -> std::convertible_to<bool>;
    { a.val() }     -> std::convertible_to<int>;
};

template <typename T> concept additive = requires(T a, T b) {
    T{};
    { a + b }       -> std::same_as<T>;
    { a - b }       -> std::same_as<T>;
    { -a }          -> std::same_as<T>;
    { a == b }      -> std::convertible_to<bool>;
    { a != b }      -> std::convertible_to<bool>;
};

template <typename T> concept mutable_additive = (
    additive<T>
    && requires(T a, T b) {
        { a += b }  -> std::same_as<T&>;
        { a -= b }  -> std::same_as<T&>;
        { ++a }     -> std::same_as<T&>;
        { --a }     -> std::same_as<T&>;
        { a++ }     -> std::same_as<T>;
        { a-- }     -> std::same_as<T>;
    }
);

}  // namespace kotone

#endif  // KOTONE_INTERNAL_TYPE_TRAITS
