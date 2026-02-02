/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:55:08 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/03 00:44:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <random>
#include "random_2D_coordinate_generator.hpp"

static long long splitmix64(long long x) {
  x += 0x9e3779b97f4a7c15ULL;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
  return x ^ (x >> 31);
}

Random2DCoordinateGenerator::Random2DCoordinateGenerator()
  : _seed([] {
      std::random_device rd;
      return static_cast<long long>(rd()) << 32 | static_cast<long long>(rd());
    }()) {
}

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed)
  : _seed(seed) {
}

long long Random2DCoordinateGenerator::seed() const {
  return _seed;
}

void Random2DCoordinateGenerator::seed(long long seed) {
  _seed = seed;
}

long long Random2DCoordinateGenerator::operator()(long long const &x, long long const &y) {
  return splitmix64(splitmix64(_seed) ^ splitmix64(x) ^ splitmix64(y));
}
