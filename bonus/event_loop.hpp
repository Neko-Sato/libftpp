/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:17:25 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 10:28:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./selector.hpp"
#include "./async.hpp"

class EventLoop {
public:
  EventLoop(Selector &&selector);
  EventLoop(EventLoop const &) = delete;
  EventLoop(EventLoop &&) = default;
  ~EventLoop() = default;
  EventLoop &operator=(EventLoop const &) = delete;
  EventLoop &operator=(EventLoop &&) = default;

  template <typename T>
  T run(Async<T> &coro);
  Async<void> add_read(int fd);
  Async<void> add_write(int fd);

private:
  using IOHandler = 

};