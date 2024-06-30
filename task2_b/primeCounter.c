#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#define THREADS_NUMBER 4

long long myMod(long long base, long long exp, long long mod) {
    long long myRes = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) { 
            myRes = (myRes * base) % mod;
        }
        exp >>= 1; 
        base = (base * base) % mod; 
    }
    return myRes;
}

bool MillerRabinTest(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    long long d = n - 1;
    int r = 0;

    while (!(d & 1)) { // Equivalent to d % 2 == 0
        d >>= 1; // Equivalent to d /= 2
        r++;
    }

}

bool prime(int n) {
    return MillerRabinTest(n);
}


void *working(void *arg) {
    int index = *(int *)arg; // Thread-specific counter index
    int n;

    while (scanf("%d", &n) != EOF) {
        if (prime(n)) {
            counters[index]++;
        }
    }

    return NULL;
}

   long long bases[] = {3, 11, 37};
int num_bases = sizeof(bases) / sizeof(bases[0]);

for (int i = 0; i < num_bases; ++i) {
    if (bases[i] >= n) break;

    long long a = bases[i];
    long long x = myMod(a, d, n); // calculate x = a ^ d mod n

    if (x == 1 || x == n - 1) continue;

    bool prime = false;
    for (int j = 0; j < r; ++j) {
        x = (x * x) % n;
        if (x == n - 1) {
            prime = true;
            break;
        }
    }

    if (!prime) return false;  // n is composite
}

return true;  



int main() {
    pthread_t threads[NUM_OF_THREADS];
    int thread_ids[NUM_OF_THREADS];

    // Create threads
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, work2, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    int total_primes = 0;
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        total_primes += counters[i];
    }

    printf("%d total primes.\n", total_primes);

    return 0;
}
