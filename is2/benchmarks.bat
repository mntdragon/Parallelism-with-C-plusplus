| Command       | Test cases | Total time |
|---------------|------------|------------|
| test-asan     | 62         | 5.2 s      |
| test-plain    | 62         | 4.2 s      |
| test-uninit   | 62         | 4.2 s      |
| test-plain    | 5          | 4.0 s      |
| benchmark-all | 5          | 4.1 s      |

| Benchmark     | Time       |
|---------------|------------|
| benchmarks/1  | 0.010711 s |
| benchmarks/2a | 0.023471 s |
| benchmarks/2b | 0.026652 s |
| benchmarks/2c | 0.029309 s |
| benchmarks/3  | 0.042355 s |

# Measurements

Here are some additional technical details that I gathered while running the benchmarks.

## benchmarks/1 — CPU:

- This benchmark requires checking 216,225 different rectangles. As your running time was 0.0107 sec, it seems that you managed to evaluate at least 0.0202 billion rectangles per second.
- Your code used 0.0107 sec of wallclock time, and 0.0107 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 0.0128 billion, and therefore it seems that the average clock frequency of the CPU was 1.19 GHz.
- The CPU executed 0.0237 billion machine-language instructions ≈ 2.21 instructions per nanosecond (wallclock) ≈ 2.22 instructions per nanosecond (CPU time) ≈ 1.85 instructions per clock cycle.
- It seems you used 110 machine language instructions per rectangle evaluation.
- 3.05% of the instructions were branches and 0.73% of them were mispredicted.

## benchmarks/2a — CPU:

- This benchmark requires checking 608,400 different rectangles. As your running time was 0.0235 sec, it seems that you managed to evaluate at least 0.0259 billion rectangles per second.
- Your code used 0.0235 sec of wallclock time, and 0.0235 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 0.0355 billion, and therefore it seems that the average clock frequency of the CPU was 1.51 GHz.
- The CPU executed 0.066 billion machine-language instructions ≈ 2.79 instructions per nanosecond (wallclock) ≈ 2.80 instructions per nanosecond (CPU time) ≈ 1.85 instructions per clock cycle.
- It seems you used 108 machine language instructions per rectangle evaluation.
- 3.00% of the instructions were branches and 0.53% of them were mispredicted.

## benchmarks/2b — CPU:

- This benchmark requires checking 672,400 different rectangles. As your running time was 0.0267 sec, it seems that you managed to evaluate at least 0.0252 billion rectangles per second.
- Your code used 0.0267 sec of wallclock time, and 0.0266 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 0.0391 billion, and therefore it seems that the average clock frequency of the CPU was 1.47 GHz.
- The CPU executed 0.072 billion machine-language instructions ≈ 2.72 instructions per nanosecond (wallclock) ≈ 2.72 instructions per nanosecond (CPU time) ≈ 1.85 instructions per clock cycle.
- It seems you used 108 machine language instructions per rectangle evaluation.
- 2.99% of the instructions were branches and 0.494% of them were mispredicted.

## benchmarks/2c — CPU:

- This benchmark requires checking 741,321 different rectangles. As your running time was 0.0293 sec, it seems that you managed to evaluate at least 0.0253 billion rectangles per second.
- Your code used 0.0293 sec of wallclock time, and 0.0293 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 0.0431 billion, and therefore it seems that the average clock frequency of the CPU was 1.47 GHz.
- The CPU executed 0.080 billion machine-language instructions ≈ 2.72 instructions per nanosecond (wallclock) ≈ 2.72 instructions per nanosecond (CPU time) ≈ 1.85 instructions per clock cycle.
- It seems you used 107 machine language instructions per rectangle evaluation.
- 2.99% of the instructions were branches and 0.496% of them were mispredicted.

## benchmarks/3 — CPU:

- This benchmark requires checking 1,625,625 different rectangles. As your running time was 0.0424 sec, it seems that you managed to evaluate at least 0.0384 billion rectangles per second.
- Your code used 0.0424 sec of wallclock time, and 0.0423 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 0.095 billion, and therefore it seems that the average clock frequency of the CPU was 2.25 GHz.
- The CPU executed 0.173 billion machine-language instructions ≈ 4.09 instructions per nanosecond (wallclock) ≈ 4.09 instructions per nanosecond (CPU time) ≈ 1.82 instructions per clock cycle.
- It seems you used 106 machine language instructions per rectangle evaluation.
- 2.97% of the instructions were branches and 0.397% of them were mispredicted.
