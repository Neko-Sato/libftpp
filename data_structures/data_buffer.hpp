/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 09:15:40 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 02:09:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <type_traits>
#include <span>
#include <string>
#include <deque>
#include <vector>
#include <exception>

class DataBuffer {
public:
  DataBuffer() = default;
  DataBuffer(std::byte const *data, std::size_t size);

  template <typename T>
  requires std::is_trivially_copyable_v<std::remove_cvref_t<T>>  
  DataBuffer& operator<<(T const &rhs);
  DataBuffer& operator<<(std::string const &rhs);

  template <typename T>
  requires std::is_trivially_copyable_v<std::remove_cvref_t<T>>  
  DataBuffer& operator>>(T &rhs);
  DataBuffer& operator>>(std::string &rhs);

  std::vector<std::byte> data() const;

private:
  std::deque<std::byte> _buffer;
};

template <typename T>
requires std::is_trivially_copyable_v<std::remove_cvref_t<T>>
DataBuffer& DataBuffer::operator<<(T const &rhs) {
  std::byte const *p = reinterpret_cast<std::byte const*>(&rhs);
  _buffer.insert(_buffer.end(), p, p + sizeof(T));
  return *this;
}

template <typename T>
requires std::is_trivially_copyable_v<std::remove_cvref_t<T>>
DataBuffer& DataBuffer::operator>>(T &rhs) {
  if (_buffer.size() < sizeof(T))
    throw std::runtime_error("not enough bytes");
  std::copy_n(_buffer.begin(), sizeof(T), reinterpret_cast<std::byte *>(&rhs));
  _buffer.erase(_buffer.begin(), _buffer.begin() + sizeof(T));
  return *this;
}
