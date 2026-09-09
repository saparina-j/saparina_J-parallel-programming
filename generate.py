import random

def generate_matrix(n, filename):
    with open(filename, 'w') as f:
        f.write(f"{n}\n")
        for _ in range(n):
            row = [f"{random.uniform(0, 10):.15f}" for _ in range(n)]
            f.write(" ".join(row) + "\n")

n = 500
print(f"Генерация матриц {n}x{n}...")
generate_matrix(n, "matrix_a.txt")
generate_matrix(n, "matrix_b.txt")
print("Готово!")