#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Algorithm {
    int PID;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnAroundTime;
    int responseTime;
    int rem;
    int start;
} Algorithm;

typedef struct Queue {
    int front;
    int rear;
    int size;
    unsigned capacity;
    int *array; 
} Queue;

Queue* createQueue(unsigned capacity) {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(Queue* queue) { return (queue->size == queue->capacity); }
int isEmpty(Queue* queue) { return (queue->size == 0); }

void enqueue(Queue* queue, int item) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int compareArrival(const void *a, const void *b) {
    return ((Algorithm*)a)->arrivalTime - ((Algorithm*)b)->arrivalTime;
}

void printMetrics(Algorithm arr[], int n, const char* name, int totalResponse, int totalTurnAround) {
    printf("\n%s Scheduling: \n", name);
    printf("PID\tArrival\tBurst\tCompletion\tTurnAround\tWaiting\tResponse\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\t%d\n",
                arr[i].PID, arr[i].arrivalTime, arr[i].burstTime,
                arr[i].completionTime, arr[i].turnAroundTime,
                arr[i].waitingTime, arr[i].responseTime);
    }
    printf("Average Response Time: %.2f\n", (float)totalResponse / n);
    printf("Average Turn Around Time: %.2f\n", (float)totalTurnAround / n);
}

void fifo(Algorithm arr[], int n) {
    int totalResponse = 0, totalTurnAround = 0;
    qsort(arr, n, sizeof(Algorithm), compareArrival);
    
    int currTime = 0;
    for (int i = 0; i < n; i++) {
        if (currTime < arr[i].arrivalTime) {
            currTime = arr[i].arrivalTime;
        }
        arr[i].responseTime = currTime - arr[i].arrivalTime;
        arr[i].completionTime = currTime + arr[i].burstTime;
        arr[i].turnAroundTime = arr[i].completionTime - arr[i].arrivalTime;
        arr[i].waitingTime = arr[i].turnAroundTime - arr[i].burstTime;
        
        currTime = arr[i].completionTime;
        totalTurnAround += arr[i].turnAroundTime;
        totalResponse += arr[i].responseTime;
    }
    printMetrics(arr, n, "FIFO", totalResponse, totalTurnAround);
}

void sjf(Algorithm arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i].rem = arr[i].burstTime;
        arr[i].start = -1;
    }
    int totalResponse = 0, totalTurnAround = 0, curr = 0, completed = 0;
    qsort(arr, n, sizeof(Algorithm), compareArrival);

    while (completed < n) {
        int idx = -1;
        int min_burst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (arr[i].arrivalTime <= curr && arr[i].rem > 0) {
                if (arr[i].burstTime < min_burst) {
                    min_burst = arr[i].burstTime;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            curr++;
            continue;
        }
        arr[idx].start = curr;
        curr += arr[idx].burstTime;
        arr[idx].completionTime = curr;
        arr[idx].turnAroundTime = arr[idx].completionTime - arr[idx].arrivalTime;
        arr[idx].waitingTime = arr[idx].turnAroundTime - arr[idx].burstTime;
        arr[idx].responseTime = arr[idx].start - arr[idx].arrivalTime;
        
        totalTurnAround += arr[idx].turnAroundTime;
        totalResponse += arr[idx].responseTime;
        arr[idx].rem = 0;
        completed++;
    }
    printMetrics(arr, n, "SJF", totalResponse, totalTurnAround);
}

void srtf(Algorithm arr[], int n) {
    int totalTurnAround = 0, totalResponse = 0, count = 0, j = 0;
    for (int i = 0; i < n; i++) {
        arr[i].rem = arr[i].burstTime;
        arr[i].start = -1;
    }
    qsort(arr, n, sizeof(Algorithm), compareArrival);

    while (count != n) {
        int mini = -1;
        int min_rem = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (arr[i].arrivalTime <= j && arr[i].rem > 0 && arr[i].rem < min_rem) {
                min_rem = arr[i].rem;
                mini = i;
            }
        }
        if (mini == -1) {
            j++;
            continue;
        }
        if (arr[mini].start == -1) {
            arr[mini].start = j;
            arr[mini].responseTime = arr[mini].start - arr[mini].arrivalTime;
            totalResponse += arr[mini].responseTime;
        }
        arr[mini].rem--;
        if (arr[mini].rem == 0) {
            count++;
            arr[mini].completionTime = j + 1;
            arr[mini].turnAroundTime = arr[mini].completionTime - arr[mini].arrivalTime;
            arr[mini].waitingTime = arr[mini].turnAroundTime - arr[mini].burstTime;
            totalTurnAround += arr[mini].turnAroundTime;
        }
        j++;
    }
    printMetrics(arr, n, "SRTF", totalResponse, totalTurnAround);
}

void rr(Algorithm arr[], int n, int timeQuantum) {
    int totalResponse = 0, totalTurnAround = 0, time = 0, completed = 0, idx = 0;
    for (int i = 0; i < n; i++) {
        arr[i].rem = arr[i].burstTime;
        arr[i].start = -1;
    }
    qsort(arr, n, sizeof(Algorithm), compareArrival);
    
    Queue* readyQueue = createQueue(n + 1);
    
    while (idx < n && arr[idx].arrivalTime <= time) {
        enqueue(readyQueue, idx);
        idx++;
    }

    while (completed < n) {
        if (isEmpty(readyQueue)) {
            time = arr[idx].arrivalTime;
            while (idx < n && arr[idx].arrivalTime <= time) {
                enqueue(readyQueue, idx);
                idx++;
            }
        }

        int currIdx = dequeue(readyQueue);
        if (arr[currIdx].start == -1) {
            arr[currIdx].start = time;
            arr[currIdx].responseTime = arr[currIdx].start - arr[currIdx].arrivalTime;
            totalResponse += arr[currIdx].responseTime;
        }

        int exec = (arr[currIdx].rem <= timeQuantum) ? arr[currIdx].rem : timeQuantum;
        time += exec;
        arr[currIdx].rem -= exec;

        while (idx < n && arr[idx].arrivalTime <= time) {
            enqueue(readyQueue, idx);
            idx++;
        }

        if (arr[currIdx].rem == 0) {
            completed++;
            arr[currIdx].completionTime = time;
            arr[currIdx].turnAroundTime = arr[currIdx].completionTime - arr[currIdx].arrivalTime;
            arr[currIdx].waitingTime = arr[currIdx].turnAroundTime - arr[currIdx].burstTime;
            totalTurnAround += arr[currIdx].turnAroundTime;
        } else {
            enqueue(readyQueue, currIdx);
        }
    }
    printMetrics(arr, n, "Round Robin", totalResponse, totalTurnAround);
    free(readyQueue->array);
    free(readyQueue);
}

int main() {
    int n;
    printf("Enter the number of Processes: ");
    if(scanf("%d", &n) != 1) return 1;

    Algorithm arr[n];
    Algorithm temp[n];

    for (int i = 0; i < n; i++) {
        arr[i].PID = i + 1;
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        if(scanf("%d %d", &arr[i].arrivalTime, &arr[i].burstTime) != 2) return 1;
    }

    for(int i=0; i<n; i++) temp[i] = arr[i]; fifo(temp, n);
    for(int i=0; i<n; i++) temp[i] = arr[i]; sjf(temp, n);
    for(int i=0; i<n; i++) temp[i] = arr[i]; srtf(temp, n);

    int timeQuantum;
    printf("\nEnter time quantum for round robin scheduling: ");
    if(scanf("%d", &timeQuantum) != 1) return 1;
    
    for(int i=0; i<n; i++) temp[i] = arr[i]; rr(temp, n, timeQuantum);

    return 0;
}
