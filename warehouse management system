#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>

#define BUFFER_CAPACITY 10           
#define MAX_PRODUCTS_DELIVERY 5     
#define MAX_PRODUCTS_STORAGE 5
#define MAX_DELIVERIES 20
#define RUN_TIME 20

int buffer[BUFFER_CAPACITY];
int in = 0;   // delivery truck for adding products
int out = 0;  // storage manager for removing products
int current_inventory = 0; 
int total_deliveries = 0;
atomic_int is_running = 1;

sem_t empty_slots;          
sem_t filled_slots;       
pthread_mutex_t buffer_mutex; 
pthread_mutex_t delivery_mutex;

void* deliveryTruck(void* arg) {
    int id = *((int*)arg);
    free(arg); 

    while (atomic_load(&is_running)) {
        int products = rand() % MAX_PRODUCTS_DELIVERY + 1;
        printf("Truck %d delivering %d products\n", id, products);

        for (int i = 0; i < products; i++) {
            if (sem_trywait(&empty_slots) != 0) {
                printf("Truck %d waiting to add products [buffer state]\n", id);
                sem_wait(&empty_slots); 
            }
        }

        pthread_mutex_lock(&buffer_mutex);

        for (int i = 0; i < products; i++) {
            if (!atomic_load(&is_running)) break;
            buffer[in] = 1;  
            in = (in + 1) % BUFFER_CAPACITY;
            current_inventory++;
        }
        printf("Truck %d delivered %d products.\n Current inventory status: %d/%d\n",
               id, products, current_inventory, BUFFER_CAPACITY);

        pthread_mutex_unlock(&buffer_mutex);
        
        pthread_mutex_lock(&delivery_mutex);
        total_deliveries++;
        if (total_deliveries >= MAX_DELIVERIES) {
            atomic_store(&is_running, 0);
        }
        pthread_mutex_unlock(&delivery_mutex);

        // Signal
        for (int i = 0; i < products; i++) {
            sem_post(&filled_slots);
        }

        sleep(rand() % 3 + 1); 
    }
    return NULL;
}

void* storageManager(void* arg) {
    int id = *((int*)arg);
    free(arg); 

    while (atomic_load(&is_running)) {
        int products = rand() % MAX_PRODUCTS_STORAGE + 1;
        printf("Manager %d storing %d products\n", id, products);

        for (int i = 0; i < products; i++) {
            if (sem_trywait(&filled_slots) != 0) {
                printf("Manager %d waiting to take products [buffer state]\n", id);
                sem_wait(&filled_slots);
            }
        }

        pthread_mutex_lock(&buffer_mutex);

        for (int i = 0; i < products; i++) {
            if (!atomic_load(&is_running)) break;
            buffer[out] = 0; 
            out = (out + 1) % BUFFER_CAPACITY;
            current_inventory--;
        }
        printf("Manager %d stored %d products.\n Current inventory status: %d/%d\n",
               id, products, current_inventory, BUFFER_CAPACITY);

        pthread_mutex_unlock(&buffer_mutex);

        // Signal
        for (int i = 0; i < products; i++) {
            sem_post(&empty_slots);
        }

        sleep(rand() % 3 + 1);
    }
    return NULL;
}

int main() {
    int num_trucks, num_managers;

    printf("Enter no. of trucks: ");
    scanf("%d", &num_trucks);
    printf("Enter no. of storage managers: ");
    scanf("%d", &num_managers);

    pthread_t trucks[num_trucks];
    pthread_t managers[num_managers];

    srand(time(NULL));

    sem_init(&empty_slots, 0, BUFFER_CAPACITY); 
    sem_init(&filled_slots, 0, 0);  

    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_mutex_init(&delivery_mutex, NULL);
    
    time_t startTime = time(NULL);

    for (int i = 0; i < num_trucks; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&trucks[i], NULL, deliveryTruck, id);
    }

    for (int i = 0; i < num_managers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&managers[i], NULL, storageManager, id);
    }
    
    // Run until timeout or maximum deliveries
    while (atomic_load(&is_running)) {
        if (difftime(time(NULL), startTime) >= RUN_TIME) {
            atomic_store(&is_running, 0);
        }
        sleep(0.1);
    }

    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        sem_post(&empty_slots);
        sem_post(&filled_slots);
    }
    
    for (int i = 0; i < num_trucks; i++) {
        pthread_join(trucks[i], NULL);
    }
    for (int i = 0; i < num_managers; i++) {
        pthread_join(managers[i], NULL);
    }

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    pthread_mutex_destroy(&buffer_mutex);
    pthread_mutex_destroy(&delivery_mutex);

    printf("Program terminated. Final buffer state: %d/%d products.\n", current_inventory, BUFFER_CAPACITY);

    return 0;
}
