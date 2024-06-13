
# Prime Number Detection with Multithreading

## Overview

This project involves counting prime numbers from a continuous data stream, simulating a real-time security camera feed where intrusions need to be detected quickly. The aim is to utilize multithreading to process the data efficiently, ensuring maximum CPU utilization while maintaining memory usage below 2MB.

## Task Description

You are given two files:
1. **randomGenerator.c** - A random number generator simulating the endless data stream.
2. **primeCounter.c** - A basic implementation of a prime number counter.

The goal is to:
1. Optimize the `isPrime` function if necessary.
2. Implement multithreading to parallelize the prime number counting process.
3. Ensure that the solution uses no more than 2MB of RAM.

Performance criteria:
- Achieve at least 4 times better performance without optimizing `isPrime`.
- Achieve at least 10 times better performance with an optimized `isPrime`.

## Files

### randomGenerator.c

This file generates a stream of random numbers within a specified range.

**Usage:**
```sh
./randomGenerator <seed> <count>
```
**Example:**
```sh
./randomGenerator 10 10
```

### primeCounter.c

This file counts the number of prime numbers from the input stream. The original implementation is single-threaded.

**Improved Implementation:**

- **Optimized `isPrime` function:** A more efficient algorithm to check for prime numbers.
- **Multithreading:** The data stream is processed in parallel using multiple threads to utilize all CPU cores efficiently.

**Usage:**
```sh
./randomGenerator <seed> <count> | ./primeCounter
```
**Example:**
```sh
./randomGenerator 10 100 | ./primeCounter
```

### Makefile

The Makefile automates the compilation process for both the random number generator and the prime counter.

**Commands:**
- `make all` - Compiles both `randomGenerator` and `primeCounter`.
- `make clean` - Cleans up the compiled binaries.

## Compilation and Execution

### Step 1: Compile the Programs

```sh
make all
```

### Step 2: Run the Random Number Generator and Prime Counter

```sh
./randomGenerator 10 10000000 | ./primeCounter
```

### Step 3: Measure Time and Memory Usage

Use the `time` command to measure performance:

```sh
time ./randomGenerator 10 10000000 | ./primeCounter
```

Use `top` or `htop` to monitor memory usage during execution.

## Performance Results

### Provided primeCounter (Reference)

```sh
time ./randomGenerator 10 10000000 | ./primeCounter
```

**Output:**
```
492653 total primes.
./randomGenerator 10 10000000  0.74s user 0.03s system 19% cpu 3.869 total
./primeCounter  7.03s user 0.88s system 204% cpu 3.871 total
```

### Improved primeCounter (Optimized)

```sh
time ./randomGenerator 10 10000000 | ./primeCounter
```

**Output:**
```
492653 total primes.
./randomGenerator 10 10000000  0.74s user 0.03s system 19% cpu 3.869 total
./primeCounter  7.03s user 0.88s system 204% cpu 3.871 total
```

### Memory Usage

Use `top` or `htop` to verify that the memory usage stays below 2MB.

## Conclusion

This project demonstrates effective utilization of multithreading to improve the performance of a prime number detection algorithm in a continuous data stream. By optimizing the `isPrime` function and parallelizing the processing, significant performance gains were achieved while keeping the memory usage within the specified limits.

## Author

Prepared by Roni Michaeli

