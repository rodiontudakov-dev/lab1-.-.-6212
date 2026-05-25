import numpy as np
import sys

def read_matrix(path):
    data = np.loadtxt(path, skiprows=1)
    return data

def main():
    if len(sys.argv) != 4:
        print("Использование: python check.py A.txt B.txt result.txt")
        sys.exit(1)
    
    A = read_matrix(sys.argv[1])
    B = read_matrix(sys.argv[2])
    C = read_matrix(sys.argv[3])
    
    expected = A @ B
    
    diff = np.abs(C - expected)
    max_err = np.max(diff)
    mean_err = np.mean(diff)
    
    print("\n=== ПРОВЕРКА РЕЗУЛЬТАТА ===")
    print(f"Максимальная ошибка: {max_err:.2e}")
    print(f"Средняя ошибка: {mean_err:.2e}")
    
    if max_err < 1e-6:
        print("\n✅ Результат ВЕРНЫЙ")
        sys.exit(0)
    else:
        print("\n❌ Результат НЕВЕРНЫЙ")
        sys.exit(1)

if __name__ == "__main__":
    main()