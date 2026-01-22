/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 14:42:43 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 18:47:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <streambuf>
#include <sstream>
#include <iostream>
#include <mutex>
#include <string>

class ThreadSafeBuf : public std::streambuf {
public:
  int_type overflow(int_type ch);
  std::streamsize xsputn(char const *s, std::streamsize n);
  void setPrefix(std::string const prefix);

private:
  static std::mutex _mtx;
  static thread_local std::unique_lock<std::mutex> _lk;
  static thread_local std::string _prefix;
};

class ThreadSafeCout : public std::ostream {
public:
  ThreadSafeCout(ThreadSafeBuf *buf);
  void setPrefix(std::string const prefix);
};

extern ThreadSafeCout threadSafeCout;

template<typename T>
void prompt(std::string const &question, T &dest) {
  static std::mutex mtx;
  std::lock_guard<std::mutex> lock(mtx);
  std::string tmp;
  for (;;) {
    threadSafeCout << question;
    std::cin >> dest;
    threadSafeCout << std::endl;
    if (std::cin)
      break;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}
