#include "etc.hh"

#include <cstddef>
#include <iostream>

#include "arm.hh"
#include "etc.hh"
#include "statistics.hh"

int main() {
  constexpr size_t MIN_K = 10, MAX_K = 30;
  constexpr size_t K_STEP = 5;
  constexpr size_t MIN_T = 1000, MAX_T = 5000;
  constexpr size_t T_STEP = 1000;
  constexpr size_t EXPERIMENT_REPITIONS = 5;

  for (size_t K = MIN_K; K <= MAX_K; K += K_STEP) {
    for (size_t T = MIN_T; T <= MAX_T; T += T_STEP) {
      for (size_t m = 1; m <= T / K; ++m) {
        std::cout << "Running K=" << std::to_string(K)
                  << "; T=" << std::to_string(T) << "; M=" << m
                  << std::to_string(EXPERIMENT_REPITIONS) << " times\n";

        std::vector<long double> rewards;
        for (size_t i = 0; i < EXPERIMENT_REPITIONS; ++i) {
          ETCAgent agent(K, T);
          auto best_arm = agent.explore(m);
          auto overall_reward = agent.commit(best_arm);
          rewards.emplace_back(overall_reward);
        }

        auto stdev = get_stdev(rewards);
        auto mena = get_mean(rewards);
        std::cout << "Got overall reward stdev: " << stdev << "; mean: " << mean << '\n';
      }
    }
  }

  return 0;
}
