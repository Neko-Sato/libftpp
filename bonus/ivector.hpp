/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:14:25 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:14:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <cstddef>
#include <cmath>

template <typename T, std::size_t D>
class IVector {
public:
  IVector() = default;
  template <typename ...Ts>
  IVector(Ts &&...args) requires (sizeof...(Ts) == D);

  template <typename U>
  operator IVector<U, D>() const;

  T &operator[] (std::size_t index);
  T const &operator[] (std::size_t index) const;
  bool operator==(IVector<T, D> const &rhs) const;
  bool operator!=(IVector<T, D> const &rhs) const;

  #define DECL_VECTOR_OP(OP) \
  template <typename U> \
  auto operator OP(IVector<U, D> const &rhs) const;

  DECL_VECTOR_OP(+)
  DECL_VECTOR_OP(-)
  DECL_VECTOR_OP(*)
  DECL_VECTOR_OP(/)

  #undef DECL_VECTOR_OP

  #define DECL_SCALAR_OP(OP) \
  template <typename U> \
  auto operator OP(U const &rhs) const;

  DECL_SCALAR_OP(+)
  DECL_SCALAR_OP(-)
  DECL_SCALAR_OP(*)
  DECL_SCALAR_OP(/)

  #undef DECL_SCALAR_OP

  auto length() const;
  auto normalize() const;
  template <typename U>
  auto dot(IVector<U, D> const &other) const;

private:
  std::array<T, D> _data;
};

template <typename T, std::size_t D>
template <typename ...Ts>
IVector<T, D>::IVector(Ts &&...args) requires (sizeof...(Ts) == D)
  : _data{static_cast<T>(std::forward<Ts>(args))...} {
}

template <typename T, std::size_t D>
template <typename U>
IVector<T, D>::operator IVector<U, D>() const {
  return [&]<std::size_t ...Is>(std::index_sequence<Is...>) {
    return IVector<U, D>(_data[Is]...);
  }(std::make_index_sequence<D>{});
}

template <typename T, std::size_t D>
bool IVector<T, D>::operator==(IVector<T, D> const &rhs) const {
  return _data == rhs._data;
}

template <typename T, std::size_t D>
bool IVector<T, D>::operator!=(IVector<T, D> const &rhs) const {
  return _data != rhs._data;
}

template <typename T, std::size_t D>
T &IVector<T, D>::operator[](std::size_t index) {
  return _data[index];
}

template <typename T, std::size_t D>
T const &IVector<T, D>::operator[](std::size_t index) const {
  return _data[index];
}

#define DEFINE_VECTOR_OP(OP) \
template <typename T, std::size_t D> \
template <typename U> \
auto IVector<T, D>::operator OP(IVector<U, D> const &rhs) const { \
  return [&]<std::size_t ...Is>(std::index_sequence<Is...>) { \
    return IVector<std::common_type_t<T, U>, D>((_data[Is] OP rhs[Is])...); \
  }(std::make_index_sequence<D>{}); \
}

DEFINE_VECTOR_OP(+)
DEFINE_VECTOR_OP(-)
DEFINE_VECTOR_OP(*)
DEFINE_VECTOR_OP(/)

#undef DEFINE_VECTOR_OP

#define DEFINE_SCALAR_OP(OP) \
template <typename T, std::size_t D> \
template <typename U> \
auto IVector<T, D>::operator OP(U const &rhs) const { \
  return [&]<std::size_t ...Is>(std::index_sequence<Is...>) { \
    return IVector<std::common_type_t<T, U>, D>((_data[Is] OP rhs)...); \
  }(std::make_index_sequence<D>{}); \
}

DEFINE_SCALAR_OP(+)
DEFINE_SCALAR_OP(-)
DEFINE_SCALAR_OP(*)
DEFINE_SCALAR_OP(/)

#undef DEFINE_SCALAR_OP

template <typename T, std::size_t D>
auto IVector<T, D>::length() const {
  return std::sqrt(dot(*this));
}

template <typename T, std::size_t D>
auto IVector<T, D>::normalize() const {
  return *this / length();
}

template <typename T, std::size_t D>
template <typename U>
auto IVector<T, D>::dot(IVector<U, D> const &other) const {
  return [&]<std::size_t ...Is>(std::index_sequence<Is...>) {
    return ((_data[Is] * other[Is]) + ... + std::common_type_t<T, U>{});
  }(std::make_index_sequence<D>{});
}
