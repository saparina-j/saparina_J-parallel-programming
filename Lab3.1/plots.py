import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

sizes = [200, 400, 800, 1200, 1600, 2000]
T = {
    1: [0.010741, 0.125076, 4.359664, 3.519956, 12.864898, 26.310627],
    2: [0.005741, 0.063242, 2.153755, 1.725390, 6.396254, 13.129066],
    4: [0.002640, 0.033463, 1.080291, 1.364799, 3.494025, 6.915896],
    8: [0.001317, 0.015887, 0.591117, 0.803315, 2.892163, 4.527556],
}
procs = [1, 2, 4, 8]

# График 1: время от N
plt.figure()
for p in procs:
    plt.plot(sizes, T[p], marker='o', label=f'{p} procs')
plt.xlabel('N (размер матрицы)')
plt.ylabel('Время T, сек')
plt.yscale('log')
plt.title('Время умножения матриц от размера (MPI, Intel MPI 4.1.3)')
plt.legend()
plt.grid(True, which='both', alpha=0.3)
plt.savefig('time_vs_N.png', dpi=150, bbox_inches='tight')
plt.close()

# График 2: ускорение от числа процессов
plt.figure()
for i, N in enumerate(sizes):
    t1 = T[1][i]
    S = [t1 / T[p][i] for p in procs]
    plt.plot(procs, S, marker='o', label=f'N={N}')
plt.plot(procs, procs, 'k--', alpha=0.5, label='линейное')
plt.xlabel('Число процессов p')
plt.ylabel('Ускорение S = T(1)/T(p)')
plt.title('Ускорение параллельной версии')
plt.legend()
plt.grid(True)
plt.savefig('speedup.png', dpi=150, bbox_inches='tight')
plt.close()

# График 3: эффективность
plt.figure()
for i, N in enumerate(sizes):
    t1 = T[1][i]
    E = [t1 / (T[p][i] * p) for p in procs]
    plt.plot(procs, E, marker='o', label=f'N={N}')
plt.xlabel('Число процессов p')
plt.ylabel('Эффективность E = S/p')
plt.title('Эффективность параллелизации')
plt.legend()
plt.grid(True)
plt.savefig('efficiency.png', dpi=150, bbox_inches='tight')
plt.close()

print('Готово: time_vs_N.png, speedup.png, efficiency.png')