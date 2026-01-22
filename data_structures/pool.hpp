/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 05:27:18 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 09:56:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ranges>
#include <vector>
#include <cassert>
#include <optional>
#include <utility>
#include <stdexcept>

template <typename TType>
class Pool {
public:
  Pool(std::size_t n = 0);
  Pool(Pool const &) = delete;
  Pool(Pool &&) = delete;

  Pool& operator=(Pool const &) = delete;
  Pool& operator=(Pool &&) = delete;

  class Object;

  void resize(size_t const &numberOfObjectStored);
  
  template<typename ...TArgs>
  Object acquire(TArgs &&...p_args);

private:
  using _Slot = std::optional<TType>;
  using _Buffer = std::vector<_Slot>;
  _Buffer _buffer;
};

template <typename TType>
class Pool<TType>::Object {
public:
  Object() = delete;
  Object(Object const &) = delete;
  Object(Object &&rhs);
  ~Object();

  Object &operator=(Object const &) = delete;
  Object &operator=(Object &&rhs);
  TType* operator->();

private:
  Object(Pool *pool, std::size_t index);

  Pool *_pool;
  std::size_t _index;
  friend class Pool;
};

template <typename TType>
Pool<TType>::Pool(std::size_t n)
  : _buffer(n) {
}

template <typename TType>
void Pool<TType>::resize(size_t const &numberOfObjectStored) {
  if (numberOfObjectStored < _buffer.size())
    throw std::runtime_error("Cannot shrink pool");
  _buffer.resize(numberOfObjectStored);
}

template <typename TType>
template<typename ...TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs &&...p_args) {
  auto it = std::find_if(_buffer.begin(), _buffer.end(),
    [](auto const &slot) { return !slot.has_value(); });  
  if (it == _buffer.end())
    throw std::runtime_error("Pool exhausted");
  it->emplace(std::forward<TArgs>(p_args)...);
  return Pool<TType>::Object(this, std::distance(_buffer.begin(), it));
}

template <typename TType>
Pool<TType>::Object::Object(Pool *pool, std::size_t index)
  : _pool(pool)
  , _index(index) {
}

template <typename TType>
Pool<TType>::Object::Object(Object &&rhs)
  : _pool(std::exchange(rhs._pool, nullptr))
  , _index(std::exchange(rhs._index, 0)) {
}

template <typename TType>
Pool<TType>::Object::~Object() {
  if (_pool)
    _pool->_buffer[_index].reset();
}

template <typename TType>
typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object &&rhs) {
  if (this != &rhs) {
    _pool = std::exchange(rhs._pool, nullptr);
    _index = std::exchange(rhs._index, 0);
  }
  return *this;
}

template <typename TType>
TType* Pool<TType>::Object::operator->() {
  assert(_pool);
  _Slot &slot = _pool->_buffer[_index];
  return &*slot;
}
