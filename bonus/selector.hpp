/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:20:16 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:20:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unordered_map>
#include <queue>

class Selector {
public:
  using event = unsigned int;
  static event const READ;
  static event const WRITE;
  static event const EXCEPT;
  struct event_details {
    int fd;
    event events;
  };
  using Mapping = std::unordered_map<int, event>;
  using Events = std::queue<event_details>;

  virtual ~Selector() = default;

  virtual void add(int fd, event events);
  virtual void remove(int fd);
  virtual void modify(int fd, event events);
  Mapping const &getMap() const;
  virtual Events select(int timeout) const = 0;

  class RegisteredError : public std::exception {
    char const *what() const throw();
  };

  class NotRegisteredError : public std::exception {
    char const *what() const throw();
  };

protected:
  Selector() = default;
  Selector(Selector const &) = delete;
  Selector(Selector &&) = default;
  Selector &operator=(Selector const &) = delete;
  Selector &operator=(Selector &&) = default;

private:
  Mapping _fds;
};
