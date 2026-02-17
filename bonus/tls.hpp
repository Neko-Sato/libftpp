/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:18:18 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:18:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unordered_map>
#include <list>
#include <mutex>
#include <thread>
#include <functional>
#include <utility>

template<typename T>
class tls {
public:
  tls() = default;
  ~tls();
  tls(tls const &) = delete;
  tls(tls &&) = delete;

  tls &operator=(tls const &) = delete;
  tls &operator=(tls &&) = delete;

  T &get();
  T const &get() const;

private:
  static std::mutex _globalMtx;
  using Entries = std::unordered_map<tls const *, T>;
  using ThreadMap = std::unordered_map<std::thread::id, Entries>;
  static ThreadMap _threadMap;
  struct Local {
    Local();
    ~Local();
  };
  static thread_local Local _local;
};

template <typename T>
std::mutex tls<T>::_globalMtx;

template <typename T>
typename tls<T>::ThreadMap tls<T>::_threadMap;

template <typename T>
thread_local typename tls<T>::Local tls<T>::_local;

template <typename T>
tls<T>::Local::Local() {
  std::lock_guard<std::mutex> lock(_globalMtx);
  _threadMap.try_emplace(std::this_thread::get_id());
}

template <typename T>
tls<T>::Local::~Local() {
  std::lock_guard<std::mutex> lock(_globalMtx);
  _threadMap.erase(std::this_thread::get_id());
}

template <typename T>
tls<T>::~tls() {
  std::lock_guard<std::mutex> lock(_globalMtx);
  for (auto &entries : _threadMap)
    entries.second.erase(this);
}

template <typename T>
T &tls<T>::get() {
  return _threadMap[std::this_thread::get_id()][this];
}

template <typename T>
T const &tls<T>::get() const {
  return _threadMap[std::this_thread::get_id()][this];
}
