| Benchmark      | Time        | Description                                                           |
|----------------|-------------|-----------------------------------------------------------------------|
| benchmarks/1   | 0.005278 s  | the input contains 100000 integers, and the output should contain the same integers in order |
| benchmarks/2   | 0.027133 s  | the input contains 1000000 integers, and the output should contain the same integers in order |
| benchmarks/3a  | 0.127132 s  | the input contains 10000000 integers, and the output should contain the same integers in order |
| benchmarks/3b  | 0.127818 s  | the input contains 10000000 integers, and the output should contain the same integers in order |
| benchmarks/3c  | 0.123880 s  | the input contains 9999997 integers, and the output should contain the same integers in order |
| benchmarks/3d  | 0.137428 s  | the input contains 9999998 integers, and the output should contain the same integers in order |
| benchmarks/3e  | 0.132901 s  | the input contains 9999999 integers, and the output should contain the same integers in order |
| benchmarks/3f  | 0.120310 s  | the input contains 10000001 integers, and the output should contain the same integers in order |
| benchmarks/3g  | 0.120256 s  | the input contains 10000002 integers, and the output should contain the same integers in order |
| benchmarks/3h  | 0.127008 s  | the input contains 10000003 integers, and the output should contain the same integers in order |
| benchmarks/4   | 0.954647 s  | the input contains 100000000 integers, and the output should contain the same integers in order |

# Measurements

Here are some additional technical details that I gathered while running the benchmarks.

## Benchmarks Summary

| Benchmark      | Time        | Description                                                           |
|----------------|-------------|-----------------------------------------------------------------------|
| benchmarks/1   | 0.005278 s  | the input contains 100000 integers, and the output should contain the same integers in order |
| benchmarks/2   | 0.027133 s  | the input contains 1000000 integers, and the output should contain the same integers in order |
| benchmarks/3a  | 0.127132 s  | the input contains 10000000 integers, and the output should contain the same integers in order |
| benchmarks/3b  | 0.127818 s  | the input contains 10000000 integers, and the output should contain the same integers in order |
| benchmarks/3c  | 0.123880 s  | the input contains 9999997 integers, and the output should contain the same integers in order |
| benchmarks/3d  | 0.137428 s  | the input contains 9999998 integers, and the output should contain the same integers in order |
| benchmarks/3e  | 0.132901 s  | the input contains 9999999 integers, and the output should contain the same integers in order |
| benchmarks/3f  | 0.120310 s  | the input contains 10000001 integers, and the output should contain the same integers in order |
| benchmarks/3g  | 0.120256 s  | the input contains 10000002 integers, and the output should contain the same integers in order |
| benchmarks/3h  | 0.127008 s  | the input contains 10000003 integers, and the output should contain the same integers in order |
| benchmarks/4   | 0.954647 s  | the input contains 100000000 integers, and the output should contain the same integers in order |

## Technical Details

### benchmarks/1 — CPU:
- Your code used 0.0053 sec of wallclock time, and 0.0270 sec of CPU time, so you used 5.11 simultaneous hardware threads on average.
- The total number of clock cycles was 0.054 billion, and therefore it seems that the average clock frequency of the CPU was 1.16 GHz.
- The CPU executed 0.0215 billion machine-language instructions ≈ 4.07 instructions per nanosecond (wallclock) ≈ 0.80 instructions per nanosecond (CPU time) ≈ 0.400 instructions per clock cycle.
- 24.6% of the instructions were branches and 4.11% of them were mispredicted.

### benchmarks/2 — CPU:
- Your code used 0.0271 sec of wallclock time, and 0.329 sec of CPU time, so you used 12.1 simultaneous hardware threads on average. 0.0061 sec (≈1.86%) of the running time was spent outside user space.
- The total number of clock cycles was 0.494 billion, and therefore it seems that the average clock frequency of the CPU was 1.38 GHz.
- The CPU executed 0.195 billion machine-language instructions ≈ 7.19 instructions per nanosecond (wallclock) ≈ 0.59 instructions per nanosecond (CPU time) ≈ 0.395 instructions per clock cycle.
- 25.7% of the instructions were branches and 4.11% of them were mispredicted.

### benchmarks/3a — CPU:
- Your code used 0.127 sec of wallclock time, and 1.28 sec of CPU time, so you used 10.1 simultaneous hardware threads on average. 0.092 sec (≈7.17%) of the running time was spent outside user space.
- The total number of clock cycles was 3.21 billion, and therefore it seems that the average clock frequency of the CPU was 2.51 GHz.
- The CPU executed 1.96 billion machine-language instructions ≈ 15.5 instructions per nanosecond (wallclock) ≈ 1.54 instructions per nanosecond (CPU time) ≈ 0.61 instructions per clock cycle.
- 24.8% of the instructions were branches and 4.72% of them were mispredicted.

### benchmarks/3b — CPU:
- Your code used 0.128 sec of wallclock time, and 1.27 sec of CPU time, so you used 9.94 simultaneous hardware threads on average. 0.122 sec (≈9.61%) of the running time was spent outside user space.
- The total number of clock cycles was 3.08 billion, and therefore it seems that the average clock frequency of the CPU was 2.42 GHz.
- The CPU executed 1.95 billion machine-language instructions ≈ 15.3 instructions per nanosecond (wallclock) ≈ 1.54 instructions per nanosecond (CPU time) ≈ 0.63 instructions per clock cycle.
- 24.8% of the instructions were branches and 4.73% of them were mispredicted.

### benchmarks/3c — CPU:
- Your code used 0.124 sec of wallclock time, and 1.17 sec of CPU time, so you used 9.43 simultaneous hardware threads on average. 0.067 sec (≈5.75%) of the running time was spent outside user space.
- The total number of clock cycles was 3.22 billion, and therefore it seems that the average clock frequency of the CPU was 2.76 GHz.
- The CPU executed 1.92 billion machine-language instructions ≈ 15.5 instructions per nanosecond (wallclock) ≈ 1.64 instructions per nanosecond (CPU time) ≈ 0.60 instructions per clock cycle.
- 24.5% of the instructions were branches and 4.85% of them were mispredicted.

### benchmarks/3d — CPU:
- Your code used 0.137 sec of wallclock time, and 1.32 sec of CPU time, so you used 9.60 simultaneous hardware threads on average. 0.092 sec (≈6.99%) of the running time was spent outside user space.
- The total number of clock cycles was 3.48 billion, and therefore it seems that the average clock frequency of the CPU was 2.64 GHz.
- The CPU executed 1.95 billion machine-language instructions ≈ 14.2 instructions per nanosecond (wallclock) ≈ 1.48 instructions per nanosecond (CPU time) ≈ 0.56 instructions per clock cycle.
- 24.6% of the instructions were branches and 4.79% of them were mispredicted.

### benchmarks/3e — CPU:
- Your code used 0.133 sec of wallclock time, and 1.24 sec of CPU time, so you used 9.34 simultaneous hardware threads on average. 0.106 sec (≈8.50%) of the running time was spent outside user space.
- The total number of clock cycles was 2.96 billion, and therefore it seems that the average clock frequency of the CPU was 2.38 GHz.
- The CPU executed 1.89 billion machine-language instructions ≈ 14.2 instructions per nanosecond (wallclock) ≈ 1.52 instructions per nanosecond (CPU time) ≈ 0.64 instructions per clock cycle.
- 24.4% of the instructions were branches and 4.99% of them were mispredicted.

### benchmarks/3f — CPU:
- Your code used 0.120 sec of wallclock time, and 1.21 sec of CPU time, so you used 10.0 simultaneous hardware threads on average. 0.0448 sec (≈3.71%) of the running time was spent outside user space.
- The total number of clock cycles was 3.18 billion, and therefore it seems that the average clock frequency of the CPU was 2.64 GHz.
- The CPU executed 1.92 billion machine-language instructions ≈ 15.9 instructions per nanosecond (wallclock) ≈ 1.59 instructions per nanosecond (CPU time) ≈ 0.60 instructions per clock cycle.
- 24.5% of the instructions were branches and 4.89% of them were mispredicted.

### benchmarks/3g — CPU:
- Your code used 0.120 sec of wallclock time, and 1.17 sec of CPU time, so you used 9.70 simultaneous hardware threads on average. 0.058 sec (≈4.94%) of the running time was spent outside user space.
- The total number of clock cycles was 3.37 billion, and therefore it seems that the average clock frequency of the CPU was 2.89 GHz.
- The CPU executed 1.94 billion machine-language instructions ≈ 16.1 instructions per nanosecond (wallclock) ≈ 1.67 instructions per nanosecond (CPU time) ≈ 0.58 instructions per clock cycle.
- 24.6% of the instructions were branches and 4.78% of them were mispredicted.

### benchmarks/3h — CPU:
- Your code used 0.127 sec of wallclock time, and 1.28 sec of CPU time, so you used 10.1 simultaneous hardware threads on average. 0.117 sec (≈9.17%) of the running time was spent outside user space.
- The total number of clock cycles was 3.11 billion, and therefore it seems that the average clock frequency of the CPU was 2.43 GHz.
- The CPU executed 1.91 billion machine-language instructions ≈ 15.0 instructions per nanosecond (wallclock) ≈ 1.49 instructions per nanosecond (CPU time) ≈ 0.62 instructions per clock cycle.
- 24.5% of the instructions were branches and 4.91% of them were mispredicted.

### benchmarks/4 — CPU:
- Your code used 0.95 sec of wallclock time, and 6.65 sec of CPU time, so you used 6.96 simultaneous hardware threads on average. 0.75 sec (≈11.2%) of the running time was spent outside user space.
- The total number of clock cycles was 24.8 billion, and therefore it seems that the average clock frequency of the CPU was 3.72 GHz.
- The CPU executed 20.0 billion machine-language instructions ≈ 21.0 instructions per nanosecond (wallclock) ≈ 3.01 instructions per nanosecond (CPU time) ≈ 0.81 instructions per clock cycle.
- 24.7% of the instructions were branches and 5.13% of them were mispredicted.
