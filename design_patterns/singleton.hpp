/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 12:26:59 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/02 11:40:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <optional>

template<typename TType>
class Singleton {
public:
  Singleton() = delete;
  ~Singleton() = delete;
  Singleton(Singleton const&) = delete;
  Singleton& operator=(Singleton const&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

  static TType* instance();
  template<typename ...TArgs> 
  static void instantiate(TArgs &&...p_args);

private:
  static std::optional<TType> _instance;
};

template<typename TType>
std::optional<TType> Singleton<TType>::_instance;

template<typename TType>
TType* Singleton<TType>::instance() {
  return _instance ? &*_instance : nullptr;
}

template<typename TType>
template<typename ...TArgs> 
void Singleton<TType>::instantiate(TArgs &&...p_args) {
  if (_instance)
    throw std::runtime_error("already initialized");
  else
    _instance.emplace(std::forward<TArgs>(p_args)...);
}
