#include <ssm/state_actions.hpp>

#include "another_unit.hpp"

#include <iostream>

struct State {
  void on_enter() {}
  void on_exit() {}
};

void on_enter(State &state) noexcept {}

struct Machine {};

int main() {

  State state;
  Machine machine;

  auto addr = reinterpret_cast<std::intptr_t>(&ssm::state_actions::enter);

  ssm::state_actions::enter(state, machine);
  ssm::state_actions::exit(state, machine);

  std::cout << "enter nothrow: "
            << ssm::state_actions::is_nothrow_enterable_v<State> << std::endl;
  std::cout << "exit nothrow: "
            << ssm::state_actions::is_nothrow_exitable_v<State> << std::endl;
}
