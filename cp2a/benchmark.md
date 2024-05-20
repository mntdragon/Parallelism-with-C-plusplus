# Benchmarks

| Benchmark      | Time        | Operations       | GFLOPS | Description                                                   |
|----------------|-------------|------------------|--------|---------------------------------------------------------------|
| benchmarks/1   | 0.270531 s  | 1,004,000,000    | 3.7    | The input contains 1000 × 1000 pixels, and the output should contain 1000 × 1000 pixels |
| benchmarks/2   | 4.721897 s  | 16,016,000,000   | 3.4    | The input contains 4000 × 1000 pixels, and the output should contain 4000 × 4000 pixels |

# Benchmarks Analysis

## Benchmarks/2 — CPU:

### Performance Summary

- **Expected Operations**: At least 16,016,000,000 arithmetic operations.
- **Actual Running Time**: 4.72 seconds.
- **Useful Arithmetic Operations**: At least 3.39 billion per second.
- **Wallclock Time**: 4.72 seconds.
- **CPU Time**: 4.72 seconds.
- **Simultaneous Hardware Threads**: 1.00 on average.
- **Total Clock Cycles**: 21.2 billion.
- **Average CPU Clock Frequency**: 4.49 GHz.
- **Executed Machine-Language Instructions**: 66.6 billion.
  - **Instructions per Nanosecond (Wallclock)**: ~14.1
  - **Instructions per Nanosecond (CPU Time)**: ~14.1
  - **Instructions per Clock Cycle**: ~3.15
- **Instructions per Useful Arithmetic Operation**: 4.16
- **Branch Instructions**: 12.1% of the total instructions.
- **Branch Mispredictions**: 0.100% of the total branches.
