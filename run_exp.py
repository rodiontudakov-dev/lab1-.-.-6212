import subprocess
import sys
import time
import os
import csv

SIZES = [200, 400, 800, 1200, 1600]
THREADS = [1, 2, 4, 8]
RUNS_PER_CONFIG = 3

GEN_SCRIPT = "generate.py"
PROG_PATH = "matmul.exe"
RESULT_FILE = "output.txt"
LOG_FILE = "results.csv"

def run_single_test(size, threads):
    subprocess.run([sys.executable, GEN_SCRIPT, str(size)], capture_output=True)
    
    times = []
    for _ in range(RUNS_PER_CONFIG):
        start = time.time()
        proc = subprocess.run([PROG_PATH, "input_A.txt", "input_B.txt", RESULT_FILE, str(threads)],
                              capture_output=True, text=True, timeout=300)
        elapsed = time.time() - start
        
        if proc.returncode == 0:
            times.append(elapsed)
        time.sleep(0.5)
    
    if times:
        return sum(times) / len(times)
    return None

def main():
    print("Запуск экспериментов...")
    print(f"Размеры: {SIZES}")
    print(f"Потоки: {THREADS}")
    print(f"Запусков на конфигурацию: {RUNS_PER_CONFIG}")
    
    with open(LOG_FILE, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["size", "threads", "avg_time_sec"])
        
        total = len(SIZES) * len(THREADS)
        current = 0
        
        for size in SIZES:
            for threads in THREADS:
                current += 1
                print(f"[{current}/{total}] N={size}, потоки={threads}...")
                
                avg_time = run_single_test(size, threads)
                if avg_time:
                    writer.writerow([size, threads, f"{avg_time:.4f}"])
                    print(f"  -> {avg_time:.2f} сек")
                else:
                    writer.writerow([size, threads, "ERROR"])
                    print(f"  -> ОШИБКА")
    
    print(f"\nГотово! Результаты в {LOG_FILE}")

if __name__ == "__main__":
    main()