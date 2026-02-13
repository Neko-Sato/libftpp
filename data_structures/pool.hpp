/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 05:27:18 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/13 18:04:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <cassert>
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

  void resize(size_t numberOfObjectStored);
  template<typename ...TArgs>
  Object acquire(TArgs &&...p_args);

private:
  class Slot;
  using Buffer = std::vector<Slot>;
  Buffer _buffer;
};

template <typename TType>
class Pool<TType>::Slot {
public:
  Slot();
  Slot(Slot const &) = delete;
  Slot(Slot &&rhs);
  ~Slot();
  Slot &operator=(Slot const &) = delete;
  Slot &operator=(Slot &&) = delete;
  template<typename ...TArgs>
  void emplace(TArgs &&...p_args);
  void increment();
  void decrement();
  std::size_t count() const;
  TType *ptr();
  TType const *ptr() const;

private:
  std::size_t _count;
  alignas(TType) std::byte _data[sizeof(TType)];
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
  TType *operator->();
  TType const *operator->() const;

private:
  Object(Buffer *buffer, std::size_t index);

  Buffer *_buffer;
  std::size_t _index;
  friend class Pool;
};

template <typename TType>
Pool<TType>::Slot::Slot()
  : _count(0) {
}

template <typename TType>
Pool<TType>::Slot::Slot(Slot &&rhs)
  : _count(0) {
  if (rhs._count) {
    new (_data) TType(std::move(*rhs.ptr()));
    rhs.ptr()->~TType();
    _count = std::exchange(rhs._count, 0);
  }
}

template <typename TType>
Pool<TType>::Slot::~Slot() {
  if (_count)
    ptr()->~TType();
}

template <typename TType>
template<typename ...TArgs>
void Pool<TType>::Slot::emplace(TArgs &&...p_args) {
  assert(!_count);
  new (_data) TType(std::forward<TArgs>(p_args)...);
  ++_count;
}

template <typename TType>
void Pool<TType>::Slot::increment() {
  assert(_count);
  ++_count;
}

template <typename TType>
void Pool<TType>::Slot::decrement() {
  assert(_count);
  if (!--_count)
    ptr()->~TType();
}

template <typename TType>
std::size_t Pool<TType>::Slot::count() const {
  return _count;
}

template <typename TType>
TType *Pool<TType>::Slot::ptr() {
  return std::launder(reinterpret_cast<TType *>(_data));
}

template <typename TType>
TType const *Pool<TType>::Slot::ptr() const {
  return std::launder(reinterpret_cast<TType const *>(_data));
}

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
  auto it = std::find_if(_buffer.begin(), _buffer.end(),
    [](auto const &slot) { return !slot.count(); });  
  if (it == _buffer.end())
    throw std::runtime_error("Pool exhausted");
  it->emplace(std::forward<TArgs>(p_args)...);
  return Pool<TType>::Object(&_buffer, std::distance(_buffer.begin(), it));
}

template <typename TType>
Pool<TType>::Object::Object()
  : Object(nullptr, 0) {
}

template <typename TType>
Pool<TType>::Object::Object(Buffer *buffer, std::size_t index)
  : _buffer(buffer)
  , _index(index) {
}

template <typename TType>
Pool<TType>::Object::Object(Object const &rhs)
  : _buffer(rhs._buffer)
  , _index(rhs._index) {
  if (_buffer)
    _buffer->at(_index).increment();
}

template <typename TType>
Pool<TType>::Object::Object(Object &&rhs)
  : _buffer(std::exchange(rhs._buffer, nullptr))
  , _index(std::exchange(rhs._index, 0)) {
}

template <typename TType>
Pool<TType>::Object::~Object() {
  if (_buffer)
    _buffer->at(_index).decrement();
}

template <typename TType>
typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object const &rhs) {
  if (this != &rhs)
    *this = Object(rhs);
  return *this;
}

template <typename TType>
typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object &&rhs) {
  if (this != &rhs) {
    if (_buffer)
      _buffer->at(_index).decrement();
    _buffer = std::exchange(rhs._buffer, nullptr);
    _index = std::exchange(rhs._index, 0);
  }
  return *this;
}

template <typename TType>
TType *Pool<TType>::Object::operator->() {
  assert(_buffer && _buffer->at(_index).count());
  return _buffer->at(_index).ptr();
}

template <typename TType>
TType const *Pool<TType>::Object::operator->() const {
  assert(_buffer && _buffer->at(_index).count());
  return _buffer->at(_index).ptr();
}
