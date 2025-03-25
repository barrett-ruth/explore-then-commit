#pragma once

#include <cstddef>
#include <vector>

#include "arm.hh"

struct ETCAgent {
public:
  ETCAgent(size_t arm_count, size_t round_count) : arms(arm_count), rounds(round_count) {
    for (size_t i = 0; i < arm_count; ++i) {
      arms.emplace_back(Arm());
    }
  }

  size_t explore(size_t depth) {
    for (Arm &arm : arms) {
      for (size_t _ = 0; _ < depth; ++_) {
        arm.pull();
      }
    }

    size_t best_index = 0;
    long double best_avg = arms[0].calculate_average_reward();
    for (size_t i = 1; i < arms.size(); ++i) {
      long double curr_avg = arms[i].calculate_average_reward();
      if (curr_avg > best_avg) {
        best_avg = curr_avg;
        best_index = i;
      }
    }
    return best_index;
  }

  long double commit(size_t arm_index, size_t depth) {
    size_t remaining_rounds = rounds - (arms.size() * depth);
    for (size_t i = 0; i < remaining_rounds; ++i) {
      arms[arm_index].pull();
    }
    long double total_reward = 0.0L;
    for (const Arm &arm : arms) {
      total_reward += arm.get_total_reward();
    }
    return total_reward;
  }

private:
  size_t rounds;
  size_t rounds_played;
  std::vector<Arm> arms;
};
