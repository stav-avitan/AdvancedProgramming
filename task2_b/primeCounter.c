#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "Queue.c"

#define NUM_OF_THREADS 4
#define QUEUE_CAPACITY 75

static int total_counter = 0;
pthread_mutex_t locks[NUM_OF_THREADS]; // Array of mutexes, one for each queue
bool END = false; // Flag to indicate input completion
pthread_t threads[NUM_OF_THREADS];
struct Queue* queues[NUM_OF_THREADS];

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true; 
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false; 
    } 

    // Check for larger primes using the 6k ± 1 optimization
    int limit = (int)sqrt(n); // Calculate the square root of n once
    for (int i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}
void *work(void* q) {
    struct Queue* queue = (struct Queue*)q;
    int queue_index = queue->index; 
    while (1) {
        pthread_mutex_lock(&locks[queue_index]); // Lock the specific mutex for this queue
        if (isEmpty(queue)) {
            pthread_mutex_unlock(&locks[queue_index]); // Unlock before checking input completion flag
            if (END) { 
                break; 
            }
            usleep(100); // avoid busy-waiting
            continue;
        }
        int num = front(queue);
        dequeue(queue);
        pthread_mutex_unlock(&locks[queue_index]); // Unlock after dequeueing

        if (isPrime(num)) {
            __sync_fetch_and_add(&total_counter, 1); // Atomically increment total_counter
           
        }
    }
    return NULL;
}


int main() {
// ---------------- arcadi's program-----------------
    // int num;
    // int total_counter = 0;

    // // Read numbers from stdin until end of file
    // while (scanf("%d", &num) != EOF) {   
    //     if (isPrime(num)) {
    //         total_counter++;
    //     }
    // }
    
    // printf("%d total primes.\n", total_counter);



    //------- my program-------
    int num;


    // Create queues and start threads
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        queues[i] = createQueue(QUEUE_CAPACITY, i); // Create queue
        pthread_mutex_init(&locks[i], NULL); // Initialize the mutex for this queue
        pthread_create(&threads[i], NULL, work, (void *)queues[i]);
    }

    // Read numbers from stdin until end of file
    while (scanf("%d", &num) != EOF) {
        bool enqueued = false;
        while (!enqueued) {
            for (int i = 0; i < NUM_OF_THREADS; i++) {
                pthread_mutex_lock(&locks[i]); // Lock the mutex for this queue
                if (!isFull(queues[i])) {
                    enqueue(queues[i], num);
                    enqueued = true;
                    pthread_mutex_unlock(&locks[i]); // Unlock after enqueueing
                    break; 
                }
                pthread_mutex_unlock(&locks[i]); // Unlock if queue is full
            }
            if (!enqueued) {
                usleep(100); // avoid busy-waiting
            }
        }
    }
    END = true;

    // Wait for all threads to finish
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&locks[i]); // Destroy the mutex for this queue
    }

    printf("%d total primes.\n", total_counter);

    return 0;
}
