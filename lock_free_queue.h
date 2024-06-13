#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <stdlib.h>
#include <stdatomic.h>

typedef struct Node {
    int value;
    _Atomic(struct Node *) next;  // _Atomic type for next pointer
} Node;

typedef struct LockFreeQueue {
    _Atomic(Node *) head;  // _Atomic type for head pointer
    _Atomic(Node *) tail;  // _Atomic type for tail pointer
} LockFreeQueue;

void initQueue(LockFreeQueue *queue) {
    Node *dummy = (Node *)malloc(sizeof(Node));
    dummy->next = NULL;
    atomic_store(&queue->head, dummy);
    atomic_store(&queue->tail, dummy);
}

void enqueue(LockFreeQueue *queue, int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;

    Node *tail;
    Node *next;

    while (1) {
        tail = atomic_load(&queue->tail);
        next = atomic_load(&tail->next);

        if (tail == atomic_load(&queue->tail)) {
            if (next == NULL) {
                if (atomic_compare_exchange_weak(&tail->next, &next, newNode)) {
                    atomic_compare_exchange_weak(&queue->tail, &tail, newNode);
                    return;
                }
            } else {
                atomic_compare_exchange_weak(&queue->tail, &tail, next);
            }
        }
    }
}

int dequeue(LockFreeQueue *queue, int *value) {
    Node *head;
    Node *tail;
    Node *next;

    while (1) {
        head = atomic_load(&queue->head);
        tail = atomic_load(&queue->tail);
        next = atomic_load(&head->next);

        if (head == atomic_load(&queue->head)) {
            if (head == tail) {
                if (next == NULL) {
                    return 0; // Queue is empty
                }
                atomic_compare_exchange_weak(&queue->tail, &tail, next);
            } else {
                *value = next->value;
                if (atomic_compare_exchange_weak(&queue->head, &head, next)) {
                    free(head);
                    return 1; // Dequeue successful
                }
            }
        }
    }
}

void destroyQueue(LockFreeQueue *queue) {
    int dummy;
    while (dequeue(queue, &dummy));
    free(atomic_load(&queue->head));
}

#endif
