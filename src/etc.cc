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
  // for last example solving for optimal m T = 5000 K = 30
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
    if (avg_regret < smallest_regret) {
      best = m;
      smallest_regret = avg_regret;
    }
  }

  return std::pair<size_t, long double>(best, smallest_regret);
}

long double get_theoretical_m(ETCAgent& agent, size_t K, size_t T) {
  std::vector<Arm> arms = agent.get_arms();
  long double delta = std::numeric_limits<long double>::max();
  long double mu_star = std::numeric_limits<long double>::min();
  for (auto &arm : arms) {
    mu_star = std::max(mu_star, arm.get_mu());
  }
  for (auto &arm : arms) {
    // https://proceedings.neurips.cc/paper_files/paper/2017/file/b299ad862b6f12cb57679f0538eca514-Paper.pdf
    if (arm.get_mu() < mu_star) {
      delta += std::min(delta, mu_star - arm.get_mu());
    }
  }
  // http://pierre.gaillard.me/doc/mva/mva_sequential_learning_lecture3.pdf
  // I derived this equation using theorem 6.1 in these lecture notes which were given to me during OH.
  // I set the derivate equal to 0 and solved for m using the simplification that T >> mK. A picture of my work is attached in the folder.
  return std::clamp(std::ceill(1.0L / (delta * delta) * std::log(static_cast<long double>(K + T * delta * delta))), 1.0L, static_cast<long double>(T / K));
}

int main() {
  constexpr size_t MIN_K = 10, MAX_K = 30;
  constexpr size_t K_STEP = 5;
  constexpr size_t MIN_T = 1000, MAX_T = 5000;
  constexpr size_t T_STEP = 1000;

  for (size_t K = MIN_K; K <= MAX_K; K += K_STEP) {
    for (size_t T = MIN_T; T <= MAX_T; T += T_STEP) {
      if (K == 30 && T == 5000) break;
      std::cout << "\nUsing a random mean from [1,100] and stdev [0.1, 1] for each arm";
        std::cout << "\n Begin simulation \narms (K): " << K
                  << ", rounds (T): " << T
                  << ", Experiment Repetitions: " << EXPERIMENT_REPITITIONS << "\n";

        std::vector<long double> regrets;
        for (size_t i = 0; i < EXPERIMENT_REPITITIONS; ++i) {
          ETCAgent agent(K, T);
          long double m = get_theoretical_m(agent, K, T);
          std::cout << "  Experiment " << (i + 1)
                    << ": Best depth m = " << m << "\n";
          size_t best_arm = agent.explore(m);
          auto [reward, regret] = agent.commit(best_arm, m);
          std::cout << "    Best arm index selected: " << best_arm
                    << ", Regret observed: " << regret << "\n";
          regrets.emplace_back(regret);
        }

        auto stdev = Statistics::get_stdev(regrets);
        auto mean = Statistics::get_mean(regrets);
        std::cout << "Got overall regret stdev: " << stdev << "; regret mean: " << mean
                  << '\n';
    }
  }

  size_t T = 5000, K = 30;
  auto [m, smallest_regret] = best_m(T, K);
  std::cout << "\n\nFinal Simulation: arms (K): " << K
                  << ", rounds (T): " << T << "\n";
  std::cout << "  Best m found: " << m << "\n";
  std::cout << "  Corresponding average regret: " << smallest_regret << "\n";

  return 0;
}
