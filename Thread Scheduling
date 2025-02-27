#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Lock A and Lock B
pthread_mutex_t lockA;
pthread_mutex_t lockB;

#define ACQUISITIONS 3

void* thread_func(void* arg);

void acquireLocks(int thread_id) {
    printf("T%d waiting for Lock A\n", thread_id);
    pthread_mutex_lock(&lockA);
    printf("T%d acquired Lock A\n", thread_id);

    // delay
    sleep(0.1);

    printf("T%d waiting for Lock B\n", thread_id);
    pthread_mutex_lock(&lockB);
    printf("T%d acquired Lock B\n", thread_id);
}

void reverseLocks(int thread_id) {
    printf("T%d releasing Lock B\n", thread_id);
    pthread_mutex_unlock(&lockB);
    printf("T%d releasing Lock A\n", thread_id);
    pthread_mutex_unlock(&lockA);
}

void* thread_func(void* arg) {
    int thread_id = *((int*)arg);
    for (int i = 1; i <= ACQUISITIONS; i++) {
        acquireLocks(thread_id);
   //     printf("T%d working with both locks\n", thread_id);
        sleep(1); 
        reverseLocks(thread_id);
   //     printf("T%d completed work and released all locks\n", thread_id);
        sleep(1);
   }

    printf("T%d finished all lock acquisitions\n", thread_id);
    return NULL;
}

int main() {
    
    pthread_mutex_init(&lockA, NULL);
    pthread_mutex_init(&lockB, NULL);

    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};

    // creating 3 threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    // waiting for threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lockA);
    pthread_mutex_destroy(&lockB);

    printf("All threads finished execution without deadlock\n");
    return 0;
}
