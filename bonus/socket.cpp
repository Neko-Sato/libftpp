/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:24:43 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:25:25 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./socket.hpp"
#include "./os_error.hpp"
#include <cerrno>
#include <unistd.h>

int Socket::_makeSockfd(int domain, int type, int protocol) {
  int sockfd = ::socket(domain, type, protocol);
  if (sockfd == -1)
    throw OSError(errno, "socket");
  return sockfd;
}

Socket::Socket() : _sockfd(-1) {
}

Socket::Socket(int domain, int type, int protocol)
  : _sockfd(_makeSockfd(domain, type, protocol)) {
}

Socket::Socket(int sockfd) : _sockfd(sockfd) {
}

Socket::~Socket() {
  close();
}

Socket::operator bool() const {
  return _sockfd != -1;
}

int Socket::fd() const {
  return _sockfd;
}

void Socket::bind(sockaddr const *addr, socklen_t addrlen) {
  if (::bind(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "bind");
}

void Socket::connect(sockaddr const *addr, socklen_t addrlen) {
  if (::connect(_sockfd, addr, addrlen) == -1) 
    throw OSError(errno, "connect");
}

void Socket::listen(int backlog) {
  if (::listen(_sockfd, backlog) == -1)
    throw OSError(errno, "listen");
}

Socket Socket::accept() {
  int connfd = ::accept(_sockfd, NULL, 0);
  if (connfd == -1)
    throw OSError(errno, "accept");
  return Socket(connfd);
}

std::size_t Socket::write(void const *buf, std::size_t len) {
  ssize_t ret = ::write(_sockfd, buf, len);
  if (ret == -1)
    throw OSError(errno, "write");
  return ret;
}

std::size_t Socket::read(void *buf, std::size_t len) {
  ssize_t ret = ::read(_sockfd, buf, len);
  if (ret == -1)
    throw OSError(errno, "read");
  return ret;
}

std::size_t Socket::send(void const *buf, std::size_t len, int flags) {
  ssize_t ret = ::send(_sockfd, buf, len, flags);
  if (ret == -1)
    throw OSError(errno, "send");
  return ret;
}

std::size_t Socket::recv(void *buf, std::size_t len, int flags) {
  ssize_t ret = ::recv(_sockfd, buf, len, flags);
  if (ret == -1)
    throw OSError(errno, "recv");
  return ret;
}

std::size_t Socket::sendto(void const *buf, std::size_t len, int flags,
                           sockaddr const *dest_addr, socklen_t addrlen) {
  ssize_t ret = ::sendto(_sockfd, buf, len, flags, dest_addr, addrlen);
  if (ret == -1)
    throw OSError(errno, "sendto");
  return ret;
}

std::size_t Socket::recvfrom(void *buf, std::size_t len, int flags,
                             sockaddr *src_addr, socklen_t *addrlen) {
  ssize_t ret = ::recvfrom(_sockfd, buf, len, flags, src_addr, addrlen);
  if (ret == -1)
    throw OSError(errno, "recvfrom");
  return ret;
}

void Socket::close() {
  if (_sockfd != -1) {
    if (::close(_sockfd) == -1)
      throw OSError(errno, "close");
    _sockfd = -1;
  }
}

void Socket::shutdown(int how) {
  if (::shutdown(_sockfd, how) == -1)
    throw OSError(errno, "shutdown");
}

void Socket::getsockname(sockaddr *addr, socklen_t *addrlen) {
  if (::getsockname(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "getsockname");
}

void Socket::getpeername(sockaddr *addr, socklen_t *addrlen) {
  if (::getpeername(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "getpeername");
}

void Socket::getsockopt(int level, int optname, void *optval,
                        socklen_t *optlen) {
  if (::getsockopt(_sockfd, level, optname, optval, optlen) == -1)
    throw OSError(errno, "getsockopt");
}

void Socket::setsockopt(int level, int optname, void const *optval,
                        socklen_t optlen) {
  if (::setsockopt(_sockfd, level, optname, optval, optlen) == -1)
    throw OSError(errno, "setsockopt");
}
