import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

plt.style.use('seaborn-v0_8-darkgrid')

def main():
    if not os.path.exists('results.csv'):
        print("Файл results.csv не найден!")
        return
    
    df = pd.read_csv('results.csv')
    
    os.makedirs('graphs', exist_ok=True)
    
    plt.figure(figsize=(12, 8))
    
    for threads in sorted(df['threads'].unique()):
        data = df[df['threads'] == threads].sort_values('size')
        plt.plot(data['size'], data['avg_time_sec'], 
                marker='o', linewidth=2, markersize=8,
                label=f'{threads} потоков')
    
    plt.xlabel('Размер матрицы (N)', fontsize=12)
    plt.ylabel('Время (секунды)', fontsize=12)
    plt.title('Производительность умножения матриц', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.savefig('graphs/time_chart.png', dpi=150)
    plt.close()
    
    print("Графики сохранены в папку graphs/")

if __name__ == "__main__":
    main()