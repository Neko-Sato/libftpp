/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 05:27:18 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/19 08:29:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>
#include <vector>

template <typename TType>
class Pool {
public:
  Pool(std::size_t n = 0);
  Pool(Pool const &) = delete;
  Pool(Pool &&) = delete;

  Pool& operator=(Pool const &) = delete;
  Pool& operator=(Pool &&) = delete;

  class Object;

  void resize(size_t numberOfObjectStored);
  
  template<typename ...TArgs>
  Object acquire(TArgs &&...p_args);

private:
  struct Slot {
    Slot();
    Slot(Slot &&rhs);
    std::size_t cnt;
    alignas(TType) std::byte buf[sizeof(TType)];
  };
  using Buffer = std::vector<Slot>;
  Buffer _buffer;
};

template <typename TType>
class Pool<TType>::Object {
  public:
  Object();
  Object(Object const &rhs);
  Object(Object &&rhs);
  ~Object();

  Object &operator=(Object const &rhs);
  Object &operator=(Object &&rhs);
  TType* operator->();
  TType& operator*();

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
void Pool<TType>::resize(size_t numberOfObjectStored) {
  if (numberOfObjectStored < _buffer.size())
    throw std::runtime_error("Cannot shrink pool");
  _buffer.resize(numberOfObjectStored);
}

template <typename TType>
template<typename ...TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs &&...p_args) {
  for (auto i = 0; i < _buffer.size(); ++i) {
    Slot &slot = _buffer[i];
    if (!slot.cnt) {
      new (&slot.buf) TType(std::forward<TArgs>(p_args)...);
      ++slot.cnt;
      return Pool<TType>::Object(this, i);
    }
  }
  throw std::runtime_error("Pool exhausted");
}

template <typename TType>
Pool<TType>::Slot::Slot()
  : cnt(0) {
}

template <typename TType>
Pool<TType>::Slot::Slot(Slot &&rhs)
  : cnt(rhs.cnt) {
  rhs.cnt = 0;
  if (cnt) {
    new (buf) TType(std::move(*std::launder(reinterpret_cast<TType *>(rhs.buf))));
    std::launder(reinterpret_cast<TType *>(rhs.buf))->~TType();
  }
}

template <typename TType>
Pool<TType>::Object::Object()
  : _pool(nullptr)
  , _index(0) {
}

template <typename TType>
Pool<TType>::Object::Object(Pool *pool, std::size_t index)
  : _pool(pool)
  , _index(index) {
}

template <typename TType>
Pool<TType>::Object::Object(Object const &rhs)
  : _pool(rhs._pool)
  , _index(rhs._index) {
  if (_pool) {
    Slot& slot = _pool->_buffer[_index];
    ++slot.cnt;
  }
}

template <typename TType>
Pool<TType>::Object::Object(Object &&rhs)
  : _pool(rhs._pool)
  , _index(rhs._index) {
  if (_pool) {
    rhs._pool = nullptr;
    rhs._index = 0;
  }
}

template <typename TType>
Pool<TType>::Object::~Object() {
  if (_pool) {
    Slot& slot = _pool->_buffer[_index];
    if (!--slot.cnt)
      std::launder(reinterpret_cast<TType *>(slot.buf))->~TType();
  }
}

template <typename TType>
typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object const &rhs) {
  if (this != &rhs)
    *this = std::move(Object(rhs));
  return *this;
}

template <typename TType>
typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object &&rhs) {
  if (this != &rhs) {
    if (_pool) {
      Slot &slot = _pool->_buffer[_index];
      if (!--slot.cnt)
        std::launder(reinterpret_cast<TType *>(slot.buf))->~TType();
    }
    _pool  = rhs._pool;
    _index = rhs._index;
    rhs._pool = nullptr;
    rhs._index = 0;
  }
  return *this;
}

template <typename TType>
TType* Pool<TType>::Object::operator->() {
  Slot &slot = _pool->_buffer[_index];
  return std::launder(reinterpret_cast<TType *>(slot.buf));
}

template <typename TType>
TType &Pool<TType>::Object::operator*() {
  Slot &slot = _pool->_buffer[_index];
  return *std::launder(reinterpret_cast<TType *>(slot.buf));
}
