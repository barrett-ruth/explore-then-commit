import numpy as np

from arm import Arm


class ETCAgent:
    def __init__(self, arm_count: int, num_rounds: int):
        self.arms = [Arm() for _ in range(arm_count)]
        self.rounds = num_rounds

    def explore(self, depth: int) -> int:
        for arm in self.arms:
            for _ in range(depth):
                arm.pull()

        j = 0
        best = self.arms[0].calculate_average_reward()

        for i in range(1, len(self.arms)):
            avg = self.arms[i].calculate_average_reward()
            if avg > best:
                best = avg
                j = i

        return j

    def commit(self, arm_index: int, depth: int) -> tuple[float, float]:
        # NOTE: returns *ACTUAL* regret
        left = self.rounds - (len(self.arms) * depth)
        net_mus = 0.0

        for _ in range(left):
            self.arms[arm_index].pull()
            net_mus += self.arms[arm_index].get_mu()

        total_reward = 0.0

        for arm in self.arms:
            total_reward += arm.get_total_reward()
            net_mus += depth * arm.get_mu()

        mu_star = max(arm.get_mu() for arm in self.arms)
        regret = self.rounds * mu_star - net_mus

        return total_reward, regret

    def get_arms(self) -> list[Arm]:
        return self.arms
