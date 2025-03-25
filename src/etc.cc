#include "etc.hh"

#include <cstddef>
#include <iostream>
#include <limits>
#include <utility>

#include "arm.hh"
#include "etc.hh"
#include "statistics.hh"

constexpr size_t EXPERIMENT_REPITIONS = 5;

std::pair<size_t, long double> best_m(size_t T, size_t K) {
  size_t best;
  long double best_avg_reward = std::numeric_limits<long double>::min();
  for (size_t m = 1; m <= T / K; ++m) {
    long double total_reward = 0;

    for (size_t i = 0; i < EXPERIMENT_REPITIONS; ++i) {
      ETCAgent agent(K, T);
      size_t best_arm = agent.explore(m);
      long double reward = agent.commit(best_arm);

      total_reward += reward;
    }

    long double avg_reward = total_reward / EXPERIMENT_REPETITION;
    if (avg_reward > best_avg_reward) {
      best = m;
      best_avg_reward = avg_reward;
    }
  }

  return {m, best_reward};
}

int main() {
  constexpr size_t MIN_K = 10, MAX_K = 30;
  constexpr size_t K_STEP = 5;
  constexpr size_t MIN_T = 1000, MAX_T = 5000;
  constexpr size_t T_STEP = 1000;

  for (size_t K = MIN_K; K <= MAX_K; K += K_STEP) {
    for (size_t T = MIN_T; T <= MAX_T; T += T_STEP) {
      for (size_t m = 1; m <= T / K; ++m) {
        std::cout << "Running K=" << std::to_string(K)
                  << "; T=" << std::to_string(T) << "; M=" << m
                  << std::to_string(EXPERIMENT_REPITIONS) << " times\n";

        std::vector<long double> rewards;
        for (size_t i = 0; i < EXPERIMENT_REPITIONS; ++i) {
          ETCAgent agent(K, T);
          size_t best_arm = agent.explore(m);
          long double overall_reward = agent.commit(best_arm);
          rewards.emplace_back(overall_reward);
        }

        auto stdev = Statistics::get_stdev(rewards);
        auto mean = Statistics::get_mean(rewards);
        std::cout << "Got overall reward stdev: " << stdev << "; mean: " << mean
                  << '\n';
      }
    }
  }

  size_t T = 5000, K = 30;
  auto [m, best_reward] = best_m(T, K);
  std::cout << "With T=" << T << "; K=" << K << ", got best m=" << m << '\n';
  std::cout << "It has a highest average reward of " << best_reward << '\n';

  return 0;
}
