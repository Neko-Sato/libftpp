/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addrInfos.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:26:02 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/18 00:39:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./addrinfos.hpp"
#include "./os_error.hpp"
#include <format>

AddrInfos::iterator::iterator(addrinfo const *info)
  : _current(info) {
}

AddrInfos::iterator &AddrInfos::iterator::operator++() {
  _current = _current->ai_next;
  return *this;
}

AddrInfos::iterator AddrInfos::iterator::operator++(int) {
  iterator tmp(*this);
  _current = _current->ai_next;
  return tmp;
}

bool AddrInfos::iterator::operator==(iterator const &rhs) const {
  return _current == rhs._current;
}

bool AddrInfos::iterator::operator!=(iterator const &rhs) const {
  return _current != rhs._current;
}

addrinfo const &AddrInfos::iterator::operator*() const {
  return *_current;
}

addrinfo const *AddrInfos::iterator::operator->() const {
  return _current;
}

AddrInfos::Hints::Hints(int family, int socktype, int protocol, int flags) {
  ai_family = family;
  ai_socktype = socktype;
  ai_protocol = protocol;
  ai_flags = flags;
}

AddrInfos::Error::Error(int __errno)
    : _errno(__errno)
    , _s(std::format("getaddrinfo: {}", gai_strerror(_errno))) {
}

int AddrInfos::Error::getErrno() const {
  return _errno;
}

char const *AddrInfos::Error::what() const noexcept {
  return _s.c_str();
}

addrinfo *AddrInfos::_getaddrinfo(char const *name, char const *service,
                                  addrinfo const *hints) {
  addrinfo *res;
  int err = ::getaddrinfo(name, service, hints, &res);
  if (err != 0)
    throw Error(err);
  return res;
}

AddrInfos::AddrInfos()
  : _addrinfo(nullptr) {
}

AddrInfos::AddrInfos(char const *name, char const *service)
  : _addrinfo(_getaddrinfo(name, service)) {
}

AddrInfos::AddrInfos(char const *name, char const *service, Hints const &hints)
  : _addrinfo(_getaddrinfo(name, service, &hints)) {
}

AddrInfos::~AddrInfos() {
  if (_addrinfo)
    freeaddrinfo(_addrinfo);
}

AddrInfos::iterator AddrInfos::begin() const {
  return iterator(_addrinfo);
}

AddrInfos::iterator AddrInfos::end() const {
  return iterator(NULL);
}
