/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:12:24 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 11:12:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hpp"

WorkerPool::WorkerPool(std::size_t n)
  : _active(true) {
  for (std::size_t i = 0; i < n; ++i) {
    auto &worker = _pool.emplace_back(std::format("worker {}", i), [this]{_worker();}); 
    worker.start();
  }
}

WorkerPool::~WorkerPool() {
  {
    std::lock_guard<std::mutex> lock(_mtx);
    _active = false;
  }
  _cond.notify_all();
  for (auto& worker : _pool)
    worker.stop();
}

void WorkerPool::addJob(std::function<void()> const &jobToExecute) {
  std::lock_guard<std::mutex> lock(_mtx);
  _queue.push(jobToExecute);
  _cond.notify_one();
}

void WorkerPool::_worker() {
  for (;;) {
    std::function<void()> job;
    {
      std::unique_lock<std::mutex> lock(_mtx);
      _cond.wait(lock, [this]{ return !_active || !_queue.empty(); });
      if (!_active && _queue.empty())
        break;
      job = std::move(_queue.front());
      _queue.pop();
    }
    job();
  }
}
