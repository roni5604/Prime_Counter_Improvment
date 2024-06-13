#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <mach/mach.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "lock_free_queue.h"

#define MAX_THREADS 4

typedef struct {
    LockFreeQueue *queue;
    int *prime_count;
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
    int value;

    while (1) {
        if (dequeue(data->queue, &value)) {
            if (value == -1) break; // Sentinel value to stop the thread
            if (isPrime(value)) {
                __sync_add_and_fetch(data->prime_count, 1);
            }
        }
    }

    return NULL;
}

void printMemoryUsage(const char* tag) {
    mach_task_basic_info_data_t info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
        printf("%s Memory used: %llu bytes\n", tag, (uint64_t)info.resident_size);
    } else {
        printf("%s Failed to get memory usage info\n", tag);
    }
}

void printCPUUsage(const char* tag) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("%s CPU time: user %ld.%06d sec, system %ld.%06d sec\n",
           tag, usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
           usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}

int main() {
    LockFreeQueue queue;
    initQueue(&queue);

    int prime_count = 0;
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        thread_data[i].queue = &queue;
        thread_data[i].prime_count = &prime_count;
        pthread_create(&threads[i], NULL, countPrimes, &thread_data[i]);
    }

    // Print initial memory and CPU usage
    printMemoryUsage("Initial");
    printCPUUsage("Initial");

    int num;
    while (scanf("%d", &num) != EOF) {
        enqueue(&queue, num);
    }

    // Signal threads to stop by enqueueing a sentinel value
    for (int i = 0; i < MAX_THREADS; i++) {
        enqueue(&queue, -1);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", prime_count);

    // Print final memory and CPU usage
    printMemoryUsage("Final");
    printCPUUsage("Final");

    destroyQueue(&queue);

    return 0;
}
