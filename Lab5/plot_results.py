import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
df_2_8 = df[df["threads"] <= 8]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5))

ax1.plot(df_2_8["threads"], df_2_8["view1_block"], marker="o", linewidth=2, label="Блочное разделение (Block)", color="#C44E52")
ax1.plot(df_2_8["threads"], df_2_8["view1_interleaved"], marker="s", linewidth=2, label="Чередующееся (Interleaved)", color="#55A868")
ax1.axhline(2.0, linestyle="--", color="gray", alpha=0.7, label="Аппаратный предел (2 vCPU)")
ax1.set_title("View 1: Ускорение от числа потоков", fontsize=11, fontweight="bold")
ax1.set_xlabel("Количество потоков")
ax1.set_ylabel("Ускорение (Speedup)")
ax1.set_ylim(0.8, 2.2)
ax1.grid(True, linestyle="--", alpha=0.5)
ax1.legend()

ax2.plot(df_2_8["threads"], df_2_8["view2_block"], marker="o", linewidth=2, label="Блочное разделение (Block)", color="#C44E52")
ax2.plot(df_2_8["threads"], df_2_8["view2_interleaved"], marker="s", linewidth=2, label="Чередующееся (Interleaved)", color="#55A868")
ax2.axhline(2.0, linestyle="--", color="gray", alpha=0.7, label="Аппаратный предел (2 vCPU)")
ax2.set_title("View 2: Ускорение от числа потоков", fontsize=11, fontweight="bold")
ax2.set_xlabel("Количество потоков")
ax2.set_ylabel("Ускорение (Speedup)")
ax2.set_ylim(0.8, 2.2)
ax2.grid(True, linestyle="--", alpha=0.5)
ax2.legend()

plt.tight_layout()
plt.savefig("mandelbrot_benchmark.png", dpi=150)
