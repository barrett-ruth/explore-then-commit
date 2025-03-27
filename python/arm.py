import random

from distribution import Distribution


class Arm:
    def __init__(self):
        self.total_reward = 0.0
        self.total_pulls = 0
        self.sigma = random.uniform(0.1, 1.0)
        self.mu = random.uniform(1.0, 100.0)

    def pull(self):
        dist = random.gauss(self.mu, self.sigma)
        self.total_reward += dist
        self.total_pulls += 1

    def calculate_average_reward(self) -> float:
        return self.total_reward / self.total_pulls if self.total_pulls > 0 else 0.0

    def get_total_reward(self) -> float:
        return self.total_reward

    def get_mu(self) -> float:
        return self.mu
