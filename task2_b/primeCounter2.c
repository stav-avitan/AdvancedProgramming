#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

#define NUM_OF_THREADS 4

// Shared counter array
int counters[NUM_OF_THREADS] = {0};

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    int limit = (int)sqrt(n);
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
