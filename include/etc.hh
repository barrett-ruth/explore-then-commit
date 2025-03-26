#pragma once

#include <cstddef>
#include <vector>

#include "arm.hh"

struct ETCAgent {
public:
  ETCAgent(size_t arm_count, size_t round_count)
      : arms(arm_count), rounds(round_count) {
    for (int i = 0; i < arm_count; ++i) {
      arms.emplace_back(Arm());
    }
  }

  size_t explore(size_t depth) {
    for (Arm &arm : arms) {
      for (int _ = 0; _ < depth; ++_) {
        arm.pull();
      }
    }

    size_t best_index = 0;
    long double best_avg = arms[0].calculate_average_reward();
    for (int i = 1; i < arms.size(); ++i) {
      long double curr_avg = arms[i].calculate_average_reward();
      if (curr_avg > best_avg) {
        best_avg = curr_avg;
        best_index = i;
      }
    }
    return best_index;
  }

  std::pair<long double, long double> commit(size_t arm_index, int depth) {
    int remaining_rounds = rounds - (arms.size() * depth);
    for (int i = 0; i < remaining_rounds; ++i) {
      arms[arm_index].pull();
    }
    long double total_reward = 0.0L;
    for (const Arm &arm : arms) {
      total_reward += arm.get_total_reward();
    }

    long double mu_star = std::numeric_limits<long double>::min();
    long double expected_reward = 0;
    for (auto &arm : arms) {
      mu_star = std::max(mu_star, arm.get_mu());
      expected_reward += arm.get_mu();
    }
    long double regret =
        static_cast<long double>(arms.size()) * mu_star - expected_reward;
    return {total_reward, regret};
  }

std::vector<Arm> get_arms() {
  return arms;
}

private:
  int rounds;
  size_t rounds_played;
  std::vector<Arm> arms;
};
