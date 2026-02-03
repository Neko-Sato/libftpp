/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 12:54:56 by hshimizu          #+#    #+#             */
/*   Updated: 2026/02/04 01:10:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>
#include <optional>
#include <unordered_set>
#include <unordered_map>

template <typename T1, typename T2>
struct PairHash {
  std::size_t operator()(std::pair<T1, T2> const &p) const;
};

template <typename TState>
class StateMachine {
public:
  StateMachine(StateMachine const &) = delete;
  StateMachine &operator=(StateMachine const &) = delete;

  void addState(TState const &state);
  void addTransition(TState const &startState, TState const &finalState,
    std::function<void()> const &lambda);
  void addAction(TState const &state, std::function<void()> const &lambda);
  void transitionTo(TState const &state);
  void update();

private:
  using _States =
    std::unordered_set<TState>;
  using _Transitions = 
    std::unordered_map<std::pair<TState, TState>, std::function<void()>, PairHash>;
  using _Actions = 
    std::unordered_map<TState, std::function<void()>>;

  _States _states;
  _Transitions _transitions;
  _Actions _actions;

  std::optional<TState> _cur;
};

template <typename T1, typename T2>
std::size_t PairHash<T1, T2>::operator()(std::pair<T1, T2> const &p) const {
  std::size_t h1 = std::hash<T1>{}(p.first);
  std::size_t h2 = std::hash<T2>{}(p.second);
  return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

template <typename TState>
void StateMachine<TState>::addState(
  TState const &state) {
  auto [it, inserted] = _states.try_emplace(state);
  if (!inserted)
    throw std::invalid_argument("state exists");
  if (!_cur)
    _cur.emplace(state);
}

template <typename TState>
void StateMachine<TState>::addTransition(
  TState const &startState,
  TState const &finalState,
  std::function<void()> const &lambda) {
  if (_states.find(startState) == _states.end() || _states.find(finalState) == _states.end())
    throw std::invalid_argument("unknown state");
  auto [it, inserted] = _transitions.try_emplace(std::make_pair(startState, finalState), lambda);
  if (!inserted)
    throw std::invalid_argument("transition exists");
}

template <typename TState>
void StateMachine<TState>::addAction(
  TState const &state,
  std::function<void()> const &lambda) {
  if (_states.find(state) == _states.end())
    throw std::invalid_argument("unknown state");
  auto [it, inserted] = _actions.try_emplace(state, lambda);
  if (!inserted)
    throw std::invalid_argument("action exists");
}

template <typename TState>
void StateMachine<TState>::transitionTo(TState const &state) {
  if (!_cur)
    throw std::invalid_argument("uninitialized state");
  auto it = _transitions.find({*_cur, state});
  if (it == _transitions.end())
    throw std::invalid_argument("invalid transition");
  it->second();
  *_cur = state;
}

template <typename TState>
void StateMachine<TState>::update() {
  if (!_cur)
    throw std::invalid_argument("uninitialized state");
  auto it = _actions.find(*_cur);
  if (it != _actions.end())
    it->second();
}
