/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:30:21 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/02 11:44:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>
#include <list>
#include <unordered_map>
#include "thread.hpp"

class PersistentWorker {
public:
  PersistentWorker();
  ~PersistentWorker();

  PersistentWorker(PersistentWorker const &) = delete;
  PersistentWorker &operator=(PersistentWorker const &) = delete;
  PersistentWorker(PersistentWorker &&) = delete;
  PersistentWorker &operator=(PersistentWorker &&) = delete;

  void addTask(std::string const &name, std::function<void()> const &jobToExecute);
  void removeTask(std::string const &name);

private:
  struct _Task {
    std::string name;
    bool removed;
    std::function<void()> fun;
  };
  
  using _Tasks = std::list<_Task>;
  using _TasksMap = std::unordered_map<std::string_view, _Tasks::iterator>;

  bool _active;
  Thread _thread;
  mutable std::mutex _mtx;
  std::condition_variable _cond;
  _Tasks _tasks;
  _TasksMap _tasksMap;

  void _worker();
};
