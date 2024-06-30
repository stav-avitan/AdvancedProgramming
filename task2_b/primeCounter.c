#include "producer.h"
#include "worker.h"

Queue_Task *queue = NULL;
bool producer_finished[NUM_PRODUCERS] = {false};
int *prime_count; // global variable for sieve algorithm
int *prime; // global variable for sieve algorithm

pthread_mutex_t producer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t producer_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {
    
    int count = 0;
    prime_count = &count;
    prime = manipulated_sieve(SIEVE_NUM);
    if(!prime){
        fprintf(stderr, "Failed to execute manipulated_sieve\n");
        return 1;
    }
    // Initialize variables
    int total_primes = 0;
    queue = create_Queue();
    pthread_t producer_thread[NUM_PRODUCERS];
    pthread_t worker_threads[NUM_THREADS];
    int prime_counters[NUM_THREADS] = {0};

    for(int i = 0; i < NUM_THREADS; i++) {
        Worker *worker_args = create_worker(&prime_counters[i], queue);
        if(pthread_create(&worker_threads[i], NULL, worker, (void*) worker_args)){
            fprintf(stderr, "Error creating worker thread\n");
            free_worker(worker_args);
            return 1;
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        Producer *producer_args = create_producer(&producer_finished[i], queue);
        if(pthread_create(&producer_thread[i], NULL, producer, (void*) producer_args)){
            fprintf(stderr, "Error creating producer thread\n");
            free_producer(producer_args);
            return 1;
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producer_thread[i], NULL);
    }
    
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(worker_threads[i], NULL);
    }
    
    destroy_Queue(queue);
       
    // Sum prime_counters
    for(int i = 0; i < NUM_THREADS; i++){ 
        total_primes += prime_counters[i];
    }

    free(prime);

    printf("%d total primes.\n", total_primes);
    return 0;
}
