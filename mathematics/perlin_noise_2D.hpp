/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:36:18 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 01:46:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <numbers>
#include "./ivector2.hpp"

template <typename RNG=std::mt19937_64>
class PerlinNoise2D {
public:
  PerlinNoise2D(RNG::result_type seed=RNG::default_seed);

  RNG::result_type seed() const;
  void seed(RNG::result_type seed);

  float sample(IVector<float, 2> const &pos);
  float sample(float x, float y);
  float operator()(float x, float y);

  static float fade(float t);
  static float lerp(float a, float b, float t);

private:
  RNG::result_type _seed;
  using Permutation = std::array<int, 512>;
  Permutation _perm;

  static Permutation _makePermutation(RNG::result_type seed);
  IVector<float, 2> _grad(IVector<float, 2> const &pos);
};

template <typename RNG>
PerlinNoise2D<RNG>::Permutation PerlinNoise2D<RNG>::_makePermutation(RNG::result_type seed) {
  Permutation tmp;
  std::iota(tmp.begin(), tmp.begin() + 256, 0);
  std::shuffle(tmp.begin(), tmp.begin() + 256, RNG(seed));
  std::copy(tmp.begin(), tmp.begin() + 256, tmp.begin() + 256);
  return tmp;
}

template <typename RNG>
PerlinNoise2D<RNG>::PerlinNoise2D(RNG::result_type seed)
  : _seed(seed), _perm(_makePermutation(seed)) {
}

template <typename RNG>
RNG::result_type PerlinNoise2D<RNG>::seed() const {
  return _seed;
}

template <typename RNG>
void PerlinNoise2D<RNG>::seed(RNG::result_type seed)  {
  _seed = seed;
  _perm = _makePermutation(seed);
}

template <typename RNG>
float PerlinNoise2D<RNG>::operator()(float x, float y) {
  return sample(IVector<float, 2>(x, y));
}

template <typename RNG>
float PerlinNoise2D<RNG>::sample(float x, float y) {
  return sample(IVector<float, 2>(x, y));
}

template <typename RNG>
float PerlinNoise2D<RNG>::fade(float t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

template <typename RNG>
float PerlinNoise2D<RNG>::lerp(float a, float b, float t) {
  return a + t * (b - a);
}

template <typename RNG>
IVector<float, 2> PerlinNoise2D<RNG>::_grad(IVector<float, 2> const &pos) {
  float p = _perm[(static_cast<int>(pos[0]) & 255) + _perm[static_cast<int>(pos[1]) & 255]];
  p /= 256.0;
  p *= 2.0 * std::numbers::pi;
  return IVector<float, 2>(cos(p), sin(p));
}

template <typename RNG>
float PerlinNoise2D<RNG>::sample(IVector<float, 2> const &pos) {
  IVector<int, 2> point[] = {
    {floor(pos[0]), floor(pos[1])},
    {floor(pos[0]), floor(pos[1]) + 1},
    {floor(pos[0]) + 1, floor(pos[1])},
    {floor(pos[0]) + 1, floor(pos[1]) + 1},
  };
  IVector<float, 2> grad[] = {
    _grad(point[0]),
    _grad(point[1]),
    _grad(point[2]),
    _grad(point[3]),
  };
  float value[] = {
    grad[0].dot(pos - point[0]),
    grad[1].dot(pos - point[1]),
    grad[2].dot(pos - point[2]),
    grad[3].dot(pos - point[3]),
  };
  IVector<float, 2> uv(pos - point[0]);
  uv[0] = fade(uv[0]);
  uv[1] = fade(uv[1]);
  return lerp(
    lerp(value[0], value[1], uv[1]),
    lerp(value[2], value[3], uv[1]),
    uv[0]);  
}

extern template class PerlinNoise2D<>;