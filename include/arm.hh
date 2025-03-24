#pragma once

#include <cstdint>

struct Arm {
 public:
  Arm() : total_reward{}, total_pulls{} {};

  long double get_reward() const;

  inline long double calculate_average_reward() { return static_cast<long double>(total_reward) / total_pulls; }

 private:
  long double total_reward;
  std::size_t total_pulls;
};
