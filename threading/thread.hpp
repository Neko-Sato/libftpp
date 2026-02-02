/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:07:50 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/02 11:56:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <functional>
#include <thread>
#include "thread_safe_iostream.hpp"

class Thread {
public:
  Thread(std::string const &name, std::function<void()> functToExecute);
  ~Thread();

  Thread() = delete;
  Thread(Thread const &) = delete;
  Thread &operator=(Thread const &) = delete;
  Thread(Thread &&) = delete;
  Thread &operator=(Thread &&) = delete;

  void start();
  void stop();

private:
  std::function<void()> _func;
  std::thread _thread;
};
