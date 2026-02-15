/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:11:39 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 01:46:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./thread.hpp"
#include <format>

Thread::Thread(std::string const &name, std::function<void()> functToExecute)
  : _func([name, functToExecute](){
    threadSafeCout.setPrefix(std::format("[{}] ", name));
    functToExecute();
  }) {
}

Thread::~Thread() {
  stop();
}

void Thread::start() {
  _thread = std::thread(_func);
}

void Thread::stop() {
  if (_thread.joinable())
    _thread.join();
}
