/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll_selector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 09:22:09 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/17 09:23:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/poll.h>
#include "./poll_selector.hpp"
#include "./os_error.hpp"

Selector::Events PollSelector::select(int timeout) const {
  Mapping const &map = getMap();
  using Pollfds = std::vector<pollfd>;
  Pollfds fds;
  fds.reserve(map.size());
  for (auto &entry : map) {
    pollfd fd;
    fd.fd = entry.first;
    fd.events = POLLERR | POLLHUP;
    if (entry.second & READ)
      fd.events |= POLLIN;
    if (entry.second & WRITE)
      fd.events |= POLLOUT;
    fds.push_back(fd);
  }
  int nfds = poll(fds.data(), fds.size(), timeout);
  if (nfds == -1)
    throw OSError(errno, "poll");
  Events events;
  for (auto &entry : fds) {
    event_details tmp;
    tmp.fd = entry.fd;
    tmp.events = 0;
    if (entry.revents & POLLIN)
      tmp.events |= READ;
    if (entry.revents & POLLOUT)
      tmp.events |= WRITE;
    if (entry.revents & (POLLERR | POLLHUP))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp);
  }
  return events;
}