/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 12:04:36 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 12:24:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>
#include <unordered_map>

template <typename TEvent>
class Observer {
public:
  void subscribe(TEvent const &event, std::function<void()> const &lambda);
  void notify(TEvent const &event);

private:
  using _EventCallbackMap = \
    std::unordered_multimap<TEvent, std::function<void()>>;
  _EventCallbackMap _map;
};

template <typename TEvent>
void Observer<TEvent>::subscribe(
  TEvent const &event,
  std::function<void()> const &lambda) {
  _map.emplace(event, lambda);
}

template <typename TEvent>
void Observer<TEvent>::notify(
  TEvent const &event) {
  auto [first, last] = _map.equal_range(event);
  for (auto it = first; it != last; ++it)
    it->second();
}
