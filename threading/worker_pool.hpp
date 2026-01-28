/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:43:25 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 11:12:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <queue>
#include <functional>
#include "thread.hpp"

class WorkerPool {
public:
  WorkerPool(std::size_t n);
  ~WorkerPool();

  void addJob(std::function<void()> const &jobToExecute);

private:
  bool _active;
  std::vector<Thread> _pool;
  std::queue<std::function<void()>> _queue;
  std::mutex _mtx;
  std::condition_variable _cond;

  void _worker();
};
