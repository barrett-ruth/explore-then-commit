#pragma once

#include <cstdint>
#include <cstddef>

struct explore_then_commit_agent {
    uint64_t rounds, rounds_played;
    size_t arms, best_arm;

    explore_then_commit_agent(size_t arms_, uint64_t rounds_) : arms(arms_), rounds(rounds_) {}

    uint64_t get_reward(size_t arm_index, uint64_t round);

    uint64_t get_mean_reward(size_t arm_index, uint64_t round, uint64_t selected_count, uint64_t num_of_actions);

};
