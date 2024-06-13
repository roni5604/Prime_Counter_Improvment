.PHONY: all
all: generator primeCounter

generator: generator.c
	gcc -o randomGenerator generator.c

primeCounter: primeCounter.c lock_free_queue.h
	gcc -o primeCounter primeCounter.c

.PHONY: clean
clean:
	rm -f randomGenerator primeCounter
