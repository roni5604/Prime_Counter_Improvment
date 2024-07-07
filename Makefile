.PHONY: all
all: generator primeCounter new_primeCounter

generator: generator.c
	gcc -g -o randomGenerator generator.c

primeCounter: primeCounter.c
	gcc -g -o primeCounter primeCounter.c

new_primeCounter: new_primeCounter.c
	gcc -g -o new_primeCounter new_primeCounter.c -pthread

.PHONY: clean
clean:
	rm -f randomGenerator primeCounter new_primeCounter
