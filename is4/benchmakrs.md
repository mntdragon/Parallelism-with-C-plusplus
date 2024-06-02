| Command        | Test cases | Total time |
|----------------|------------|------------|
| test-asan      | 89         | 6.3 s      |
| test-plain     | 89         | 4.3 s      |
| test-uninit    | 89         | 4.4 s      |
| test-plain     | 5          | 6.0 s      |
| benchmark-all  | 5          | 6.0 s      |

| Benchmark       | Time       |
|-----------------|------------|
| benchmarks/1    | 0.020144 s |
| benchmarks/2a   | 0.111497 s |
| benchmarks/2b   | 0.106456 s |
| benchmarks/2c   | 0.108167 s |
| benchmarks/3    | 1.772078 s |

# Measurements

Here are some additional technical details that I gathered while running the benchmarks.

### benchmarks/1 — CPU:

- This benchmark requires checking 25,502,500 different rectangles. As your running time was 0.0201 sec, it seems that you managed to evaluate at least 1.27 billion rectangles per second.
- Your code used 0.0201 sec of wallclock time, and 0.387 sec of CPU time, so you used 19.2 simultaneous hardware threads on average.
- The total number of clock cycles was 0.54 billion, and therefore it seems that the average clock frequency of the CPU was 1.37 GHz.
- The CPU executed 0.60 billion machine-language instructions ≈ 29.7 instructions per nanosecond (wallclock) ≈ 1.55 instructions per nanosecond (CPU time) ≈ 1.11 instructions per clock cycle.
- It seems you used 23.5 machine language instructions per rectangle evaluation.
- 8.90% of the instructions were branches and 0.94% of them were mispredicted.

### benchmarks/2a — CPU:

- This benchmark requires checking 396,010,000 different rectangles. As your running time was 0.111 sec, it seems that you managed to evaluate at least 3.55 billion rectangles per second.
- Your code used 0.111 sec of wallclock time, and 2.16 sec of CPU time, so you used 19.4 simultaneous hardware threads on average.
- The total number of clock cycles was 7.93 billion, and therefore it seems that the average clock frequency of the CPU was 3.57 GHz.
- The CPU executed 9.17 billion machine-language instructions ≈ 82.3 instructions per nanosecond (wallclock) ≈ 4.24 instructions per nanosecond (CPU time) ≈ 1.16 instructions per clock cycle.
- It seems you used 23.2 machine language instructions per rectangle evaluation.
- 8.73% of the instructions were branches and 0.55% of them were mispredicted.

### benchmarks/2b — CPU:

- This benchmark requires checking 404,010,000 different rectangles. As your running time was 0.106 sec, it seems that you managed to evaluate at least 3.80 billion rectangles per second.
- Your code used 0.106 sec of wallclock time, and 2.06 sec of CPU time, so you used 19.4 simultaneous hardware threads on average.
- The total number of clock cycles was 8.07 billion, and therefore it seems that the average clock frequency of the CPU was 3.79 GHz.
- The CPU executed 9.35 billion machine-language instructions ≈ 87.9 instructions per nanosecond (wallclock) ≈ 4.53 instructions per nanosecond (CPU time) ≈ 1.16 instructions per clock cycle.
- It seems you used 23.1 machine language instructions per rectangle evaluation.
- 8.73% of the instructions were branches and 0.52% of them were mispredicted.

### benchmarks/2c — CPU:

- This benchmark requires checking 412,130,601 different rectangles. As your running time was 0.108 sec, it seems that you managed to evaluate at least 3.81 billion rectangles per second.
- Your code used 0.108 sec of wallclock time, and 2.14 sec of CPU time, so you used 19.8 simultaneous hardware threads on average.
- The total number of clock cycles was 8.20 billion, and therefore it seems that the average clock frequency of the CPU was 3.79 GHz.
- The CPU executed 9.54 billion machine-language instructions ≈ 88.2 instructions per nanosecond (wallclock) ≈ 4.46 instructions per nanosecond (CPU time) ≈ 1.16 instructions per clock cycle.
- It seems you used 23.1 machine language instructions per rectangle evaluation.
- 8.74% of the instructions were branches and 0.448% of them were mispredicted.

### benchmarks/3 — CPU:

- This benchmark requires checking 6,432,040,000 different rectangles. As your running time was 1.77 sec, it seems that you managed to evaluate at least 3.63 billion rectangles per second.
- Your code used 1.77 sec of wallclock time, and 35.4 sec of CPU time, so you used 20.0 simultaneous hardware threads on average.
- The total number of clock cycles was 134 billion, and therefore it seems that the average clock frequency of the CPU was 3.79 GHz.
- The CPU executed 148 billion machine-language instructions ≈ 83.7 instructions per nanosecond (wallclock) ≈ 4.19 instructions per nanosecond (CPU time) ≈ 1.10 instructions per clock cycle.
- It seems you used 23.1 machine language instructions per rectangle evaluation.
- 8.71% of the instructions were branches and 0.241% of them were mispredicted.
