/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 14:42:43 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:18:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "bonus/tls.hpp"
#include <iostream>
#include <sstream>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>

template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicThreadSafeStreamBuf : public std::basic_streambuf<CharT, Traits> {
public:
  using char_type = CharT;
  using traits_type = Traits;
  using int_type = typename traits_type::int_type;
  using pos_type = typename traits_type::pos_type;
  using off_type = typename traits_type::off_type;

  BasicThreadSafeStreamBuf(std::basic_streambuf<CharT, Traits> *dest);

  void setPrefix(std::basic_string<CharT, Traits> const &prefix);
  void resetLine();

protected:
  virtual int_type overflow(int_type ch = traits_type::eof());
  virtual std::streamsize xsputn(char_type const *s, std::streamsize n);
  virtual int sync();

private:
  std::basic_streambuf<CharT, Traits> *_dest;
  mutable std::mutex _mtx;

  struct Local {
    std::basic_string<CharT, Traits> prefix;
    std::unique_lock<std::mutex> lock;
  };
  tls<Local> _local;
  void _tryLock();
};

using ThreadSafeStreamBuf = BasicThreadSafeStreamBuf<char>;
using ThreadSafeWStreamBuf = BasicThreadSafeStreamBuf<wchar_t>;

template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicThreadSafeOStream : public std::basic_ostream<CharT, Traits> {
public:
  using char_type = CharT;
  using traits_type = Traits;
  using int_type = typename traits_type::int_type;
  using pos_type = typename traits_type::pos_type;
  using off_type = typename traits_type::off_type;

  BasicThreadSafeOStream(BasicThreadSafeStreamBuf<CharT, Traits> *buf);

  void setPrefix(std::basic_string<CharT, Traits> const &prefix);
  void resetLine();

  BasicThreadSafeStreamBuf<CharT, Traits>* rdbuf() const;
  BasicThreadSafeOStream<CharT, Traits>& rdbuf(BasicThreadSafeStreamBuf<CharT, Traits> *sb);
};

using ThreadSafeOStream = BasicThreadSafeOStream<char>;
using ThreadSafeWOStream = BasicThreadSafeOStream<wchar_t>;

extern template class BasicThreadSafeStreamBuf<char>;
extern template class BasicThreadSafeStreamBuf<wchar_t>;

extern template class BasicThreadSafeOStream<char>;
extern template class BasicThreadSafeOStream<wchar_t>;

extern thread_local ThreadSafeOStream threadSafeCout;
extern thread_local ThreadSafeWOStream threadSafeWCout;

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::BasicThreadSafeStreamBuf(
  std::basic_streambuf<CharT, Traits> *dest)
  : _dest(dest) {
}

template <typename CharT, typename Traits>
void BasicThreadSafeStreamBuf<CharT, Traits>::_tryLock() {
  auto &local = _local.get();
  if (!local.lock.owns_lock()) {
    if (!local.lock.mutex())
      local.lock = std::unique_lock<std::mutex>(_mtx, std::defer_lock);
    local.lock.lock();
    _dest->sputn(local.prefix.data(), local.prefix.size());
  }
}

template <typename CharT, typename Traits>
void BasicThreadSafeStreamBuf<CharT, Traits>::setPrefix(
  std::basic_string<CharT, Traits> const &prefix) {
  _local.get().prefix = prefix;
}

template <typename CharT, typename Traits>
void BasicThreadSafeStreamBuf<CharT, Traits>::resetLine() {
  auto &local = _local.get();
  if (local.lock.owns_lock())
    local.lock.unlock();
}

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::int_type
BasicThreadSafeStreamBuf<CharT, Traits>::overflow(int_type ch) {
  if (traits_type::eq_int_type(ch, traits_type::eof()))
    return traits_type::not_eof(ch);
  _tryLock();
  _dest->sputc(traits_type::to_char_type(ch));
  if (traits_type::eq(traits_type::to_char_type(ch), char_type('\n')))
    resetLine();
  return traits_type::not_eof(ch);
}

template <typename CharT, typename Traits>
std::streamsize BasicThreadSafeStreamBuf<CharT, Traits>::xsputn(
  char_type const *s, std::streamsize n) {
  std::streamsize total = 0;
  while (n > 0) {
    char_type const*nl = std::find(s, s + n, char_type('\n'));
    if (nl == s + n) {
      if (n) {
        _tryLock();
        _dest->sputn(s, n);
        total += n;
      }
      break;
    }
    std::streamsize chunk = (nl - s) + 1;
    _tryLock();
    _dest->sputn(s, chunk);
    resetLine();
    s += chunk;
    n -= chunk;
    total += chunk;
  }
  return total;
}

template <typename CharT, typename Traits>
int BasicThreadSafeStreamBuf<CharT, Traits>::sync() {
  return _dest->pubsync();
}

template <typename CharT, typename Traits>
BasicThreadSafeOStream<CharT, Traits>::BasicThreadSafeOStream(
  BasicThreadSafeStreamBuf<CharT, Traits> *buf)
  : std::basic_ostream<CharT, Traits>(buf) {
}

template <typename CharT, typename Traits>
void BasicThreadSafeOStream<CharT, Traits>::setPrefix(
  std::basic_string<CharT, Traits> const &prefix) {
  static_cast<BasicThreadSafeStreamBuf<CharT, Traits> *>(rdbuf())->setPrefix(prefix);
}

template <typename CharT, typename Traits>
void BasicThreadSafeOStream<CharT, Traits>::resetLine() {
  static_cast<BasicThreadSafeStreamBuf<CharT, Traits> *>(rdbuf())->resetLine();
}

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>* 
BasicThreadSafeOStream<CharT, Traits>::rdbuf() const {
  return static_cast<BasicThreadSafeStreamBuf<CharT, Traits> *>(
    std::basic_ostream<CharT, Traits>::rdbuf()
  );
}

template <typename CharT, typename Traits>
BasicThreadSafeOStream<CharT, Traits>& 
BasicThreadSafeOStream<CharT, Traits>::rdbuf(BasicThreadSafeStreamBuf<CharT, Traits>* sb) {
  std::basic_ostream<CharT, Traits>::rdbuf(sb);
  return *this;
}

template<typename T>
void prompt(std::string const &question, T &dest) {
  std::string tmp;
  for (;;) {
    threadSafeCout << question;
    std::getline(std::cin, tmp);
    threadSafeCout.resetLine();
    if (!std::cin)
      throw std::runtime_error("input stream error");
    std::istringstream iss(tmp);
    iss >> dest;
    if (iss)
      break;
  }
}
