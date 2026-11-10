import numpy as np
import sys

if __name__ == "__main__":
    n = int(sys.argv[1])
    out = sys.argv[2]
    # Фиксированный seed для воспроизводимости
    rng = np.random.default_rng(42)
    A = rng.random((n, n))
    np.savetxt(out, A, fmt="%.12e")