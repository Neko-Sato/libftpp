/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:56:07 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/02 22:16:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ivector2.hpp"

template <typename T>
class IVector3 : public IVector<T, 3>{
public:
  T &x, &y, &z;
  
  IVector3();
  IVector3(IVector<T, 3> const &vec);
  IVector3(T const &x_val, T const &y_val, T const &z_val);

  float length() const;
  IVector3<float> normalize() const;
  float dot(IVector3<T> const &other) const;
  IVector3<float> cross(IVector3<T> const &other) const;

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
  : IVector<T, 3>(), x((*this)[0]), y((*this)[1]), z((*this)[2]) {
}

template <typename T>
IVector3<T>::IVector3(IVector<T, 3> const &vec)
  : IVector<T, 3>(vec), x((*this)[0]), y((*this)[1]), z((*this)[2]) {
}

template <typename T>
IVector3<T>::IVector3(T const &x_val, T const &y_val, T const &z_val)
  : IVector<T, 3>(x_val, y_val, z_val), x((*this)[0]), y((*this)[1]), z((*this)[2]) {
}

template <typename T>
float IVector3<T>::length() const {
  return static_cast<IVector<float, 3>>(*this).length();
}

template <typename T>
IVector3<float> IVector3<T>::normalize() const {
  return static_cast<IVector<float, 3>>(*this).normalize();
}

template <typename T>
float IVector3<T>::dot(IVector3<T> const &other) const {
  return static_cast<IVector<float, 3>>(*this).dot(other);
}

template <typename T>
IVector3<float> IVector3<T>::cross(IVector3<T> const &other) const {
  return IVector3<float>(
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  );
}

#define DEFINE_IVECTOR3_OP(OP) \
template <typename T> \
IVector3<T> IVector3<T>::operator OP(IVector3<T> const &rhs) const { \
  return IVector<T, 3>::operator OP(static_cast<IVector<T, 3>>(rhs)); \
} \
template <typename T> \
IVector3<T> IVector3<T>::operator OP(T const &rhs) const { \
  return IVector<T, 3>::operator OP(rhs); \
}

DEFINE_IVECTOR3_OP(+)
DEFINE_IVECTOR3_OP(-)
DEFINE_IVECTOR3_OP(*)
DEFINE_IVECTOR3_OP(/)

#undef DEFINE_IVECTOR3_OP
