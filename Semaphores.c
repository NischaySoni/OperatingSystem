#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define SERVERS 5

// Semaphores for network channels 
sem_t channel[SERVERS];
// Tracking of each servers processed
int process_count[SERVERS] = {0}; 

void* server(void* arg)
{
    int serverID = *((int*)arg); 
    int left = serverID;  // Left network channel
    int right = (serverID + 1) % SERVERS;  // Right network channel

    while(process_count[serverID] < 3)
    {
        printf("Server %d is waiting\n", serverID);

        if (left < right) {
            sem_wait(&channel[left]);  // Lock left channel
            sem_wait(&channel[right]); // Lock right channel
        } else {
            sem_wait(&channel[right]); // Lock right channel
            sem_wait(&channel[left]);  // Lock left channel
        }

        process_count[serverID]++;
        printf("Server %d is processing\n", serverID);
        sleep(1); 

        sem_post(&channel[left]);  // Unlock left channel
        sem_post(&channel[right]); // Unlock right channel

        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t servers[SERVERS]; 
    int serverID[SERVERS];   

    // Initializing semaphores
    for (int i = 0; i < SERVERS; i++) {
        sem_init(&channel[i], 0, 1);
    }

    // Creating threads
    for (int i = 0; i < SERVERS; i++) {
        serverID[i] = i;  
        pthread_create(&servers[i], NULL, server, &serverID[i]); 
    }

    // waiting for threads
    for (int i = 0; i < SERVERS; i++) {
        pthread_join(servers[i], NULL);
    }

    // destroying semaphores
    for (int i = 0; i < SERVERS; i++) {
        sem_destroy(&channel[i]);
    }
    return 0;
}
