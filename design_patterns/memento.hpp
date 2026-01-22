/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 11:22:33 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 11:59:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <deque>
#include <any>

class AnyQueue{
public:
  template <typename T>
  AnyQueue &operator<<(T const &rhs);
  template <typename T>
  AnyQueue &operator>>(T &rhs);

private:
  std::deque<std::any> _data;
};

template <typename T>
AnyQueue &AnyQueue::operator<<(T const &rhs) {
  _data.push_back(rhs);
  return *this;
}

template <typename T>
AnyQueue &AnyQueue::operator>>(T &rhs) {
  if (_data.empty())
    throw std::runtime_error("empty");
  rhs = std::move(any_cast<T &>(_data.front()));
  _data.pop_front();
  return *this;
};

class Memento {
public:
  using Snapshot = AnyQueue;

  Snapshot save() const;
  void load(Snapshot &snapshot);

protected:
  virtual void _saveToSnapshot(Snapshot &snapshotToFill) const = 0;
  virtual void _loadFromSnapshot(Snapshot &snapshot) = 0;
};
