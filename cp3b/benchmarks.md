| Benchmark     | Time       | Operations      | GFLOPS | Input Pixels         | Output Pixels        |
|---------------|------------|-----------------|--------|----------------------|----------------------|
| benchmarks/1  | 0.009249 s | 1,004,000,000   | 108.6  | 1000 × 1000          | 1000 × 1000          |
| benchmarks/2a | 0.039701 s | 16,016,000,000  | 403.4  | 4000 × 1000          | 4000 × 4000          |
| benchmarks/2b | 0.041293 s | 16,016,000,000  | 387.9  | 4000 × 1000          | 4000 × 4000          |
| benchmarks/2c | 0.041813 s | 15,991,989,003  | 382.5  | 3999 × 999           | 3999 × 3999          |
| benchmarks/2d | 0.039328 s | 16,040,029,005  | 407.8  | 4001 × 1001          | 4001 × 4001          |
| benchmarks/3  | 0.360457 s | 216,144,000,000 | 599.6  | 6000 × 6000          | 6000 × 6000          |
| benchmarks/4  | 1.456728 s | 729,324,000,000 | 500.7  | 9000 × 9000          | 9000 × 9000          |

# Benchmark Results

## Command Performance Table

| Command       | Test cases | Total time |
|---------------|------------|------------|
| test-asan     | 227        | 18.3 s     |
| test-uninit   | 227        | 9.7 s      |
| test-plain    | 227        | 9.7 s      |
| test-plain    | 7          | 14.3 s     |
| benchmark-all | 7          | 11.2 s     |

## Additional Technical Details

### benchmarks/1 — CPU:

- Arithmetic operations: At least 1,004,000,000
- Running time: 0.0092 sec
- Arithmetic operations per second: At least 109 billion
- Wallclock time: 0.0092 sec
- CPU time: 0.088 sec
- Simultaneous hardware threads: 9.51 on average
- Time outside user space: 0.00205 sec (≈2.33%)
- Clock cycles: 0.137 billion
- Average clock frequency: 1.19 GHz
- Machine-language instructions: 0.193 billion
- Instructions per nanosecond (wallclock): ≈ 20.8
- Instructions per nanosecond (CPU time): ≈ 2.19
- Instructions per clock cycle: ≈ 1.41
- Instructions per arithmetic operation: 0.192
- Branches: 6.65%
- Branch mispredictions: 0.219%

### benchmarks/2a — CPU:

- Arithmetic operations: At least 16,016,000,000
- Running time: 0.0397 sec
- Arithmetic operations per second: At least 403 billion
- Wallclock time: 0.0397 sec
- CPU time: 0.64 sec
- Simultaneous hardware threads: 16.1 on average
- Clock cycles: 1.51 billion
- Average clock frequency: 2.18 GHz
- Machine-language instructions: 2.77 billion
- Instructions per nanosecond (wallclock): ≈ 69.7
- Instructions per nanosecond (CPU time): ≈ 4.32
- Instructions per clock cycle: ≈ 1.83
- Instructions per arithmetic operation: 0.173
- Branches: 5.70%
- Branch mispredictions: 0.109%

### benchmarks/2b — CPU:

- Arithmetic operations: At least 16,016,000,000
- Running time: 0.0413 sec
- Arithmetic operations per second: At least 388 billion
- Wallclock time: 0.0413 sec
- CPU time: 0.66 sec
- Simultaneous hardware threads: 16.0 on average
- Clock cycles: 1.54 billion
- Average clock frequency: 2.12 GHz
- Machine-language instructions: 2.77 billion
- Instructions per nanosecond (wallclock): ≈ 67.1
- Instructions per nanosecond (CPU time): ≈ 4.20
- Instructions per clock cycle: ≈ 1.80
- Instructions per arithmetic operation: 0.173
- Branches: 5.72%
- Branch mispredictions: 0.108%

### benchmarks/2c — CPU:

- Arithmetic operations: At least 15,991,989,003
- Running time: 0.0418 sec
- Arithmetic operations per second: At least 382 billion
- Wallclock time: 0.0418 sec
- CPU time: 0.70 sec
- Simultaneous hardware threads: 16.6 on average
- Clock cycles: 1.61 billion
- Average clock frequency: 2.24 GHz
- Machine-language instructions: 2.78 billion
- Instructions per nanosecond (wallclock): ≈ 66.6
- Instructions per nanosecond (CPU time): ≈ 4.00
- Instructions per clock cycle: ≈ 1.73
- Instructions per arithmetic operation: 0.174
- Branches: 5.84%
- Branch mispredictions: 0.107%

### benchmarks/2d — CPU:

- Arithmetic operations: At least 16,040,029,005
- Running time: 0.0393 sec
- Arithmetic operations per second: At least 408 billion
- Wallclock time: 0.0393 sec
- CPU time: 0.65 sec
- Simultaneous hardware threads: 16.6 on average
- Time outside user space: 0.064 sec (≈9.80%)
- Clock cycles: 1.65 billion
- Average clock frequency: 2.43 GHz
- Machine-language instructions: 2.80 billion
- Instructions per nanosecond (wallclock): ≈ 71.2
- Instructions per nanosecond (CPU time): ≈ 4.28
- Instructions per clock cycle: ≈ 1.70
- Instructions per arithmetic operation: 0.175
- Branches: 5.85%
- Branch mispredictions: 0.108%

### benchmarks/3 — CPU:

- Arithmetic operations: At least 216,144,000,000
- Running time: 0.360 sec
- Arithmetic operations per second: At least 600 billion
- Wallclock time: 0.360 sec
- CPU time: 6.00 sec
- Simultaneous hardware threads: 16.7 on average
- Time outside user space: 0.178 sec (≈2.97%)
- Clock cycles: 20.5 billion
- Average clock frequency: 3.42 GHz
- Machine-language instructions: 35.0 billion
- Instructions per nanosecond (wallclock): ≈ 97.2
- Instructions per nanosecond (CPU time): ≈ 5.84
- Instructions per clock cycle: ≈ 1.71
- Instructions per arithmetic operation: 0.162
- Branches: 5.29%
- Branch mispredictions: 0.052%

### benchmarks/4 — CPU:

- Arithmetic operations: At least 729,324,000,000
- Running time: 1.46 sec
- Arithmetic operations per second: At least 501 billion
- Wallclock time: 1.46 sec
- CPU time: 22.5 sec
- Simultaneous hardware threads: 15.5 on average
- Time outside user space: 0.289 sec (≈1.28%)
- Clock cycles: 83.7 billion
- Average clock frequency: 3.71 GHz
- Machine-language instructions: 117 billion
- Instructions per nanosecond (wallclock): ≈ 80.1
- Instructions per nanosecond (CPU time): ≈ 5.18
- Instructions per clock cycle: ≈ 1.39
- Instructions per arithmetic operation: 0.160
- Branches: 5.19%
- Branch mispredictions: 0.0326%
