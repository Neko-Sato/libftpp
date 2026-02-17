/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os_error.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:10:58 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:15:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

class OSError : public std::exception {
public:
  OSError(int __errno = 0, std::string const &s = "OSError");

  int getErrno() const;
  char const *what() const noexcept;

private:
  int _errno;
  std::string _s;
};
