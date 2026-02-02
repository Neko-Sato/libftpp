/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:55:08 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/03 00:44:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Random2DCoordinateGenerator {
public:
  Random2DCoordinateGenerator();
  Random2DCoordinateGenerator(long long seed);

  long long seed() const;
  void seed(long long seed);
  long long operator()(long long const &x, long long const &y);

private:
  long long _seed;

};
