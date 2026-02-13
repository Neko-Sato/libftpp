/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:49:52 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/06 23:49:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <deque>
#include <mutex>
#include <functional>

template <typename TType, typename Container = std::deque<TType>>
class ThreadSafeQueue {
public:
  using container_type = Container;
  using value_type = typename container_type::value_type;
  using reference = typename container_type::reference;
  using const_reference = typename container_type::const_reference;
  using size_type = typename container_type::size_type;
  static_assert(std::is_same<TType, value_type>::value, "");

  ThreadSafeQueue(ThreadSafeQueue const &rhs);
  ThreadSafeQueue(ThreadSafeQueue &&rhs);
  ThreadSafeQueue &operator=(ThreadSafeQueue const &rhs);
  ThreadSafeQueue &operator=(ThreadSafeQueue &&rhs);

  void push_back(TType const &newElement);
  void push_front(TType const &newElement);
  TType pop_back();
  TType pop_front();

private:
  container_type _container;
  mutable std::mutex _mtx;
};

template <typename TType, typename Container>
ThreadSafeQueue<TType, Container>::ThreadSafeQueue(ThreadSafeQueue const &rhs)
  : _container([&]() {
    std::lock_guard<std::mutex> lock(rhs._mtx);
    return rhs._container;
  }()) {
};

template <typename TType, typename Container>
ThreadSafeQueue<TType, Container>::ThreadSafeQueue(ThreadSafeQueue &&rhs)
  : _container([&]{
    std::lock_guard<std::mutex> lock(rhs._mtx);
    return std::move(rhs._container);
  }()) {
};

template <typename TType, typename Container>
ThreadSafeQueue<TType, Container>
&ThreadSafeQueue<TType, Container>::operator=(ThreadSafeQueue const &rhs) {
  if (this != &rhs)
    *this = ThreadSafeQueue(rhs);
  return *this;
};

template <typename TType, typename Container>
ThreadSafeQueue<TType, Container>
&ThreadSafeQueue<TType, Container>::operator=(ThreadSafeQueue &&rhs) {
  if (this != &rhs) {
    std::lock_guard<std::mutex> lock1(_mtx), lock2(rhs._mtx);
    _container = std::move(rhs._container);
  }
  return *this;
};

template <typename TType, typename Container>
void ThreadSafeQueue<TType, Container>::push_back(TType const &newElement) {
  std::lock_guard<std::mutex> lock(_mtx);
  _container.push_back(newElement);
};

template <typename TType, typename Container>
void ThreadSafeQueue<TType, Container>::push_front(TType const &newElement) {
  std::lock_guard<std::mutex> lock(_mtx);
  _container.push_front(newElement);
};

template <typename TType, typename Container>
TType ThreadSafeQueue<TType, Container>::pop_back() {
  std::lock_guard<std::mutex> lock(_mtx);
  if (_container.empty())
      throw std::runtime_error("pop from empty queue");
  TType tmp(std::move(_container.back()));
  _container.pop_back();
  return tmp;
};

template <typename TType, typename Container>
TType ThreadSafeQueue<TType, Container>::pop_front() {
  std::lock_guard<std::mutex> lock(_mtx);
  if (_container.empty())
      throw std::runtime_error("pop from empty queue");
  TType tmp(std::move(_container.front()));
  _container.pop_front();
  return tmp;
};
