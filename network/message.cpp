/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:14:50 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/16 04:07:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./message.hpp"

Message::Message(Type type)
  : _type(type) {
}

Message::Message(Type type, std::byte const *data, std::size_t size)
  : _type(type)
  , _buffer(data, size) {
}

Message::Type Message::type() const {
  return _type;
}

std::vector<std::byte> Message::srialize(Message const &message) {
  std::vector<std::byte> tmp;
  std::byte const *typePtr = reinterpret_cast<std::byte const *>(&message._type);
  tmp.insert(tmp.end(), typePtr, typePtr + sizeof(Message::Type));
  std::vector<std::byte> data = message._buffer.data();
  tmp.insert(tmp.end(), data.begin(), data.end());
  return tmp;
}

Message Message::deserialize(std::byte const *data, std::size_t size) {
  if (size < sizeof(Message::Type))
    throw std::runtime_error("invalid binary");
  return Message(*reinterpret_cast<Message::Type const *>(data),
    data + sizeof(Message::Type), size - sizeof(Message::Type));
}
