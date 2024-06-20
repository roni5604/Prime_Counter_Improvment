#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 512 // Adjusted to ensure we stay within 2MB limit with overhead
#define NUM_THREADS 4

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

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
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
            atomic_fetch_add(state->total_counter, 1);// Increment the total counter
        }
    }
    return NULL;
}

int main() {
    Queue *queue = createQueue();
    atomic_int total_counter = 0;
    atomic_bool done = false;

    // Set up state for worker threads
    PrimeCounterState state = {queue, &total_counter, &done};

    // Create worker threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
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
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", atomic_load(&total_counter));

    // Clean up
    free(queue->head);
    free(queue);

    return 0;
}
