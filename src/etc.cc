#include "etc.hh"

#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <cmath>
#include <string>
#include <type_traits>

#include "arm.hh"
#include "etc.hh"
#include "statistics.hh"

constexpr size_t EXPERIMENT_REPITITIONS = 5;

std::pair<size_t, long double> best_m(size_t T, size_t K) {
  size_t best;
  long double smallest_regret = std::numeric_limits<long double>::max();
  for (size_t m = 1; m <= T / K; ++m) {
    long double total_regret = 0;

    for (int i = 0; i < EXPERIMENT_REPITITIONS; ++i) {
      ETCAgent agent(K, T);
      size_t best_arm = agent.explore(m);
      auto [reward, regret] = agent.commit(best_arm, m);

      total_regret += regret;
    }

    long double avg_regret = total_regret / EXPERIMENT_REPITITIONS;
    if (avg_regret > smallest_regret) {
      best = m;
      smallest_regret = avg_regret;
    }
  }

  return std::pair<size_t, long double>(best, smallest_regret);
}

long double get_theoretical_m(ETCAgent& agent) {
  std::vector<Arm> arms = agent.get_arms();
  long double delta = std::numeric_limits<long double>::max();
  long double mu_star = std::numeric_limits<long double>::min();
  for (auto &arm : arms) {
    mu_star = std::max(mu_star, arm.get_mu());
  }
  for (auto &arm : arms) {
    // https://proceedings.neurips.cc/paper_files/paper/2017/file/b299ad862b6f12cb57679f0538eca514-Paper.pdf
    if (arm.get_mu() < mu_star) {
      delta = std::min(delta, mu_star - arm.get_mu());
    }
  }
  return std::max(std::ceill(4.0L / (delta * delta) * std::log(static_cast<long double>(arms.size()) * delta * delta / 4.0L)), 0.0L);
}

int main() {
  constexpr size_t MIN_K = 10, MAX_K = 30;
  constexpr size_t K_STEP = 5;
  constexpr size_t MIN_T = 1000, MAX_T = 5000;
  constexpr size_t T_STEP = 1000;

  for (size_t K = MIN_K; K <= MAX_K; K += K_STEP) {
    for (size_t T = MIN_T; T <= MAX_T; T += T_STEP) {
        std::cout << "Running K=" << std::to_string(K)
                  << "; T=" << std::to_string(T)
                  << std::to_string(EXPERIMENT_REPITITIONS) << " times\n";

        std::vector<long double> regrets;
        for (size_t i = 0; i < EXPERIMENT_REPITITIONS; ++i) {
          ETCAgent agent(K, T);
          long double m = get_theoretical_m(agent);
          std::cout << "Best m: " << m << '\n';
          size_t best_arm = agent.explore(m);
          auto [_, regret] = agent.commit(best_arm, m);
          regrets.emplace_back(regret);
        }

        auto stdev = Statistics::get_stdev(regrets);
        auto mean = Statistics::get_mean(regrets);
        std::cout << "Got overall regret stdev: " << stdev << "; mean: " << mean
                  << '\n';
    }
  }

  size_t T = 5000, K = 30;
  auto [m, smallest_regret] = best_m(T, K);
  std::cout << "With T=" << T << "; K=" << K << ", got best m=" << m << '\n';
  std::cout << "It has a smallest average regret of" << smallest_regret << '\n';

  return 0;
}
