#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>

#define MAX_QUEUE_SIZE 512 // Adjusted to ensure we stay within 2MB limit with overhead

// Node structure for the queue
typedef struct Node {
    int value;
    struct Node *next;
} Node;

// Lock-free queue structure
typedef struct {
    Node *head;
    Node *tail;
    atomic_int size;
} Queue;

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

// Initialize the queue
Queue* createQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    Node *dummy = (Node*)malloc(sizeof(Node));
    dummy->next = NULL;
    queue->head = queue->tail = dummy;
    atomic_init(&queue->size, 0);
    return queue;
}

// Enqueue operation (lock-free)
void enqueue(Queue *queue, int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    Node *tail;

    while (1) {
        tail = queue->tail;
        Node *next = tail->next;
        if (tail == queue->tail) {
            if (next == NULL) {
                if (__sync_bool_compare_and_swap(&tail->next, next, newNode)) {
                    __sync_bool_compare_and_swap(&queue->tail, tail, newNode);
                    atomic_fetch_add(&queue->size, 1);
                    return;
                }
            } else {
                __sync_bool_compare_and_swap(&queue->tail, tail, next);
            }
        }
    }
}

// Dequeue operation (lock-free)
int dequeue(Queue *queue) {
    Node *head;
    while (1) {
        head = queue->head;
        Node *tail = queue->tail;
        Node *next = head->next;
        if (head == queue->head) {
            if (head == tail) {
                if (next == NULL) {
                    return -1; // Queue is empty
                }
                __sync_bool_compare_and_swap(&queue->tail, tail, next);
            } else {
                int value = next->value;
                if (__sync_bool_compare_and_swap(&queue->head, head, next)) {
                    atomic_fetch_sub(&queue->size, 1);
                    free(head);
                    return value;
                }
            }
        }
    }
}

// Structure to hold the prime counting state
typedef struct {
    Queue *queue;
    atomic_int *total_counter;
    atomic_bool *done;
} PrimeCounterState;

// Worker thread function to count primes
void* primeCounterWorker(void *arg) {
    PrimeCounterState *state = (PrimeCounterState*)arg;
    int num;

    while (!atomic_load(state->done) || atomic_load(&state->queue->size) > 0) {
        num = dequeue(state->queue);
        if (num == -1) {
            usleep(100); // Sleep briefly to avoid busy-waiting
            continue;
        }
        if (isPrime(num)) {
            atomic_fetch_add(state->total_counter, 1);
        }
    }
    return NULL;
}

// Function to print resource usage
void printResourceUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("CPU time: User = %ld.%06ld, System = %ld.%06ld\n",
        usage.ru_utime.tv_sec, (long)usage.ru_utime.tv_usec,
        usage.ru_stime.tv_sec, (long)usage.ru_stime.tv_usec);
    printf("Max resident set size: %ld KB\n", usage.ru_maxrss);
    printf("Page faults: %ld (I/O: %ld)\n",
        usage.ru_majflt, usage.ru_minflt);
    printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
    printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
}

int main() {
    Queue *queue = createQueue();
    atomic_int total_counter = 0;
    atomic_bool done = false;

    // Set up state for worker threads
    PrimeCounterState state = {queue, &total_counter, &done};

    // Determine the number of CPU cores
    long numCPU = sysconf(_SC_NPROCESSORS_ONLN);
    if (numCPU < 1) {
        numCPU = 1; // Fallback to at least one thread if detection fails
    }

    printf("Detected %ld CPU cores.\n", numCPU);
    
    // Create worker threads based on the number of CPU cores
    pthread_t threads[numCPU];
    for (long i = 0; i < numCPU; i++) {
        pthread_create(&threads[i], NULL, primeCounterWorker, &state);
    }

    int num;
    int total_numbers = 0;
    while (scanf("%d", &num) != EOF) {
        while (atomic_load(&queue->size) >= MAX_QUEUE_SIZE) {
            usleep(100); // Wait if the queue is full
        }
        enqueue(queue, num);
        total_numbers++;
    }

    // Signal to threads that processing is done
    atomic_store(&done, true);

    // Wait for all threads to finish
    for (long i = 0; i < numCPU; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Processed %d numbers.\n", total_numbers);
    printf("%d total primes.\n", atomic_load(&total_counter));

    // Print resource usage
    printResourceUsage();

    // Clean up
    free(queue->head);
    free(queue);

    return 0;
}
