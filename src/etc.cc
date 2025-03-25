#include "etc.hh"

#include <cstddef>
#include <random>

#include "arm.hh"
std::vector<long double> generate_sigmas(size_t N) {
  std::vector<long double> vec;
  vec.reserve(N);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<long double> dist(0.1L, 1.0L);

  for (size_t i = 0; i < N; ++i) {
    vec.push_back(dist(gen));
  }

  return vec;
}

int main() {
  constexpr size_t K = 15;
  constexpr size_t K_step = 5;
  constexpr size_t T = 1000;
  constexpr size_t T_step = 1000;
  constexpr size_t EXPERIMENT_REPITIONS = 5;



  return 0;
}
