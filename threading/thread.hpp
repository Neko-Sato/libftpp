/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:07:50 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/03 20:00:27 by hshimizu         ###   ########.fr       */
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

  Thread(Thread const &) = delete;
  Thread &operator=(Thread const &) = delete;

  void start();
  void stop();

private:
  std::function<void()> _func;
  std::thread _thread;
};
