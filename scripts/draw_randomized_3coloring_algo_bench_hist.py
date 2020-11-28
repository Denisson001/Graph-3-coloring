#!/usr/bin/python3

import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns


def read_distribution_from_file(file_name):
    with open(file_name, "r") as f:
        return np.array(list(map(int, f.read().split())))


distribution_n5 = read_distribution_from_file(sys.argv[1])
distribution_n6 = read_distribution_from_file(sys.argv[2])
distribution_n7 = read_distribution_from_file(sys.argv[3])

sns.set(palette='Set2')
plt.figure(figsize=(9, 6))

max_iter_count = 8

plt.hist([distribution_n5[distribution_n5 < max_iter_count],
          distribution_n6[distribution_n6 < max_iter_count],
          distribution_n7[distribution_n7 < max_iter_count]],
          bins=np.arange(1, max_iter_count + 1) - 0.5,
          density=True, width=0.2, rwidth=0.6,
          label=[r"$n=5$", r"$n=6$", r"$n=7$"])

plt.locator_params("x", nbins=10)
plt.xlim(0.3, max_iter_count - 0.3)
plt.xlabel("Количество итераций алгоритма", fontsize=9)
plt.ylabel("Доля запусков", fontsize=9)
plt.grid(ls=":")
plt.legend(fontsize=10)

plt.savefig("randomized_3coloring_algo_bench_hist.png")
