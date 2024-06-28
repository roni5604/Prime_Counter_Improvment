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
- **`isPrime` Function:** simple check for prime numbers.

**Usage:**
```sh
./randomGenerator <seed> <count> | ./primeCounter
```
**Example:**
```sh
./randomGenerator 10 100 | ./primeCounter
```

### new_primeCounter.c

This file reads numbers from standard input, checks for prime numbers, and counts them. The improved implementation with multi-threading.

**Improvements:**
- **Optimized `isPrime` Function:** Uses a more efficient algorithm to check for prime numbers.
- **Multithreading:** Processes the data stream in parallel using multiple threads to utilize all CPU cores efficiently.

**Usage:**
```sh
./randomGenerator <seed> <count> | ./new_primeCounter
```
**Example:**
```sh
./randomGenerator 10 100 | ./new_primeCounter
```

### Makefile

The Makefile automates the compilation process for all relevant files: the random number generator, the original prime counter and the improved prime counter (the new).

**Commands:**
- `make all` - Compiles `randomGenerator`, `primeCounter` and `new_primeCounter`.
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

For the original code:
```sh
time ./randomGenerator 10 10000000 | ./primeCounter
```

For the new and improved code:
```sh
time ./randomGenerator 10 10000000 | ./new_primeCounter
```

## Monitoring Resource Usage

To monitor CPU and memory usage, a Python script (`monitor_resources.py`) is used, which runs the `randomGenerator` and `new_primeCounter`, capturing and displaying resource usage statistics.


### Running the Monitoring Script
To be able to see the live graph:

**Run the Monitoring Script**:
```sh
python3 monitor_resources.py
```

## Proofs

Below are the proofs for the improvements and resource usage:

**Original Prime Counter Memory and CPU Usage:**
![Original Prime Counter Memory and CPU](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/original_primeCounter_Memory_and_CPU.png)

**Print Screen of 10M Numbers Checked by Both Implementations:**
![Print Screen of 10M Numbers](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/printScreen_10M_both_times.png)

**Proof that RAM Usage is Under 2MB:**
![Proof RAM Usage Under 2MB](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/proof_RAM_under_2MB.png)


## Conclusion

This project demonstrates effective utilization of multithreading to improve the performance of a prime number detection algorithm in a continuous data stream. By optimizing the `isPrime` function and parallelizing the processing, significant performance gains were achieved while keeping the memory usage within the specified limits.

## Authors

Roni Michaeli and Elor Israeli
