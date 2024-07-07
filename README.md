# Prime Number Detection with Multithreading

## Overview

This project aims to create a highly efficient and parallelized prime number counter using a random number generator to simulate an endless data stream. The goal is to optimize the prime checking process, utilizing multiple CPU cores while maintaining a low memory footprint under 2MB.

## Features

- Efficient random number generation.
- Optimized prime number checking.
- Parallel processing using multiple threads.

## Files

- `generator.c`: Generates a specified number of random numbers within a given range.
- `primeCounter.c`: Basic implementation of the prime counter.
- `new_primeCounter.c`: Optimized and parallelized implementation of the prime counter.
- `Makefile`: Compilation instructions for the project.
- `Proofs` folder: Contains screenshots proving the solution's efficiency and memory usage.

## Getting Started

### Prerequisites

- GCC compiler
- Python 3
- psutil library (`pip install psutil`)
- matplotlib library (`pip install matplotlib`)

### Compilation

To compile the project, navigate to the project directory and run:

```bash
make
```

This will compile the following executables:

- `randomGenerator`: Random number generator.
- `primeCounter`: Basic prime counter.
- `new_primeCounter`: Optimized prime counter.

### Usage

1. **Generate Random Numbers**

```bash
./randomGenerator <seed> <count>
```

- `<seed>`: Seed for the random number generator.
- `<count>`: Number of random numbers to generate.

Example:

```bash
./randomGenerator 10 100
```

2. **Count Primes Using Basic Prime Counter**

```bash
./randomGenerator <seed> <count> | ./primeCounter
```

Example:

```bash
./randomGenerator 10 100 | ./primeCounter
```

3. **Count Primes Using Optimized Prime Counter**

```bash
./randomGenerator <seed> <count> | ./new_primeCounter
```

Example:

```bash
./randomGenerator 10 100 | ./new_primeCounter
```


## Optimizations

### Prime Checking

- Early exit for small numbers.
- 6k ± 1 optimization to reduce the number of iterations.
- Checking up to the square root boundary for factors.

### Parallel Processing

- Utilizes multiple CPU cores to process numbers concurrently.


## Makefile

The `Makefile` includes targets for compiling the project and cleaning up generated files.

- `make`: Compiles the project.
- `make clean`: Cleans up generated executables.



## Measure Time and Memory Usage

Use the `time` command to measure performance:

For the original code:
```sh
time ./randomGenerator 10 10000000 | ./primeCounter
```

For the new and improved code:
```sh
time ./randomGenerator 10 10000000 | ./new_primeCounter
```

## Proofs

Below are the proofs for the improvements and resource usage:

**Original Prime Counter RAM Memory Usage:**
![Original Prime Counter RAM Memory Usage](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/original_RAM_usage.png)

**Print Screen of 10M Numbers Checked by Both Implementations:**
![Print Screen of 10M Numbers](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/time_compare_10M_numbers.png)

**Proof that improve RAM usage is under 2MB:**
![Proof RAM Usage Under 2MB](https://github.com/roni5604/Prime_Counter_Improvment/raw/main/Proofs/proof_under_2MB_RAM_10M_numbers.png)


## Conclusion

This project demonstrates effective utilization of multithreading to improve the performance of a prime number detection algorithm in a continuous data stream. By optimizing the `isPrime` function and parallelizing the processing, significant performance gains were achieved while keeping the memory usage within the specified limits.

## Authors

Roni Michaeli and Elor Israeli
