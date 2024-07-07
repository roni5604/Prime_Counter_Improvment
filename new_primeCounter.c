#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 4
#define BUFFER_SIZE 256 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int total_primes = 0;

/*
 * Optimized Prime Checking Function
 * 
 * Techniques Used:
 * 1. Early Exit for Small Numbers:
 *    - Directly return false for numbers <= 1.
 *    - Directly return true for numbers 2 and 3 (smallest primes).
 *    - Eliminate multiples of 2 and 3 early.
 *
 * 2. 6k ± 1 Optimization:
 *    - Any integer can be expressed in the form 6k + i where i is one of 0, 1, 2, 3, 4, 5.
 *    - All primes greater than 3 can be expressed as 6k ± 1.
 *    - This allows us to skip checking numbers that are not of the form 6k ± 1, reducing the number of iterations.
 *
 * 3. Square Root Boundary:
 *    - We only need to check for factors up to the square root of n.
 *    - If n has a factor larger than its square root, it must also have a smaller factor.
 */
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true; // 2 and 3 are prime
    if (n % 2 == 0 || n % 3 == 0) return false; // eliminate multiples of 2 and 3

    // Check from 5 to sqrt(n), in steps of 6
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

void* checkPrime(void* arg) {
    int* numbers = (int*)arg;
    int local_counter = 0;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (numbers[i] == -1) break;
        if (isPrime(numbers[i])) {
            local_counter++;
        }
    }

    pthread_mutex_lock(&mutex);
    total_primes += local_counter;
    pthread_mutex_unlock(&mutex);

    free(numbers);
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int* buffer = (int*)malloc(BUFFER_SIZE * sizeof(int));
    int num, index = 0, active_threads = 0;

    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    while (scanf("%d", &num) != EOF) {
        buffer[index++] = num;
        if (index == BUFFER_SIZE) {
            int* numbers = buffer;
            pthread_create(&threads[active_threads++], NULL, checkPrime, (void*)numbers);
            buffer = (int*)malloc(BUFFER_SIZE * sizeof(int));
            if (!buffer) {
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            index = 0;
            if (active_threads == MAX_THREADS) {
                for (int i = 0; i < MAX_THREADS; i++) {
                    pthread_join(threads[i], NULL);
                }
                active_threads = 0;
            }
        }
    }

    if (index > 0) {
        for (int i = index; i < BUFFER_SIZE; i++) buffer[i] = -1;
        pthread_create(&threads[active_threads++], NULL, checkPrime, (void*)buffer);
    }

    for (int i = 0; i < active_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_primes);

    pthread_mutex_destroy(&mutex);
    return 0;
}
