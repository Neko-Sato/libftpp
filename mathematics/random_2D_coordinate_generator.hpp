/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:55:08 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 01:46:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <random>

template <size_t D, typename RNG=std::mt19937_64>
class RandomCoordinateGenerator {
public:
  RandomCoordinateGenerator(RNG::result_type value = RNG::default_seed);

  RNG::result_type seed() const;
  void seed(RNG::result_type seed);
  template <typename ...Args>
  RNG::result_type operator()(Args &&...args) requires (sizeof...(Args) == D) ;

private:
  RNG::result_type _seed;

};

template <size_t D, typename RNG>
RandomCoordinateGenerator<D, RNG>::RandomCoordinateGenerator(RNG::result_type seed)
  : _seed(seed) {
}

template <size_t D, typename RNG>
RNG::result_type RandomCoordinateGenerator<D, RNG>::seed() const {
  return _seed;
}

template <size_t D, typename RNG>
void RandomCoordinateGenerator<D, RNG>::seed(RNG::result_type seed) {
  _seed = seed;
}

template <size_t D, typename RNG>
template <typename ...Args>
RNG::result_type RandomCoordinateGenerator<D, RNG>::operator()(Args &&...args) requires (sizeof...(Args) == D)  {
  return RNG{_seed}() ^ (RNG{std::forward<Args>(args)}() ^ ...);
}

using Random2DCoordinateGenerator = RandomCoordinateGenerator<2>;
extern template class RandomCoordinateGenerator<2>;
