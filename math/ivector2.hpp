/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:56:07 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/02 22:16:07 by hshimizu         ###   ########.fr       */
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
  return [this]<std::size_t ...Is>(std::index_sequence<Is...>) {
    return IVector<U, D>(static_cast<U>(_data[Is])...);
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
  return [this, &other]<std::size_t ...Is>(std::index_sequence<Is...>) {
    return ((_data[Is] * other[Is]) + ... + std::common_type_t<T, U>{});
  }(std::make_index_sequence<D>{});
}

template <typename T>
class IVector2 : public IVector<T, 2>{
public:
  T &x, &y;
  
  IVector2();
  IVector2(IVector<T, 2> const &vec);
  IVector2(T const &x_val, T const &y_val);

  float length() const;
  IVector2<float> normalize() const;
  float dot(IVector2<T> const &other) const;
  IVector2<float> cross() const;

  #define DECL_IVECTOR2_OP(OP) \
    IVector2<T> operator OP(IVector2<T> const &rhs) const; \
    IVector2<T> operator OP(T const &rhs) const;

  DECL_IVECTOR2_OP(+)
  DECL_IVECTOR2_OP(-)
  DECL_IVECTOR2_OP(*)
  DECL_IVECTOR2_OP(/)

  #undef DECL_IVECTOR2_OP
};

template <typename T>
IVector2<T>::IVector2()
  : IVector<T, 2>(), x((*this)[0]), y((*this)[1]) {
}

template <typename T>
IVector2<T>::IVector2(IVector<T, 2> const &vec)
  : IVector<T, 2>(vec), x((*this)[0]), y((*this)[1]) {
}

template <typename T>
IVector2<T>::IVector2(T const &x_val, T const &y_val)
  : IVector<T, 2>(x_val, y_val), x((*this)[0]), y((*this)[1]) {
}

template <typename T>
float IVector2<T>::length() const {
  return static_cast<IVector<float, 2>>(*this).length();
}

template <typename T>
IVector2<float> IVector2<T>::normalize() const {
  return static_cast<IVector<float, 2>>(*this).normalize();
}

template <typename T>
float IVector2<T>::dot(IVector2<T> const &other) const {
  return static_cast<IVector<float, 2>>(*this).dot(other);
}

template <typename T>
IVector2<float> IVector2<T>::cross() const {
  return IVector2<float>(0, 0);
}

#define DEFINE_IVECTOR2_OP(OP) \
template <typename T> \
IVector2<T> IVector2<T>::operator OP(IVector2<T> const &rhs) const { \
  return IVector<T, 2>::operator OP(static_cast<IVector<T, 2>>(rhs)); \
} \
template <typename T> \
IVector2<T> IVector2<T>::operator OP(T const &rhs) const { \
  return IVector<T, 2>::operator OP(rhs); \
}

DEFINE_IVECTOR2_OP(+)
DEFINE_IVECTOR2_OP(-)
DEFINE_IVECTOR2_OP(*)
DEFINE_IVECTOR2_OP(/)

#undef DEFINE_IVECTOR2_OP
