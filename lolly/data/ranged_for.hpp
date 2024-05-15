
/** \file ranged_for.hpp
 *  \copyright GPLv3
 *  \details adapter from N() to ranged for clause in C++ 11
 *  \author jingkaimori
 *  \date   2024
 */

#pragma once

#include <type_traits>

namespace lolly {
namespace data {

template <typename T, typename= void>
struct has_operator_brackets : std::false_type {};
template <typename T>
struct has_operator_brackets<T, std::void_t<decltype (std::declval<T> ()[0])>>
    : std::true_type {};

template <class T, class= std::void_t<>> struct has_N : std::false_type {};
template <class T>
struct has_N<T, std::void_t<decltype (N (std::declval<T> ()))>>
    : std::is_same<decltype (N (std::declval<T> ())), int> {};

template <typename T>
using iterable_with_subscript=
    std::conjunction<has_N<T>, has_operator_brackets<T>>;

/**
 * @class iterator_lite
 * @brief A template class for iterators.
 * @tparam T The type of the underlying container of iterator.
 */
template <typename T> struct iterator_lite {
  static_assert (
      iterable_with_subscript<T>::value,
      "given type cannot be iterated according to N() and operator[].");
  int       i;
  const int n;
  T         container;
  iterator_lite (T c) : i (0), n (N (c)), container (c){};
  iterator_lite<T>& begin () { return *this; };
  std::nullptr_t    end () { return nullptr; };
  /**
   * @brief Move the iterator to the next element.
   */
  iterator_lite<T>& operator++ () {
    i++;
    return *this;
  };
  /**
   * @brief Returns the current element of the iterator
   */
  auto operator* () -> decltype (std::declval<T> ()[std::declval<int> ()]) {
    return container[i];
  };
  bool operator!= (std::nullptr_t) { return i < n; }
};

} // namespace data
} // namespace lolly
