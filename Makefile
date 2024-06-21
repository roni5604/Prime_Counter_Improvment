.PHONY: all
all: generator primeCounter original_primeCounter

generator: generator.c
	gcc -o randomGenerator generator.c

primeCounter: primeCounter.c lock_free_queue.h
	gcc -o primeCounter primeCounter.c -pthread

original_primeCounter: original_primeCounter.c
	gcc -o original_primeCounter original_primeCounter.c

.PHONY: clean
clean:
	rm -f randomGenerator primeCounter original_primeCounter
