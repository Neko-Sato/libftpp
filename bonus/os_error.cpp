/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os_error.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:11:53 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:15:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <format>
#include "./os_error.hpp"

OSError::OSError(int __errno, std::string const &s)
    : _errno(__errno), _s(std::format("{}: {}", s, ::strerror(_errno))) {
}

int OSError::getErrno() const {
  return _errno;
}

char const *OSError::what() const noexcept {
  return _s.c_str();
}