/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:56:07 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:15:52 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "bonus/ivector.hpp"

template <typename T>
class IVector2 {
public:
  T x, y;
  
  IVector2();
  IVector2(IVector<T, 2> const &vec);
  IVector2(T const &x_val, T const &y_val);

  float length() const;
  IVector2<float> normalize() const;
  float dot(IVector2<T> const &other) const;
  IVector2<float> cross() const;

  bool operator==(IVector2<T> const &rhs) const;
  bool operator!=(IVector2<T> const &rhs) const;

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
  : IVector2(0, 0) {
}

template <typename T>
IVector2<T>::IVector2(IVector<T, 2> const &vec)
  : x(vec[0]), y(vec[1]) {
}

template <typename T>
IVector2<T>::IVector2(T const &x_val, T const &y_val)
  : x(x_val), y(y_val) {
}

template <typename T>
float IVector2<T>::length() const {
  return IVector<float, 2>(x, y).length();
}

template <typename T>
IVector2<float> IVector2<T>::normalize() const {
  return IVector<float, 2>(x, y).normalize();
}

template <typename T>
float IVector2<T>::dot(IVector2<T> const &other) const {
  return IVector<float, 2>(x, y).dot(IVector<float, 2>(other.x, other.y));
}

template <typename T>
IVector2<float> IVector2<T>::cross() const {
  return IVector2<float>(0, 0);
}

template <typename T>
bool IVector2<T>::operator==(IVector2<T> const &rhs) const {
  return IVector<T, 2>(x, y) == IVector<T, 2>(rhs.x, rhs.y);
}

template <typename T>
bool IVector2<T>::operator!=(IVector2<T> const &rhs) const {
  return IVector<T, 2>(x, y) != IVector<T, 2>(rhs.x, rhs.y);
}

#define DEFINE_IVECTOR2_OP(OP) \
template <typename T> \
IVector2<T> IVector2<T>::operator OP(IVector2<T> const &rhs) const { \
  return IVector<T, 2>(x, y) OP IVector<T, 2>(rhs.x, rhs.y); \
} \
template <typename T> \
IVector2<T> IVector2<T>::operator OP(T const &rhs) const { \
  return IVector<T, 2>(x, y) OP rhs; \
}

DEFINE_IVECTOR2_OP(+)
DEFINE_IVECTOR2_OP(-)
DEFINE_IVECTOR2_OP(*)
DEFINE_IVECTOR2_OP(/)

#undef DEFINE_IVECTOR2_OP
