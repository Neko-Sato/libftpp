/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:28:32 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:30:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./fcntl.h"
#include "./os_error.hpp"
#include <cerrno>

void setblocking(int fd, bool blocking) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    throw OSError(errno, "fcntl");
  if ((!!(flags & O_NONBLOCK)) ^ !blocking)
    if (fcntl(fd, F_SETFL, flags ^ O_NONBLOCK) == -1)
      throw OSError(errno, "fcntl");
}

void setcloexec(int fd, bool cloexec) {
  int flags = fcntl(fd, F_GETFD, 0);
  if (flags == -1)
    throw OSError(errno, "fcntl");
  if ((!!(flags & FD_CLOEXEC)) ^ !!cloexec)
    if (fcntl(fd, F_SETFD, flags ^ FD_CLOEXEC) == -1)
      throw OSError(errno, "fcntl");
}
