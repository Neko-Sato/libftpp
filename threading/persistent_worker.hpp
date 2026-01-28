/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:30:21 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 12:07:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>
#include <list>
#include <unordered_map>


class PersistentWorker {
public:
  PersistentWorker();
  ~PersistentWorker();

  void addTask(std::string const &name, std::function<void()> const &jobToExecute);
  void removeTask(std::string const &name);

private:
  using _Tasks = std::list<std::function<void()>>;
  using _TaskMap = std::unordered_map<std::string, _Tasks::iterator>;

  std::mutex _mtx;
  _Tasks _tasks;
  _TaskMap _taskMap;
};
