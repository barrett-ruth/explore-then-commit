#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "arm.hh"

struct ETCAgent {
public:
  ETCAgent(size_t arm_count) : arms(arm_count) {}

  size_t explore(size_t depth);

  long double commit(size_t arm_index);

private:
  size_t rounds;
  size_t rounds_played;
  std::vector<Arm> arms;
};
