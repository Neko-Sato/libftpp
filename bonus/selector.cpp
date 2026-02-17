/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selector.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:20:16 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:21:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./selector.hpp"

Selector::event const Selector::READ = 1;
Selector::event const Selector::WRITE = 2;
Selector::event const Selector::EXCEPT = 4;

void Selector::add(int fd, event events) {
  auto [it, inserted] = _fds.try_emplace(fd, events);
  if (!inserted)
    throw RegisteredError();
}

void Selector::remove(int fd) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  _fds.erase(it);
}

void Selector::modify(int fd, event events) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  it->second = events;
}

Selector::Mapping const &Selector::getMap() const {
  return _fds;
}

char const *Selector::RegisteredError::what() const throw() {
  return "already registered";
}

char const *Selector::NotRegisteredError::what() const throw() {
  return "not registered";
}
