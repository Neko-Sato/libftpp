/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:55:08 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/03 19:10:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <random>
#include "random_2D_coordinate_generator.hpp"

long long splitmix64(long long x) {
  x += 0x9e3779b97f4a7c15ULL;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
  return x ^ (x >> 31);
}

template class RandomCoordinateGenerator<2>;