/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:56:07 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:16:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "bonus/ivector.hpp"

template <typename T>
class IVector3 {
public:
  T x, y, z;
  
  IVector3();
  IVector3(IVector<T, 3> const &vec);
  IVector3(T const &x_val, T const &y_val, T const &z_val);

  float length() const;
  IVector3<float> normalize() const;
  float dot(IVector3<T> const &other) const;
  IVector3<float> cross(IVector3<T> const &other) const;

  bool operator==(IVector3<T> const &rhs) const;
  bool operator!=(IVector3<T> const &rhs) const;

  #define DECL_IVECTOR3_OP(OP) \
    IVector3<T> operator OP(IVector3<T> const &rhs) const; \
    IVector3<T> operator OP(T const &rhs) const;

  DECL_IVECTOR3_OP(+)
  DECL_IVECTOR3_OP(-)
  DECL_IVECTOR3_OP(*)
  DECL_IVECTOR3_OP(/)

  #undef DECL_IVECTOR3_OP
};

template <typename T>
IVector3<T>::IVector3()
  : IVector3<T>(0, 0, 0) {
}

template <typename T>
IVector3<T>::IVector3(IVector<T, 3> const &vec)
  : x(vec[0]), y(vec[1]), z(vec[2]) {
}

template <typename T>
IVector3<T>::IVector3(T const &x_val, T const &y_val, T const &z_val)
  : x(x_val), y(y_val), z(z_val) {
}

template <typename T>
float IVector3<T>::length() const {
  return IVector<float, 3>(x, y, z).length();
}

template <typename T>
IVector3<float> IVector3<T>::normalize() const {
  return IVector<float, 3>(x, y, z).normalize();
}

template <typename T>
float IVector3<T>::dot(IVector3<T> const &other) const {
  return IVector<float, 3>(x, y, z).dot(IVector<float, 3>(other.x, other.y, other.z));
}

template <typename T>
IVector3<float> IVector3<T>::cross(IVector3<T> const &other) const {
  return IVector3<float>(
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  );
}

template <typename T>
bool IVector3<T>::operator==(IVector3<T> const &rhs) const {
  return IVector<T, 3>(x, y, z) == IVector<T, 3>(rhs.x, rhs.y, rhs.z);
}

template <typename T>
bool IVector3<T>::operator!=(IVector3<T> const &rhs) const {
  return IVector<T, 3>(x, y, z) != IVector<T, 3>(rhs.x, rhs.y, rhs.z);
}


#define DEFINE_IVECTOR3_OP(OP) \
template <typename T> \
IVector3<T> IVector3<T>::operator OP(IVector3<T> const &rhs) const { \
  return IVector<T, 3>(x, y, z) OP IVector<T, 3>(rhs.x, rhs.y, rhs.z); \
} \
template <typename T> \
IVector3<T> IVector3<T>::operator OP(T const &rhs) const { \
  return IVector<T, 3>(x, y, z) OP rhs; \
}

DEFINE_IVECTOR3_OP(+)
DEFINE_IVECTOR3_OP(-)
DEFINE_IVECTOR3_OP(*)
DEFINE_IVECTOR3_OP(/)

#undef DEFINE_IVECTOR3_OP
