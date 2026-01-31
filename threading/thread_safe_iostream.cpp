/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:58 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/31 23:29:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"

template class BasicThreadSafeStreamBuf<char>;
template class BasicThreadSafeStreamBuf<wchar_t>;

template class BasicThreadSafeOStream<char>;
template class BasicThreadSafeOStream<wchar_t>;

static ThreadSafeStreamBuf _tsCoutBuf(std::cout.rdbuf());
thread_local ThreadSafeOStream threadSafeCout(&_tsCoutBuf);

static ThreadSafeWStreamBuf _tsWCoutBuf(std::wcout.rdbuf());
thread_local ThreadSafeWOStream threadSafeWCout(&_tsWCoutBuf);
