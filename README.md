# Prime Number Detection with Multithreading

## Overview

This project aims to efficiently count prime numbers from a continuous data stream, simulating a real-time security camera feed where intrusions need to be detected quickly. The goal is to use multithreading to process the data stream efficiently, ensuring maximum CPU utilization while keeping memory usage below 2MB.

## Task Description

You are provided with two main components:
1. **randomGenerator**: A random number generator simulating the endless data stream.
2. **primeCounter**: A basic implementation of a prime number counter.

### Goals
1. Optimize the `isPrime` function if necessary.
2. Implement multithreading to parallelize the prime number counting process.
3. Ensure that the solution uses no more than 2MB of RAM.

### Performance Criteria
- Achieve at least 4 times better performance without optimizing `isPrime`.
- Achieve at least 10 times better performance with an optimized `isPrime`.

## Files

### randomGenerator.c

This file generates a stream of random numbers based on a provided seed and count.

**Usage:**
```sh
./randomGenerator <seed> <count>
```
**Example:**
```sh
./randomGenerator 10 10
```

### primeCounter.c

This file reads numbers from standard input, checks for prime numbers, and counts them. The original implementation is single-threaded.

**Improvements:**
- **Optimized `isPrime` Function:** Uses a more efficient algorithm to check for prime numbers.
- **Multithreading:** Processes the data stream in parallel using multiple threads to utilize all CPU cores efficiently.

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

## Monitoring Resource Usage

To monitor CPU and memory usage, a Python script (`monitor_resources.py`) is used, which runs the `randomGenerator` and `primeCounter`, capturing and displaying resource usage statistics.


### Running the Monitoring Script

1. **Run the Monitoring Script**:
    ```sh
    python3 monitor_resources.py
    ```

### Memory Usage

Use `top` or `htop` to verify that the memory usage stays below 2MB.

**Example using `top`:**
1. Open another terminal.
2. Run `top`.
3. Look for the `primeCounter` process and observe its memory usage.

## Conclusion

This project demonstrates effective utilization of multithreading to improve the performance of a prime number detection algorithm in a continuous data stream. By optimizing the `isPrime` function and parallelizing the processing, significant performance gains were achieved while keeping the memory usage within the specified limits.

## Author

Prepared by Roni Michaeli