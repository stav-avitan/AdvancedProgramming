.PHONY: all
all: generator primeCounter arcadiCounter

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter2.c
	gcc -pthread -o primeCounter primeCounter2.c -lm

arcadiCounter: arcadiCounter.c
	gcc -pthread -o arcadiCounter arcadiCounter.c -lm

.PHONY: clean
clean:
	-rm randomGenerator primeCounter 2>/dev/null
