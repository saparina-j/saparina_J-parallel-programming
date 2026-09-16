import numpy as np

def load_matrix(filename):
    with open(filename, 'r') as f:
        n = int(f.readline())
        matrix = []
        for _ in range(n):
            row = list(map(float, f.readline().split()))
            matrix.append(row)
    return np.array(matrix)

print("\n========== ВЕРИФИКАЦИЯ ==========")

try:
    A = load_matrix("matrix_a.txt")
    B = load_matrix("matrix_b.txt")
    C_cpp = load_matrix("result_cpp.txt")
    
    C_numpy = np.matmul(A, B)
    
    max_error = np.max(np.abs(C_numpy - C_cpp))
    print(f"Максимальная ошибка: {max_error:.10f}")
    
    if max_error < 1e-6:
        print("ВЕРИФИКАЦИЯ ПРОЙДЕНА!")
    else:
        print("ВЕРИФИКАЦИЯ НЕ ПРОЙДЕНА!")
        
except Exception as e:
    print(f"Ошибка: {e}")