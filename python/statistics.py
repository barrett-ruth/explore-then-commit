import math


class Statistics:
    @staticmethod
    def get_mean(values: list[float]) -> float:
        return sum(values) / len(values) if values else 0.0

    @staticmethod
    def get_stdev(values: list[float]) -> float:
        if len(values) < 2:
            return 0.0
        mean = Statistics.get_mean(values)
        variance = sum((value - mean) ** 2 for value in values) / len(values)
        return math.sqrt(variance)
