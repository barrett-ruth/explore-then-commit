#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

#include "arm.hpp"

struct explore_then_commit_agent {
    uint64_t rounds, rounds_played;
    std::vector<Arm> arms;
    size_t best_arm;

    explore_then_commit_agent(size_t arm_count, uint64_t rounds_) : rounds(rounds_) {
        arms.reserve(arm_count);
    }

    uint64_t explore();

    uint64_t commit();
};
