/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addrInfos.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:26:02 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:27:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netdb.h>
#include <exception>
#include <string>

class AddrInfos {
public:
  class iterator {
  public:
    iterator(addrinfo const *info = nullptr);
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
  