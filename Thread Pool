#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    void (*callback)(void*); 
    int m, p, n;         
    int **A, **B, **C;      
} Task;

typedef struct {
    pthread_t* threads;      
    Task* task_queue;        
    int size;          
    int capacity;     
    int flag;            
    pthread_mutex_t lock;    
    pthread_cond_t signal; 
} ThreadPool;

void* thread_function(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        while (pool->size == 0 && !pool->flag) {
            pthread_cond_wait(&pool->signal, &pool->lock);
        }
        if (pool->flag) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }
        Task task = pool->task_queue[--pool->size];
        pthread_mutex_unlock(&pool->lock);
        task.callback(&task);
    }
}

void compute_element(void* arg) {
    Task* task = (Task*)arg;
    int sum = 0;
    for (int k = 0; k < task->n; k++) {
        sum += task->A[task->m][k] * task->B[k][task->p];
    }
    task->C[task->m][task->p] = sum;
}

ThreadPool* thread_pool_init(int num_threads, int capacity) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    pool->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    pool->task_queue = (Task*)malloc(capacity * sizeof(Task));
    pool->size = 0;
    pool->capacity = capacity;
    pool->flag = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->signal, NULL);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, (void* (*)(void*))thread_function, pool);
    }
    return pool;
}

void thread_pool_add(ThreadPool* pool, Task task) {
    pthread_mutex_lock(&pool->lock);
    pool->task_queue[pool->size++] = task;
    pthread_cond_signal(&pool->signal);
    pthread_mutex_unlock(&pool->lock);
}

void thread_pool_destroy(ThreadPool* pool, int num_threads) {
    pthread_mutex_lock(&pool->lock);
    pool->flag = 1;
    pthread_cond_broadcast(&pool->signal);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->task_queue);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->signal);
    free(pool);
}

void sequential_matrix(int** A, int** B, int** C, int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_matrix(int** mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

double get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

int main() {
    int m, n, n_B, p;
    int **A, **B, **C_sequential, **C_thread_pool;

    printf("Enter dimensions of matrix A (m n): ");
    scanf("%d %d", &m, &n);
    printf("Enter dimensions of matrix B (n p): ");
    scanf("%d %d", &n_B, &p);

    if (n != n_B) {
        printf("Invalid matrix dimensions for multiplication.\n");
        return -1;
    }

    A = (int**)malloc(m * sizeof(int*));
    B = (int**)malloc(n * sizeof(int*));
    C_sequential = (int**)malloc(m * sizeof(int*));
    C_thread_pool = (int**)malloc(m * sizeof(int*));
    for (int i = 0; i < m; i++) {
        A[i] = (int*)malloc(n * sizeof(int));
        C_sequential[i] = (int*)malloc(p * sizeof(int));
        C_thread_pool[i] = (int*)malloc(p * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        B[i] = (int*)malloc(p * sizeof(int));
    }

    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }
    printf("Enter elements of matrix B:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    // Sequential multiplication
    double start_seq = get_time();
    sequential_matrix(A, B, C_sequential, m, n, p);
    double end_seq = get_time();

    // Thread pool multiplication
    int num_threads = 4; 
    ThreadPool* pool = thread_pool_init(num_threads, m * p);

    double start_thread_pool = get_time();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            Task task = {compute_element, i, j, n, A, B, C_thread_pool};
            thread_pool_add(pool, task);
        }
    }
    thread_pool_destroy(pool, num_threads);
    double end_thread_pool = get_time();

    printf("Resultant matrix (Sequential):\n");
    print_matrix(C_sequential, m, p);

    printf("Resultant matrix (Thread Pool):\n");
    print_matrix(C_sequential, m, p);

    printf("Time for sequential execution: %.6f seconds\n", end_seq - start_seq);
    printf("Time for thread pool execution: %.6f seconds\n", end_thread_pool - start_thread_pool);
    printf("Speed-up: %.6f\n", (end_seq - start_seq) / (end_thread_pool - start_thread_pool));

    for (int i = 0; i < m; i++) {
        free(A[i]);
        free(C_sequential[i]);
        free(C_thread_pool[i]);
    }
    for (int i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C_sequential);
    free(C_thread_pool);

    return 0;
}
