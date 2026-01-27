/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 14:42:43 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/27 12:02:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include <mutex>
#include <unordered_map>
#include <string>

template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicThreadSafeStreamBuf : public std::basic_streambuf<CharT, Traits> {
public:
  typedef CharT char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  BasicThreadSafeStreamBuf(std::basic_streambuf<CharT, Traits> *dest);
  ~BasicThreadSafeStreamBuf();

  void setPrefix(std::basic_string<CharT, Traits> const &prefix);
  void resetLine();

protected:
  virtual int_type overflow(int_type ch = traits_type::eof());
  virtual std::streamsize xsputn(char_type const *s, std::streamsize n);
  virtual int sync();

private:
  std::basic_streambuf<CharT, Traits> *_dest;
  std::basic_string<CharT, Traits> _prefix;
  std::unique_lock<std::mutex> _lock;
  std::shared_ptr<std::mutex> _mtx; 
  
  using StreambufMutexMap
    = std::unordered_map<std::basic_streambuf<CharT, Traits> *, std::weak_ptr<std::mutex>>;
  static StreambufMutexMap _map;
  static std::mutex _mapMtx;
};

using ThreadSafeStreamBuf = BasicThreadSafeStreamBuf<char>;
using ThreadSafeWStreamBuf = BasicThreadSafeStreamBuf<wchar_t>;

extern template class BasicThreadSafeStreamBuf<char>;
extern template class BasicThreadSafeStreamBuf<wchar_t>;

template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicThreadSafeOStream : public std::basic_ostream<CharT, Traits> {
public:
  typedef CharT char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  explicit BasicThreadSafeOStream(BasicThreadSafeStreamBuf<CharT, Traits> *buf);

  void setPrefix(std::basic_string<CharT, Traits> const &prefix);
  void resetLine();

private:
  using std::basic_ostream<CharT, Traits>::rdbuf;
};

using ThreadSafeOStream = BasicThreadSafeOStream<char>;
using ThreadSafeWOStream = BasicThreadSafeOStream<wchar_t>;

extern template class BasicThreadSafeOStream<char>;
extern template class BasicThreadSafeOStream<wchar_t>;

extern thread_local ThreadSafeOStream threadSafeCout;
extern thread_local ThreadSafeWOStream threadWSafeCout;

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::StreambufMutexMap
BasicThreadSafeStreamBuf<CharT, Traits>::_map;

template <typename CharT, typename Traits>
std::mutex BasicThreadSafeStreamBuf<CharT, Traits>::_mapMtx;

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::BasicThreadSafeStreamBuf(
  std::basic_streambuf<CharT, Traits> *dest)
  : _dest(dest) {
  std::lock_guard<std::mutex> guard(_mapMtx);
  auto it = _map.find(_dest);
  if (it != _map.end())
    _mtx = it->second.lock();
  else
    _map[_dest] = _mtx = std::make_shared<std::mutex>();
  _lock = std::unique_lock<std::mutex>(*_mtx, std::defer_lock);
}

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::~BasicThreadSafeStreamBuf() {
  if (_lock.owns_lock())
    _lock.unlock();
  std::lock_guard<std::mutex> guard(_mapMtx);
  auto it = _map.find(_dest);
  _mtx.reset();
  if (it != _map.end() && it->second.expired())
    _map.erase(it);
}

template <typename CharT, typename Traits>
void BasicThreadSafeStreamBuf<CharT, Traits>::setPrefix(
  std::basic_string<CharT, Traits> const &prefix) {
  _prefix = prefix;
}

template <typename CharT, typename Traits>
void BasicThreadSafeStreamBuf<CharT, Traits>::resetLine() {
  if (_lock.owns_lock())
    _lock.unlock();
}

template <typename CharT, typename Traits>
BasicThreadSafeStreamBuf<CharT, Traits>::int_type
BasicThreadSafeStreamBuf<CharT, Traits>::overflow(int_type ch) {
  if (traits_type::eq_int_type(ch, traits_type::eof()))
    return traits_type::not_eof(ch);
  if (!_lock.owns_lock()) {
    _lock.lock();
    _dest->sputn(_prefix.data(), _prefix.size());
  }
  _dest->sputc(traits_type::to_char_type(ch));
  if (traits_type::eq(traits_type::to_char_type(ch), char_type('\n')))
    _lock.unlock();
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
        if (!_lock.owns_lock()) {
          _lock.lock();
          _dest->sputn(_prefix.data(), _prefix.size());
        }
        _dest->sputn(s, n);
        total += n;
      }
      break;
    }
    std::streamsize chunk = (nl - s) + 1;
    if (!_lock.owns_lock()) {
        _lock.lock();
        _dest->sputn(_prefix.data(), _prefix.size());
    }
    _dest->sputn(s, chunk);
    if (_lock.owns_lock())
      _lock.unlock();
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

template<typename T>
void prompt(std::string const &question, T &dest) {
  std::string tmp;
  for (;;) {
    threadSafeCout << question;
    std::string tmp;
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
