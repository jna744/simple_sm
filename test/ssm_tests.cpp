#include "another_unit.hpp"
#include <simple/sm/sm_we.hpp>
#include <simple/sm/state_actions.hpp>
#include <simple/sm/state_traits.hpp>

#include <iostream>

struct State;
struct State1;

using sm_t = simple::sm::sm_woe<simple::sm::state_list<State, State1>>;

struct State {
  constexpr void on_enter() noexcept {}
  constexpr void on_exit() {}
};

struct State1 {
};

constexpr auto get_value() noexcept
{
  sm_t sm;
  sm.set_state<State1>();
  sm.set_state<State>();
  return sm;
}

int main()
{
  constexpr auto sm = get_value();
}
