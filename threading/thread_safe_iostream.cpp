/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:58 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 19:38:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"
#include <iostream>

std::mutex ThreadSafeBuf::_mtx;
thread_local std::unique_lock<std::mutex>
  ThreadSafeBuf::_lk(ThreadSafeBuf::_mtx, std::defer_lock);
thread_local std::string ThreadSafeBuf::_prefix;


ThreadSafeBuf::int_type ThreadSafeBuf::overflow(int_type ch) {
  if (traits_type::eq_int_type(ch, traits_type::eof()))
    return traits_type::not_eof(ch);
  try {
    if (!_lk.owns_lock()) {
      _lk.lock();
      std::cout.write(_prefix.c_str(), _prefix.size());
    }
    std::cout.put(ch);
    if (ch == '\n') {
      _lk.unlock();
    }
  } catch (...) {
    if (_lk.owns_lock())
      _lk.unlock();
    throw;
  }
  return traits_type::not_eof(ch);
}

std::streamsize ThreadSafeBuf::xsputn(char const *s, std::streamsize n) {
  std::streamsize written = 0;
  for (std::streamsize i = 0; i < n; ++i) {
    if (overflow(s[i]) == traits_type::eof())
      break;
    ++written;
  }
  return written;
}

void ThreadSafeBuf::setPrefix(std::string const prefix) {
  _prefix = prefix;
}

void ThreadSafeCout::setPrefix(std::string const prefix) {
  static_cast<ThreadSafeBuf *>(rdbuf())->setPrefix(prefix);
}

ThreadSafeCout::ThreadSafeCout(ThreadSafeBuf *buf)
  : std::ostream(buf) {
}

static ThreadSafeBuf g_tsbuf;
ThreadSafeCout threadSafeCout(&g_tsbuf);
