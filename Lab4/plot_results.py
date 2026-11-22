import matplotlib.pyplot as plt
import numpy as np

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5))

sizes = ['256x256', '512x512', '1024x1024', '2048x2048']
x = np.arange(len(sizes))
width = 0.25

gpu_8 = [0.241952, 0.993376, 7.83965, np.nan]
gpu_16 = [0.093056, 0.638976, 7.19462, 56.8168]
gpu_32 = [0.077824, 0.626016, 5.35722, 33.2347]

ax1.bar(x - width, gpu_8, width, label='Block 8x8 (64 threads)', color='#4C72B0')
ax1.bar(x, gpu_16, width, label='Block 16x16 (256 threads)', color='#55A868')
ax1.bar(x + width, gpu_32, width, label='Block 32x32 (1024 threads)', color='#C44E52')

ax1.set_yscale('log')
ax1.set_ylabel('Время выполнения на GPU (мс, лог. шкала)')
ax1.set_title('Время работы GPU при разных конфигурациях блоков')
ax1.set_xticks(x)
ax1.set_xticklabels(sizes)
ax1.legend()
ax1.grid(True, which='both', linestyle='--', alpha=0.5)

sizes_sp = ['256x256', '512x512', '1024x1024']
sp_8 = [85.70, 225.72, 440.62]
sp_16 = [228.27, 345.43, 463.76]
sp_32 = [268.58, 375.33, 623.54]

ax2.plot(sizes_sp, sp_8, marker='o', linewidth=2, label='Block 8x8', color='#4C72B0')
ax2.plot(sizes_sp, sp_16, marker='s', linewidth=2, label='Block 16x16', color='#55A868')
ax2.plot(sizes_sp, sp_32, marker='^', linewidth=2, label='Block 32x32', color='#C44E52')

for i, txt in enumerate(sp_32):
    ax2.annotate(f'{txt:.1f}x', (sizes_sp[i], sp_32[i]), textcoords="offset points", xytext=(0, 8), ha='center', fontweight='bold', color='#C44E52')

ax2.set_ylabel('Ускорение (CPU Time / GPU Time)')
ax2.set_title('Рост ускорения CUDA с увеличением размера матрицы')
ax2.set_ylim(0, 720)
ax2.legend()
ax2.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig('cuda_benchmark.png', dpi=150)
print("График сохранен в cuda_benchmark.png")
