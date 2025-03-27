import math
import random
from statistics import Statistics

from agent import ETCAgent

REPS = 5


def experimental_best_m(T: int, K: int) -> tuple[int, float]:
    best = 0
    smallest_regret = float("inf")

    for m in range(1, T // K + 1):
        total_regret = 0.0

        for _ in range(REPS):
            agent = ETCAgent(K, T)
            best_arm = agent.explore(m)
            _, regret = agent.commit(best_arm, m)
            total_regret += regret

        avg_regret = total_regret / REPS
        if avg_regret < smallest_regret:
            best = m
            smallest_regret = avg_regret

    return best, smallest_regret


def get_theoretical_m(agent: ETCAgent, K: int, T: int) -> int:
    arms = agent.get_arms()
    mu_star = max(arm.get_mu() for arm in arms)
    delta = min((mu_star - arm.get_mu()) for arm in arms if arm.get_mu() < mu_star)

    # clamp to [1, T/K]
    return int(max(
        1.0, min(math.ceil((1.0 / (delta**2)) * math.log(K + T * delta**2)), T / K)
    ))


def main():
    # Given problem params
    MIN_K, MAX_K, K_STEP = 10, 30, 5
    MIN_T, MAX_T, T_STEP = 1000, 5000, 1000

    print(f"\n**NOTE**: Using {REPS} repetitions per T, K configuration\n")
    print(f"\n**NOTE**: Using a random mean, stdev in [1,100], [0.1, 1] for each arm\n")
    for K in range(MIN_K, MAX_K + 1, K_STEP):
        for T in range(MIN_T, MAX_T + 1, T_STEP):
            print(f"{K=}, {T=}")

            regrets = []

            for i in range(REPS):
                agent = ETCAgent(K, T)
                m = get_theoretical_m(agent, K, T)
                best_arm = agent.explore(m)
                _, regret = agent.commit(best_arm, m)
                print(f"-> trial {i}: theoretical {m=}; {best_arm=}; {regret=}")
                regrets.append(regret)

            stdev = Statistics.get_stdev(regrets)
            mean = Statistics.get_mean(regrets)
            print(f"Configuration resulted in aggregate {mean=}, {stdev=}\n")

    T, K = 5000, 30
    m, smallest_regret = experimental_best_m(T, K)
    print('\n----------------------\n')
    print(f"With {T=}, {K=}, experimentall determined best {m=} with minimized regret {smallest_regret}\n")


if __name__ == "__main__":
    main()
