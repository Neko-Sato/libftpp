/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 10:47:25 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 02:05:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./data_buffer.hpp"

DataBuffer::DataBuffer(std::byte const *data, std::size_t size)
  : _buffer(data, data + size) {
}

DataBuffer& DataBuffer::operator<<(std::string const &rhs) {
  *this << rhs.size();
  auto begin = reinterpret_cast<std::byte const *>(rhs.data());
  auto end = begin + rhs.size();
  _buffer.insert(_buffer.end(), begin, end);
  return *this;
}

DataBuffer& DataBuffer::operator>>(std::string &rhs) {
  std::string::size_type size;
  *this >> size;
  rhs.resize(size);
  if (_buffer.size() < size)
    throw std::runtime_error("not enough bytes");
  auto dest = reinterpret_cast<std::byte *>(rhs.data());
  std::copy_n(_buffer.begin(), size, dest);
  _buffer.erase(_buffer.begin(), _buffer.begin() + size);
  return *this;
}

std::vector<std::byte> DataBuffer::data() const {
  return std::vector(_buffer.begin(), _buffer.end());
}
