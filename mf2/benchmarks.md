| Command        | Test cases | Total time |
|----------------|------------|------------|
| test-asan      | 257        | 14.7 s     |
| test-plain     | 257        | 4.8 s      |
| test-uninit    | 257        | 4.8 s      |
| test-plain     | 4          | 9.1 s      |
| benchmark-all  | 4          | 4.3 s      |

| Benchmark      | Time       |
|----------------|------------|
| benchmarks/1   | 0.017125 s |
| benchmarks/2   | 0.093222 s |
| benchmarks/3   | 0.298438 s |
| benchmarks/4   | 0.671834 s |

## Measurements

Here are some additional technical details that I gathered while running the benchmarks.

**benchmarks/1 — CPU:**

- Your code used 0.0171 sec of wallclock time, and 0.294 sec of CPU time, so you used 17.2 simultaneous hardware threads on average.
- The total number of clock cycles was 0.396 billion, and therefore it seems that the average clock frequency of the CPU was 1.24 GHz.
- The CPU executed 0.287 billion machine-language instructions ≈ 16.8 instructions per nanosecond (wallclock) ≈ 0.98 instructions per nanosecond (CPU time) ≈ 0.73 instructions per clock cycle.
- 20.4% of the instructions were branches and 7.93% of them were mispredicted.

**benchmarks/2 — CPU:**

- Your code used 0.093 sec of wallclock time, and 1.77 sec of CPU time, so you used 19.0 simultaneous hardware threads on average.
- The total number of clock cycles was 6.44 billion, and therefore it seems that the average clock frequency of the CPU was 3.63 GHz.
- The CPU executed 4.99 billion machine-language instructions ≈ 53.5 instructions per nanosecond (wallclock) ≈ 2.81 instructions per nanosecond (CPU time) ≈ 0.78 instructions per clock cycle.
- 21.4% of the instructions were branches and 11.4% of them were mispredicted.

**benchmarks/3 — CPU:**

- Your code used 0.298 sec of wallclock time, and 5.75 sec of CPU time, so you used 19.3 simultaneous hardware threads on average.
- The total number of clock cycles was 24.4 billion, and therefore it seems that the average clock frequency of the CPU was 4.25 GHz.
- The CPU executed 18.9 billion machine-language instructions ≈ 63.2 instructions per nanosecond (wallclock) ≈ 3.28 instructions per nanosecond (CPU time) ≈ 0.77 instructions per clock cycle.
- 21.6% of the instructions were branches and 12.0% of them were mispredicted.

**benchmarks/4 — CPU:**

- Your code used 0.67 sec of wallclock time, and 13.0 sec of CPU time, so you used 19.3 simultaneous hardware threads on average.
- The total number of clock cycles was 54.6 billion, and therefore it seems that the average clock frequency of the CPU was 4.19 GHz.
- The CPU executed 41.7 billion machine-language instructions ≈ 62.0 instructions per nanosecond (wallclock) ≈ 3.21 instructions per nanosecond (CPU time) ≈ 0.76 instructions per clock cycle.
- 21.7% of the instructions were branches and 12.2% of them were mispredicted.
