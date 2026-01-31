/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:30:21 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/31 23:16:30 by hshimizu         ###   ########.fr       */
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
  std::mutex _mtx;
  std::condition_variable _cond;
  _Tasks _tasks;
  _TasksMap _tasksMap;

  void _worker();
};
