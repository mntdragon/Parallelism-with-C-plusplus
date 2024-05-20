# Benchmarks

| Benchmark      | Time        | Operations       | GFLOPS | Description                                                   |
|----------------|-------------|------------------|--------|---------------------------------------------------------------|
| benchmarks/1   | 0.055075 s  | 1,004,000,000    | 18.2   | The input contains 1000 × 1000 pixels, and the output should contain 1000 × 1000 pixels |
| benchmarks/2   | 0.537870 s  | 16,016,000,000   | 29.8   | The input contains 4000 × 1000 pixels, and the output should contain 4000 × 4000 pixels |

# Benchmarks Analysis

## Benchmarks/2 — CPU:

### Performance Summary

- **Expected Operations**: At least 16,016,000,000 arithmetic operations.
- **Actual Running Time**: 0.54 seconds.
- **Useful Arithmetic Operations**: At least 29.8 billion per second.
- **Wallclock Time**: 0.54 seconds.
- **CPU Time**: 9.76 seconds.
- **Simultaneous Hardware Threads**: 18.1 on average.
- **Total Clock Cycles**: 38.2 billion.
- **Average CPU Clock Frequency**: 3.92 GHz.
- **Executed Machine-Language Instructions**: 40.8 billion.
  - **Instructions per Nanosecond (Wallclock)**: ~75.9
  - **Instructions per Nanosecond (CPU Time)**: ~4.18
  - **Instructions per Clock Cycle**: ~1.07
- **Instructions per Useful Arithmetic Operation**: 2.55
- **Branch Instructions**: 19.9% of the total instructions.
- **Branch Mispredictions**: 0.101% of the total branches.
