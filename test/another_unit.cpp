#include <ssm/enter.hpp>

#include "another_unit.hpp"

#include <iostream>

namespace test {

struct S {
  void on_enter() {}
};

struct M {};

int function() {
  S s;
  M m;
  std::cout << "Address in another unit:" << std::endl;
  std::cout << reinterpret_cast<std::intptr_t>(&ssm::enter) << std::endl;
  ssm::enter(s, m);
  return 1;
}

} // namespace test
