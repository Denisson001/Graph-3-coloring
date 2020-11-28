#!/usr/bin/python3

import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data_file_name = sys.argv[1]
df = pd.read_csv(data_file_name, header=None)
dataset = df.values
max_value = 50

sns.set(palette="Set2")
plt.figure(figsize=(9, 6))
with sns.axes_style("darkgrid"):
    plt.hist(dataset[dataset <= max_value], bins=max_value, density=True)

plt.tick_params(axis='both', labelsize=8)
plt.locator_params("y", nbins=20)
plt.locator_params("x", nbins=40)
plt.xlim(1, max_value)
plt.xlabel("Количество итераций алгоритма", fontsize=9)
plt.ylabel("Доля запусков", fontsize=9)
plt.grid(ls=":")

plt.savefig("randomized_csp_solver_bench_hist.png")
