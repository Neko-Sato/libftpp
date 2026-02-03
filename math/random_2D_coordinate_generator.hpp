/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:55:08 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/04 01:21:27 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

long long splitmix64(long long x);

template <size_t D>
class RandomCoordinateGenerator {
public:
  RandomCoordinateGenerator();
  RandomCoordinateGenerator(long long seed);

  long long seed() const;
  void seed(long long seed);
  template <typename ...Args>
  long long operator()(Args &&...args) requires (sizeof...(Args) == D) ;

private:
  long long _seed;

};

template <size_t D>
RandomCoordinateGenerator<D>::RandomCoordinateGenerator()
  : RandomCoordinateGenerator([] {
      std::random_device rd;
      return static_cast<long long>(rd()) << 32 | static_cast<long long>(rd());
    }()) {
}

template <size_t D>
RandomCoordinateGenerator<D>::RandomCoordinateGenerator(long long seed)
  : _seed(seed) {
}

template <size_t D>
long long RandomCoordinateGenerator<D>::seed() const {
  return _seed;
}

template <size_t D>
void RandomCoordinateGenerator<D>::seed(long long seed) {
  _seed = seed;
}

template <size_t D>
template <typename ...Args>
long long RandomCoordinateGenerator<D>::operator()(Args &&...args) requires (sizeof...(Args) == D)  {
  return (splitmix64(splitmix64(_seed) ^ ... ^ splitmix64(std::forward<Args>(args))));
}

using Random2DCoordinateGenerator = RandomCoordinateGenerator<2>;
extern template class RandomCoordinateGenerator<2>;
