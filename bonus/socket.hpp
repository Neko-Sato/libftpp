/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:23:41 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:26:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <cstddef>

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
