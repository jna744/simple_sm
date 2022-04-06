#include <ssm/enter.hpp>
#include <ssm/exit.hpp>

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

  auto addr = reinterpret_cast<std::intptr_t>(&ssm::enter);

  ssm::enter(state, machine);
  ssm::exit(state, machine);

  std::cout << "enter nothrow: "
            << ssm::is_nothrow_enterable_v<State> << std::endl;
  std::cout << "exit nothrow: "
            << ssm::is_nothrow_exitable_v<State> << std::endl;
}
