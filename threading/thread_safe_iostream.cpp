/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:58 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/28 06:12:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"

template class BasicThreadSafeStreamBuf<char>;
template class BasicThreadSafeStreamBuf<wchar_t>;

template class BasicThreadSafeOStream<char>;
template class BasicThreadSafeOStream<wchar_t>;

static ThreadSafeStreamBuf g_tsbuf(std::cout.rdbuf());
thread_local ThreadSafeOStream threadSafeCout(&g_tsbuf);

static ThreadSafeWStreamBuf g_tswbuf(std::wcout.rdbuf());
thread_local ThreadSafeWOStream threadSafeWCout(&g_tswbuf);
