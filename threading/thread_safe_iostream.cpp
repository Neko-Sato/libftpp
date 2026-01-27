/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:58 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/27 10:36:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"

template class BasicThreadSafeStreamBuf<char>;
template class BasicThreadSafeStreamBuf<wchar_t>;

template class BasicThreadSafeOStream<char>;
template class BasicThreadSafeOStream<wchar_t>;

static thread_local ThreadSafeStreamBuf g_tsbuf(std::cout.rdbuf());
thread_local ThreadSafeOStream threadSafeCout(&g_tsbuf);

static thread_local ThreadSafeWStreamBuf g_tswbuf(std::wcout.rdbuf());
thread_local ThreadSafeWOStream threadSafeWCout(&g_tswbuf);
