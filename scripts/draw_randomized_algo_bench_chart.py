#!/usr/bin/python3

import sys
import pandas as pd
import matplotlib.pyplot as plt

data_file_name = sys.argv[1]
benchmark_name = data_file_name.split('.')[0]
df = pd.read_csv(data_file_name, header=None)

plt.figure(figsize=(15, 10))
plt.plot(df.to_numpy().T.mean(axis=1))

# plt.xlim(20, 30)
plt.xlabel("Мощность множества вершин графа", fontsize=12)
plt.ylabel("Количество итераций алгоритма", fontsize=12)
plt.title(benchmark_name, fontsize=13)

plt.savefig(f"{benchmark_name}.png")
