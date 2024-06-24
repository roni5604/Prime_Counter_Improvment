.PHONY: all
all: generator primeCounter new_primeCounter

generator: generator.c
	gcc -o randomGenerator generator.c

primeCounter: primeCounter.c
	gcc -o primeCounter primeCounter.c

new_primeCounter: new_primeCounter.c lock_free_queue.h
	gcc -o new_primeCounter new_primeCounter.c -pthread

.PHONY: clean
clean:
	rm -f randomGenerator primeCounter new_primeCounter
