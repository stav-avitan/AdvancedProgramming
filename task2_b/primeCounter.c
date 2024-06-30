#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#define NUM_OF_THREADS 4

// Shared counter array
int counters[NUM_OF_THREADS] = {0};

bool isMillerRabinPrime(int n, int d) {
    if (n == 2 || n == 3) return true;
    if (n <= 1 || n % 2 == 0) return false;

    // Find r and d such that n-1 = d * 2^r
    int r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    // Miller-Rabin test for base 2
    int a = 2;
    int x = (int)pow(a, d) % n;
    if (x == 1 || x == n - 1) return true;

    for (int i = 0; i < r - 1; i++) {
        x = (x * x) % n;
        if (x == n - 1) return true;
    }

    return false;
}

bool isPrime(int n) {
    // Perform Miller-Rabin test
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    int d = n - 1;
    return isMillerRabinPrime(n, d);
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
