#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "arm.hh"

struct ETCAgent {
 public:
  explore_then_commit_agent(size_t arm_count, size_t rounds_) : rounds(rounds_) { arms.reserve(arm_count); }

  uint64_t explore();

  uint64_t commit();

 private:
  size_t rounds;
  size_t rounds_played;
  std::vector<Arm> arms;
  size_t best_arm;
};
