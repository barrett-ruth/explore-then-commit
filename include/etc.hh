#pragma once

#include <cstddef>
#include <vector>

#include "arm.hh"

struct ETCAgent {
public:
  ETCAgent(size_t arm_count, size_t round_count)
      : arms(arm_count), rounds(round_count) {}

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
    long double net_mus = 0;
    // exploit
    for (int i = 0; i < remaining_rounds; ++i) {
      arms[arm_index].pull();
      net_mus += arms[arm_index].get_mu();
    }
    long double total_reward = 0.0L;
    // explore: sum the expected rewards from exploration
    for (const Arm &arm : arms) {
      total_reward += arm.get_total_reward();
      net_mus += depth *  arm.get_mu();
    }

    long double mu_star = std::numeric_limits<long double>::min();
    for (auto &arm : arms) {
      mu_star = std::max(mu_star, arm.get_mu());
    }
    // regret as the difference between the reward from pulling best arm vs mean of other arms
    long double regret = static_cast<long double>(rounds) * mu_star - net_mus;
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
