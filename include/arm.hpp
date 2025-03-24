#pragma once

#include <cstdint>

struct Arm {
    uint64_t total_reward = 0, total_pulls = 0;
    Arm() = default;

    uint64_t pull_arm();

    uint64_t calculate_average_reward();
};
