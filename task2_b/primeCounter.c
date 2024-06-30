#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#define NUM_OF_THREADS 4

// Shared counter array
int counters[NUM_OF_THREADS] = {0};
#define MAX_PRIMES 10000
#define SIEVE_LIMIT 104729  // 104729 is the 10000th prime number

int primes[MAX_PRIMES];
int prime_count = 0;

void generatePrimes() {
    bool* is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));

    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        is_prime[i] = true;
    }

    for (int i = 2; i * i <= SIEVE_LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    for (int i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIMES; i++) {
        if (is_prime[i]) {
            primes[prime_count++] = i;
        }
    }

    free(is_prime);
}

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= SIEVE_LIMIT) {
        for (int i = 0; i < prime_count; i++) {
            if (primes[i] == n) return true;
            if (n % primes[i] == 0) return false;
        }
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) return false;

    int limit = (int)sqrt(n);
    for (int i = 0; i < prime_count && primes[i] <= limit; i++) {
        if (n % primes[i] == 0) return false;
    }

    for (int i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}


// Thread function to count primes
void *work2(void *arg) {
    int index = *(int *)arg; // Thread-specific counter index
    int n;

    while (scanf("%d", &n) != EOF) {
        if (isPrime(n)) {
            counters[index]++;
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_OF_THREADS];
    int thread_ids[NUM_OF_THREADS];

    // Create threads
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, work2, (void *)&thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sum up the counters from all threads
    int sum = 0;
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        sum += counters[i];
    }

    printf("%d total primes.\n", sum);

    return 0;
}
