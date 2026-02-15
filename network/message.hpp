/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 00:51:41 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 04:05:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "data_structures/data_buffer.hpp"

class Message {
public:
  using Type = int;

  Message(Type type = 0);
  static std::vector<std::byte> srialize(Message const &message);
  static Message deserialize(std::byte const *data, std::size_t size);

  Type type() const;

  template <typename T>
  Message& operator<<(T const &value);
  template <typename T>
  Message& operator>>(T& value);

private:
  Message(Type type, std::byte const *data, std::size_t size);

  Type _type;
  DataBuffer _buffer;
};

template <typename T>
Message& Message::operator<<(T const &value) {
  _buffer << value;
}

template <typename T>
Message& Message::operator>>(T &value) {
  _buffer >> value;
}
