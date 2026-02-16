/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:16:41 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 08:52:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include <format>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <sys/poll.h>

// OSError
OSError::OSError(int __errno, std::string const &s)
    : _errno(__errno), _s(std::format("{}: {}", s, ::strerror(_errno))) {
}

int OSError::getErrno() const {
  return _errno;
}

char const *OSError::what() const noexcept {
  return _s.c_str();
}

// Selector
Selector::event const Selector::READ = 1;
Selector::event const Selector::WRITE = 2;
Selector::event const Selector::EXCEPT = 4;

void Selector::add(int fd, event events) {
  auto [it, inserted] = _fds.try_emplace(fd, events);
  if (!inserted)
    throw RegisteredError();
}

void Selector::remove(int fd) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  _fds.erase(it);
}

void Selector::modify(int fd, event events) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  it->second = events;
}

Selector::Mapping const &Selector::getMap() const {
  return _fds;
}

char const *Selector::RegisteredError::what() const throw() {
  return "already registered";
}

char const *Selector::NotRegisteredError::what() const throw() {
  return "not registered";
}

// PollSelector
Selector::Events PollSelector::select(int timeout) const {
  Mapping const &map = getMap();
  using Pollfds = std::vector<pollfd>;
  Pollfds fds;
  fds.reserve(map.size());
  for (auto &entry : map) {
    pollfd fd;
    fd.fd = entry.first;
    fd.events = POLLERR | POLLHUP;
    if (entry.second & READ)
      fd.events |= POLLIN;
    if (entry.second & WRITE)
      fd.events |= POLLOUT;
    fds.push_back(fd);
  }
  int nfds = poll(fds.data(), fds.size(), timeout);
  if (nfds == -1)
    throw OSError(errno, "poll");
  Events events;
  for (auto &entry : fds) {
    event_details tmp;
    tmp.fd = entry.fd;
    tmp.events = 0;
    if (entry.revents & POLLIN)
      tmp.events |= READ;
    if (entry.revents & POLLOUT)
      tmp.events |= WRITE;
    if (entry.revents & (POLLERR | POLLHUP))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp);
  }
  return events;
}

// Socket
int Socket::_makeSockfd(int domain, int type, int protocol) {
  int sockfd = ::socket(domain, type, protocol);
  if (sockfd == -1)
    throw OSError(errno, "socket");
  return sockfd;
}

Socket::Socket() : _sockfd(-1) {
}

Socket::Socket(int domain, int type, int protocol)
  : _sockfd(_makeSockfd(domain, type, protocol)) {
}

Socket::Socket(int sockfd) : _sockfd(sockfd) {
}

Socket::~Socket() {
  close();
}

Socket::operator bool() const {
  return _sockfd != -1;
}

int Socket::fd() const {
  return _sockfd;
}

void Socket::bind(sockaddr const *addr, socklen_t addrlen) {
  if (::bind(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "bind");
}

void Socket::connect(sockaddr const *addr, socklen_t addrlen) {
  if (::connect(_sockfd, addr, addrlen) == -1) 
    throw OSError(errno, "connect");
}

void Socket::listen(int backlog) {
  if (::listen(_sockfd, backlog) == -1)
    throw OSError(errno, "listen");
}

Socket Socket::accept() {
  int connfd = ::accept(_sockfd, NULL, 0);
  if (connfd == -1)
    throw OSError(errno, "accept");
  return Socket(connfd);
}

std::size_t Socket::write(void const *buf, std::size_t len) {
  ssize_t ret = ::write(_sockfd, buf, len);
  if (ret == -1)
    throw OSError(errno, "write");
  return ret;
}

std::size_t Socket::read(void *buf, std::size_t len) {
  ssize_t ret = ::read(_sockfd, buf, len);
  if (ret == -1)
    throw OSError(errno, "read");
  return ret;
}

std::size_t Socket::send(void const *buf, std::size_t len, int flags) {
  ssize_t ret = ::send(_sockfd, buf, len, flags);
  if (ret == -1)
    throw OSError(errno, "send");
  return ret;
}

std::size_t Socket::recv(void *buf, std::size_t len, int flags) {
  ssize_t ret = ::recv(_sockfd, buf, len, flags);
  if (ret == -1)
    throw OSError(errno, "recv");
  return ret;
}

std::size_t Socket::sendto(void const *buf, std::size_t len, int flags,
                           sockaddr const *dest_addr, socklen_t addrlen) {
  ssize_t ret = ::sendto(_sockfd, buf, len, flags, dest_addr, addrlen);
  if (ret == -1)
    throw OSError(errno, "sendto");
  return ret;
}

std::size_t Socket::recvfrom(void *buf, std::size_t len, int flags,
                             sockaddr *src_addr, socklen_t *addrlen) {
  ssize_t ret = ::recvfrom(_sockfd, buf, len, flags, src_addr, addrlen);
  if (ret == -1)
    throw OSError(errno, "recvfrom");
  return ret;
}

void Socket::close() {
  if (_sockfd != -1) {
    if (::close(_sockfd) == -1)
      throw OSError(errno, "close");
    _sockfd = -1;
  }
}

void Socket::shutdown(int how) {
  if (::shutdown(_sockfd, how) == -1)
    throw OSError(errno, "shutdown");
}

void Socket::getsockname(sockaddr *addr, socklen_t *addrlen) {
  if (::getsockname(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "getsockname");
}

void Socket::getpeername(sockaddr *addr, socklen_t *addrlen) {
  if (::getpeername(_sockfd, addr, addrlen) == -1)
    throw OSError(errno, "getpeername");
}

void Socket::getsockopt(int level, int optname, void *optval,
                        socklen_t *optlen) {
  if (::getsockopt(_sockfd, level, optname, optval, optlen) == -1)
    throw OSError(errno, "getsockopt");
}

void Socket::setsockopt(int level, int optname, void const *optval,
                        socklen_t optlen) {
  if (::setsockopt(_sockfd, level, optname, optval, optlen) == -1)
    throw OSError(errno, "setsockopt");
}

// AddrInfos
AddrInfos::iterator::iterator(addrinfo const *info)
  : _current(info) {
}

AddrInfos::iterator &AddrInfos::iterator::operator++() {
  _current = _current->ai_next;
  return *this;
}

AddrInfos::iterator AddrInfos::iterator::operator++(int) {
  iterator tmp(*this);
  _current = _current->ai_next;
  return tmp;
}

bool AddrInfos::iterator::operator==(iterator const &rhs) const {
  return _current == rhs._current;
}

bool AddrInfos::iterator::operator!=(iterator const &rhs) const {
  return _current != rhs._current;
}

addrinfo const &AddrInfos::iterator::operator*() const {
  return *_current;
}

addrinfo const *AddrInfos::iterator::operator->() const {
  return _current;
}

AddrInfos::Hints::Hints(int family, int socktype, int protocol, int flags) {
  ai_family = family;
  ai_socktype = socktype;
  ai_protocol = protocol;
  ai_flags = flags;
}

AddrInfos::Error::Error(int __errno)
    : _errno(__errno)
    , _s(std::format("getaddrinfo: {}", gai_strerror(_errno))) {
}

int AddrInfos::Error::getErrno() const {
  return _errno;
}

char const *AddrInfos::Error::what() const noexcept {
  return _s.c_str();
}

addrinfo *AddrInfos::_getaddrinfo(char const *name, char const *service,
                                  addrinfo const *hints) {
  addrinfo *res;
  int err = ::getaddrinfo(name, service, hints, &res);
  if (err != 0)
    throw Error(err);
  return res;
}

AddrInfos::AddrInfos()
  : _addrinfo(nullptr) {
}

AddrInfos::AddrInfos(char const *name, char const *service)
  : _addrinfo(_getaddrinfo(name, service)) {
}

AddrInfos::AddrInfos(char const *name, char const *service, Hints const &hints)
  : _addrinfo(_getaddrinfo(name, service, &hints)) {
}

AddrInfos::~AddrInfos() {
  if (_addrinfo)
    freeaddrinfo(_addrinfo);
}

AddrInfos::iterator AddrInfos::begin() const {
  return iterator(_addrinfo);
}

AddrInfos::iterator AddrInfos::end() const {
  return iterator(NULL);
}

// utils
void setblocking(int fd, bool blocking) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    throw OSError(errno, "fcntl");
  if ((!!(flags & O_NONBLOCK)) ^ !blocking)
    if (fcntl(fd, F_SETFL, flags ^ O_NONBLOCK) == -1)
      throw OSError(errno, "fcntl");
}

// // AsyncTCPClient
// AsyncConnection::AsyncConnection(Socket &&socket)
//   : _socket(std::move(socket)) {
// }

// Async<void> AsyncConnection::start(Socket &&socket, Handler const &handler) {
//   AsyncConnection client(std::move(socket));
//   for (auto session = handler(client); !session.done(); session.resume()) {
//     //ここでポーリングを行なってサーバを
//   }
// }


// Socket &AsyncConnection::socket() {
//   return _socket;
// }

// void AsyncConnection::send(std::byte const *buffer, std::size_t size) {
//   if (_wbuf.empty()) {
//     try {
//       std::size_t writen = _socket.write(buffer, size);
//       buffer += writen;
//       size -= writen;
//       if (!size)
//         return;
//     } catch (...) {
//     }
//   }
//   _wbuf.insert(_wbuf.end(), buffer, buffer + size);
// }

// Socket AsyncTCPClient::_makeAyncSocket(std::string const &address, size_t port) {
//   AddrInfos infos(address.c_str(), std::to_string(port).c_str(),
//     AddrInfos::Hints(AF_UNSPEC, SOCK_STREAM, 0, 0));
//   for (auto &info : infos) {
//     try {
//       Socket tmp(info.ai_family, info.ai_socktype, info.ai_protocol);
//       setblocking(tmp.fd(), true);
//       try {
//         tmp.connect(info.ai_addr, info.ai_addrlen);
//       } catch (OSError const &e) {
//         if (e.getErrno() != EINPROGRESS)
//           throw ;
//       }
//       return tmp;
//     } catch (...) {
//     }
//   }
//   throw std::runtime_error("connection failed");
// }

// std::deque<std::byte> &AsyncTCPClient::recvBuffer() {
//   return 
// }

/// Client
// void Client::connect(std::string const &address, size_t const &port) {
//   if (_socket)
//     throw std::runtime_error("already connected");
//   AddrInfos infos(address.c_str(), std::to_string(port).c_str(),
//     AddrInfos::Hints(AF_UNSPEC, SOCK_STREAM, 0, 0));
//   for (auto &info : infos) {
//     try {
//       Socket tmp(info.ai_family, info.ai_socktype, info.ai_protocol);
//       setblocking(tmp.fd(), true);
//       try {
//         tmp.connect(info.ai_addr, info.ai_addrlen);
//       } catch (OSError const &e) {
//         if (e.getErrno() != EINPROGRESS)
//           throw ;
//       }
//       _socket = std::move(tmp);
//       return ;
//     } catch (...) {
//     }
//   }
//   throw std::runtime_error("connection failed");
// }

// void Client::disconnect() {
//   if (!_socket)
//     throw std::runtime_error("not connected");
//   _socket.close();
//   _buffer.clear();
// }

// void Client::defineAction(Message::Type const &messageType, Action const &action) {
//   auto [it, inserted] = _actions.try_emplace(messageType, action);
//   if (inserted)
//     it->second = action;
// }

// void Client::send(Message const &message) {
//   if (!_socket)
//     throw std::runtime_error("not connected");
//   std::vector<std::byte> data = message.data();
//   std::size_t size = data.size();
//   ::write(_sockfd, &size, sizeof(size));
//   ::write(_sockfd, data.data(), data.size());

// }