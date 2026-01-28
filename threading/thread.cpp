/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:11:39 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 09:18:32 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.hpp"

Thread::Thread(std::string const &name, std::function<void()> functToExecute)
  : _func([name, functToExecute](){
    threadSafeCout.setPrefix(name);
    functToExecute();
  }) {
}

void Thread::start() {
  _thread = std::thread(_func);
}

void Thread::stop() {
  if (_thread.joinable())
    _thread.join();
}
