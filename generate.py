import numpy as np
import argparse

def main():
    parser = argparse.ArgumentParser(description='Генерация случайных матриц')
    parser.add_argument('N', type=int, help='Размер матрицы')
    parser.add_argument('--seed', type=int, default=42, help='Seed для генерации')
    
    args = parser.parse_args()
    
    np.random.seed(args.seed)
    
    A = np.random.rand(args.N, args.N) * 20 - 10
    B = np.random.rand(args.N, args.N) * 20 - 10
    
    with open('input_A.txt', 'w') as f:
        f.write(f"{args.N}\n")
        np.savetxt(f, A, fmt='%.6f')
    
    with open('input_B.txt', 'w') as f:
        f.write(f"{args.N}\n")
        np.savetxt(f, B, fmt='%.6f')
    
    print(f"Созданы матрицы {args.N}x{args.N}")
    print("Файлы: input_A.txt, input_B.txt")

if __name__ == "__main__":
    main()