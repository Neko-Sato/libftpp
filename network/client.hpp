/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:16:41 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 08:56:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>
#include <unordered_map>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// OSError
class OSError : public std::exception {
public:
  OSError(int __errno = 0, std::string const &s = "OSError");

  int getErrno() const;
  char const *what() const noexcept;

private:
  int _errno;
  std::string _s;
};

// Selector
class Selector {
public:
  using event = unsigned int;
  static event const READ;
  static event const WRITE;
  static event const EXCEPT;
  struct event_details {
    int fd;
    event events;
  };
  using Mapping = std::unordered_map<int, event>;
  using Events = std::queue<event_details>;

  virtual ~Selector() = default;

  virtual void add(int fd, event events);
  virtual void remove(int fd);
  virtual void modify(int fd, event events);
  Mapping const &getMap() const;
  virtual Events select(int timeout) const = 0;

  class RegisteredError : public std::exception {
    char const *what() const throw();
  };

  class NotRegisteredError : public std::exception {
    char const *what() const throw();
  };

protected:
  Selector() = default;
  Selector(Selector const &) = delete;
  Selector(Selector &&) = default;
  Selector &operator=(Selector const &) = delete;
  Selector &operator=(Selector &&) = default;

private:
  Mapping _fds;
};

// PollSelector
class PollSelector : public Selector {
public:
  Events select(int timeout) const;
};

// Socket
class Socket {
public:
  Socket();
  Socket(int domain, int type, int protocol);
  Socket(Socket const &) = delete;
  Socket(Socket &&) = default;
  ~Socket();
  Socket &operator=(Socket const &) = delete;
  Socket &operator=(Socket &&) = default;

  operator bool() const;
  int fd() const;

  void bind(sockaddr const *addr, socklen_t addrlen);
  void connect(sockaddr const *addr, socklen_t addrlen);
  void listen(int backlog = SOMAXCONN);

  Socket accept();
  std::size_t write(void const *buf, std::size_t len);
  std::size_t read(void *buf, std::size_t len);
  std::size_t send(void const *buf, std::size_t len, int flags);
  std::size_t recv(void *buf, std::size_t len, int flags);
  std::size_t sendto(void const *buf, std::size_t len, int flags,
                      sockaddr const *dest_addr, socklen_t addrlen);
  std::size_t recvfrom(void *buf, std::size_t len, int flags,
                        sockaddr *src_addr, socklen_t *addrlen);
  void close();
  void shutdown(int how);

  void getsockname(sockaddr *addr, socklen_t *addrlen);
  void getpeername(sockaddr *addr, socklen_t *addrlen);

  void getsockopt(int level, int optname, void *optval, socklen_t *optlen);
  void setsockopt(int level, int optname, void const *optval, socklen_t optlen);

private:
  Socket(int sockfd);
  static int _makeSockfd(int domain, int type, int protocol);
  int _sockfd;

};

// AddrInfos
class AddrInfos {
public:
  class iterator {
  public:
    iterator(addrinfo const *info = NULL);
    iterator &operator++();
    iterator operator++(int);
    bool operator==(iterator const &rhs) const;
    bool operator!=(iterator const &rhs) const;
    addrinfo const &operator*() const;
    addrinfo const *operator->() const;

  private:
    addrinfo const *_current;
  };

  class Hints : private addrinfo {
  public:
    Hints() = default;
    Hints(int family, int socktype, int protocol, int flags);

    friend class AddrInfos;
  };

  class Error : public std::exception {
  private:
    int _errno;
    std::string _s;

  public:
    Error(int __errno = 0);

    int getErrno() const;
    char const *what() const noexcept;
  };

  AddrInfos();
  AddrInfos(char const *name, char const *service);
  AddrInfos(char const *name, char const *service, Hints const &hints);
  AddrInfos(AddrInfos const &) = delete; 
  AddrInfos(AddrInfos &&) = default; 
  ~AddrInfos();
  AddrInfos &operator=(AddrInfos const &) = delete; 
  AddrInfos &operator=(AddrInfos &&) = default; 

  iterator begin() const;
  iterator end() const;

private:
  addrinfo *_addrinfo;
  static addrinfo *_getaddrinfo(char const *name, char const *service,
    addrinfo const *hints = nullptr);
};

// utils
void setblocking(int fd, bool blocking);

///
// class AsyncConnection {
// public:
//   AsyncConnection(AsyncConnection const &) = delete;
//   AsyncConnection(AsyncConnection &&) = delete;
//   AsyncConnection &operator=(AsyncConnection const &) = delete;
//   AsyncConnection &operator=(AsyncConnection &&) = delete;

//   using Handler = std::function<Async<void>(AsyncConnection &)>;
//   static Async<void> start(Socket &&socket, Handler const &handler);

//   Socket &socket();
//   Async<void> recv(std::byte *buf, std::size_t);
//   void send(std::byte const *buffer, std::size_t size);
//   Async<void> drain();

// private:
//   AsyncConnection(Socket &&socket);

//   Socket _socket;
//   std::deque<std::byte> _rbuf;
//   std::vector<std::byte> _wbuf;
// };

/// Client
// class Client {
// public:
//   Client() = default;
//   Client(Client const &) = delete;
//   Client(Client &&) = default;
//   Client &operator=(Client const &) = delete;
//   Client &operator=(Client &&) = default;

//   using Action = std::function<void(Message const &msg)>;
//   void connect(std::string const &address, size_t const &port);
//   void disconnect();
//   void defineAction(Message::Type const &messageType, Action const &action);
//   void send(Message const &message);
//   void update();

// private:
//   Socket _socket;
//   std::deque<std::byte> _buffer;
//   using Actions = std::unordered_map<Message::Type, Action>;
//   Actions _actions;
// };

