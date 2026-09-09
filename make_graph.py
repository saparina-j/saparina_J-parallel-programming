import matplotlib.pyplot as plt


sizes = [50, 100, 200, 300, 400, 500]
gflops = [0.00, 2.00, 4.00, 0.75, 3.05, 2.32]


plt.figure(figsize=(10, 6))
plt.plot(sizes, gflops, 'bo-', linewidth=2, markersize=8)


plt.xlabel('Matrix Size (n x n)', fontsize=12)
plt.ylabel('Performance (GFLOPS)', fontsize=12)
plt.title('Matrix Multiplication Performance', fontsize=14)
plt.grid(True, alpha=0.3)

for i, (x, y) in enumerate(zip(sizes, gflops)):
    plt.annotate(f'{y:.2f}', (x, y), textcoords="offset points", 
                 xytext=(0, 10), ha='center')


plt.savefig('performance_graph.png', dpi=300, bbox_inches='tight')
print("График сохранён как performance_graph.png")