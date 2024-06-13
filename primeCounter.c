#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 4
#define BUFFER_SIZE 1024

typedef struct {
    int *buffer;
    int start;
    int end;
    int *prime_count;
    pthread_mutex_t *mutex;
} ThreadData;

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void *countPrimes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;

    for (int i = data->start; i < data->end; i++) {
        if (isPrime(data->buffer[i])) {
            count++;
        }
    }

    pthread_mutex_lock(data->mutex);
    *(data->prime_count) += count;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main() {
    int buffer[BUFFER_SIZE];
    int prime_count = 0;
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int num;
    int index = 0;

    while (scanf("%d", &num) != EOF) {
        buffer[index++] = num;

        if (index == BUFFER_SIZE) {
            int chunk_size = BUFFER_SIZE / MAX_THREADS;

            for (int i = 0; i < MAX_THREADS; i++) {
                thread_data[i].buffer = buffer;
                thread_data[i].start = i * chunk_size;
                thread_data[i].end = (i + 1) * chunk_size;
                thread_data[i].prime_count = &prime_count;
                thread_data[i].mutex = &mutex;
                pthread_create(&threads[i], NULL, countPrimes, &thread_data[i]);
            }

            for (int i = 0; i < MAX_THREADS; i++) {
                pthread_join(threads[i], NULL);
            }

            index = 0;
        }
    }

    if (index > 0) {
        int chunk_size = index / MAX_THREADS;

        for (int i = 0; i < MAX_THREADS; i++) {
            thread_data[i].buffer = buffer;
            thread_data[i].start = i * chunk_size;
            thread_data[i].end = (i + 1) * chunk_size;
            if (i == MAX_THREADS - 1) {
                thread_data[i].end = index;
            }
            thread_data[i].prime_count = &prime_count;
            thread_data[i].mutex = &mutex;
            pthread_create(&threads[i], NULL, countPrimes, &thread_data[i]);
        }

        for (int i = 0; i < MAX_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    printf("%d total primes.\n", prime_count);

    return 0;
}
