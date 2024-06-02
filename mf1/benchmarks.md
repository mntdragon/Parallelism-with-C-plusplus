| Command       | Test cases | Total time |
|---------------|------------|------------|
| test-asan     | 257        | 21.1 s     |
| test-plain    | 257        | 5.2 s      |
| test-uninit   | 257        | 5.2 s      |
| test-plain    | 4          | 21.2 s     |
| benchmark-all | 4          | 16.3 s     |

| Benchmark     | Time       |
|---------------|------------|
| benchmarks/1  | 0.082842 s |
| benchmarks/2  | 0.943321 s |
| benchmarks/3  | 3.735070 s |
| benchmarks/4  | 8.373319 s |

# Measurements

Here are some additional technical details that I gathered while running the benchmarks.

## benchmarks/1 — CPU:

- Your code used 0.083 sec of wallclock time, and 0.083 sec of CPU time, so you used 1.00 simultaneous hardware threads on average. 0.00153 sec (≈1.84%) of the running time was spent outside user space.
- The total number of clock cycles was 0.180 billion, and therefore it seems that the average clock frequency of the CPU was 2.18 GHz.
- The CPU executed 0.265 billion machine-language instructions ≈ 3.20 instructions per nanosecond (wallclock) ≈ 3.20 instructions per nanosecond (CPU time) ≈ 1.47 instructions per clock cycle.
- 20.0% of the instructions were branches and 8.43% of them were mispredicted.

## benchmarks/2 — CPU:

- Your code used 0.94 sec of wallclock time, and 0.94 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 4.23 billion, and therefore it seems that the average clock frequency of the CPU was 4.48 GHz.
- The CPU executed 4.90 billion machine-language instructions ≈ 5.20 instructions per nanosecond (wallclock) ≈ 5.20 instructions per nanosecond (CPU time) ≈ 1.16 instructions per clock cycle.
- 21.5% of the instructions were branches and 11.0% of them were mispredicted.

## benchmarks/3 — CPU:

- Your code used 3.74 sec of wallclock time, and 3.74 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 16.7 billion, and therefore it seems that the average clock frequency of the CPU was 4.48 GHz.
- The CPU executed 18.7 billion machine-language instructions ≈ 4.99 instructions per nanosecond (wallclock) ≈ 4.99 instructions per nanosecond (CPU time) ≈ 1.11 instructions per clock cycle.
- 21.8% of the instructions were branches and 11.5% of them were mispredicted.

## benchmarks/4 — CPU:

- Your code used 8.37 sec of wallclock time, and 8.37 sec of CPU time, so you used 1.00 simultaneous hardware threads on average.
- The total number of clock cycles was 37.5 billion, and therefore it seems that the average clock frequency of the CPU was 4.48 GHz.
- The CPU executed 41.2 billion machine-language instructions ≈ 4.92 instructions per nanosecond (wallclock) ≈ 4.92 instructions per nanosecond (CPU time) ≈ 1.10 instructions per clock cycle.
- 21.9% of the instructions were branches and 11.7% of them were mispredicted.
