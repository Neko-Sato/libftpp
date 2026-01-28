/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:49:52 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 08:34:55 by hshimizu         ###   ########.fr       */
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

  void push_back(TType const &newElement);
  void push_front(TType const &newElement);
  TType pop_back();
  TType pop_front();

protected:
  container_type c;
private:
  std::mutex _mtx;
};

template <typename TType, typename Container>
void ThreadSafeQueue<TType, Container>::push_back(TType const &newElement) {
  std::lock_guard<std::mutex> lock(_mtx);
  c.push_back(newElement);
};

template <typename TType, typename Container>
void ThreadSafeQueue<TType, Container>::push_front(TType const &newElement) {
  std::lock_guard<std::mutex> lock(_mtx);
  c.push_front(newElement);
};

template <typename TType, typename Container>
TType ThreadSafeQueue<TType, Container>::pop_back() {
  std::lock_guard<std::mutex> lock(_mtx);
  if (c.empty())
      throw std::runtime_error("pop from empty queue");
  TType tmp = std::move(c.back());
  c.pop_back();
  return tmp;
};

template <typename TType, typename Container>
TType ThreadSafeQueue<TType, Container>::pop_front() {
  std::lock_guard<std::mutex> lock(_mtx);
  if (c.empty())
      throw std::runtime_error("pop from empty queue");
  TType tmp = std::move(c.front());
  c.pop_front();
  return tmp;
};
