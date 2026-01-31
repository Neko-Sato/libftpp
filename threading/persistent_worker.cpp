/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 23:16:35 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/31 23:29:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_worker.hpp"

PersistentWorker::PersistentWorker()
  : _active(true)
  , _thread("Worker", [this]() {_worker();}) {
  _thread.start();
}

PersistentWorker::~PersistentWorker() {
  {
    std::lock_guard<std::mutex> lock(_mtx);
    _active = false;
  }
  _cond.notify_all();
  _thread.stop();
}

void PersistentWorker::addTask(std::string const &name, std::function<void()> const &jobToExecute) {
  std::lock_guard<std::mutex> lock(_mtx);
  if (_tasksMap.find(name) != _tasksMap.end())
    throw std::runtime_error("task exists");
  _tasks.emplace_back(_Task{name, false, jobToExecute});
  _tasksMap.emplace(name, std::prev(_tasks.end()));
}

void PersistentWorker::removeTask(std::string const &name) {
  std::lock_guard<std::mutex> lock(_mtx);
  auto it = _tasksMap.find(name);
  if (it == _tasksMap.end())
    throw std::runtime_error("task not found");
  it->second->removed = true;
  _tasksMap.erase(it);
}

void PersistentWorker::_worker() {
  for (;;) {
    std::unique_lock<std::mutex> lock(_mtx);
    _cond.wait(lock, [this]{ return !_active || !_tasks.empty(); });
    if (!_active)
      break;
    for (auto it = _tasks.begin(); it != _tasks.end(); ) {
      if (it->removed){
        it = _tasks.erase(it);
      } else {
        lock.unlock();
        try {
          it->fun();
        } catch (std::exception const &e) {
          threadSafeCout << std::format("{} : {}", it->name, e.what()) << std::endl;
        }
        lock.lock();
        ++it;
      }
    }
  }
}
