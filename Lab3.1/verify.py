import numpy as np
import sys

def load_matrix(filename, n):
    data = np.loadtxt(filename)
    if data.size != n * n:
        raise ValueError(f"Size mismatch: expected {n*n}, got {data.size}")
    return data.reshape((n, n))

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python verify.py N fileA fileB fileC")
        sys.exit(1)
    n = int(sys.argv[1])
    A = load_matrix(sys.argv[2], n)
    B = load_matrix(sys.argv[3], n)
    C = load_matrix(sys.argv[4], n)
    C_ref = A @ B
    if np.allclose(C, C_ref, rtol=1e-9, atol=1e-9):
        print("VERIFICATION: OK")
    else:
        diff = np.abs(C - C_ref).max()
        print(f"VERIFICATION: FAIL (max abs diff = {diff})")
        sys.exit(1)