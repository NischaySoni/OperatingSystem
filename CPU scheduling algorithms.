#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct Algorithm{
	int PID;
	int arrivalTime;
	int burstTime;
	int completionTime;
	int waitingTime;
	int turnAroundTime;
	int responseTime;
	int rem;
	int start;
	int isDone;
} Algorithm;

typedef struct Queue{
	int front;
	int rear;
	int size;
	unsigned capacity;
	Algorithm *array;
} Queue;

Queue* createQueue(unsigned capacity){
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;
	queue->rear = capacity - 1;
	queue->array = (Algorithm*)malloc(queue->capacity * sizeof(Algorithm));
	return queue;
}

int isFull(Queue* queue){
	return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue){
	return (queue->size == 0);
}

void enqueue(Queue* queue, Algorithm item){
	if (isFull(queue)){
		return;
	}
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size+1;
}

Algorithm dequeue(Queue* queue){
	if (isEmpty(queue)){
		Algorithm dummy = {0};
		return dummy;
	}
	Algorithm item = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

Algorithm front(Queue* queue) {
    	if (isEmpty(queue)) {
        	Algorithm dummy = {0};
        	return dummy;
    	}	
    	return queue->array[queue->front];
}

int compareArrival(const void *a, const void *b){
	struct Algorithm *s = (struct Algorithm*)a;
        struct Algorithm *t = (struct Algorithm*)b;
        return s->arrivalTime - t->arrivalTime;
}

int compareBurst(const void *a, const void *b){
	struct Algorithm *s = (struct Algorithm*)a;
	struct Algorithm *t = (struct Algorithm*)b;
	return s->burstTime - t->burstTime;
}

void fifo(Algorithm arr[], int n){
	int totalResponse = 0;
	int totalTurnAround = 0;
	qsort(arr, n, sizeof(struct Algorithm), compareArrival);
	for (int i = 0; i<n; i++){
		if (i == 0){
			arr[i].completionTime = arr[i].burstTime;
		}
		else {
			arr[i].completionTime = arr[i].burstTime + arr[i-1].completionTime;
		}
		arr[i].turnAroundTime = arr[i].completionTime - arr[i].arrivalTime;
		arr[i].waitingTime = arr[i].turnAroundTime - arr[i].burstTime;
		arr[i].responseTime = arr[i].waitingTime;

		totalTurnAround += arr[i].turnAroundTime;
		totalResponse += arr[i].responseTime;
	}
	printf("FIFO Scheduling: \n");
	printf("PID\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\tResponse Time\n");
	for (int i = 0; i<n; i++){
		printf("%d\t\t%d\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%d\n",
				arr[i].PID,
				arr[i].arrivalTime,
				arr[i].burstTime,
				arr[i].completionTime,
				arr[i].turnAroundTime,
				arr[i].waitingTime,
				arr[i].responseTime
		      );
	}
	printf("Average Response Time: %.2f\n", (float)totalResponse/n);
	printf("Average Turn Around Time: %.2f\n", (float)totalTurnAround/n);
}

void sjf(Algorithm arr[], int n){
	for (int i = 0; i<n; i++){
		arr[i].rem = arr[i].burstTime;
		arr[i].start = -1;
	}
	int totalResponse = 0;
	int totalTurnAround = 0;
	int curr = 0;
	int completed = 0;
	qsort(arr, n, sizeof(struct Algorithm), compareArrival);
	while (completed < n){
		int idx = -1;
		int BT = INT_MAX;
		for (int i = 0; i < n; i++){
			if (arr[i].arrivalTime <= curr && arr[i].rem > 0){
				if (arr[i].burstTime < BT){
					BT = arr[i].burstTime;
					idx = i;
				}
			}
		}
		if (idx == -1){
			curr++;
			continue;
		}
		if (arr[idx].start == -1){
			arr[idx].start = curr;
		}
		curr += arr[idx].rem;
		arr[idx].completionTime = curr;
		arr[idx].turnAroundTime = arr[idx].completionTime - arr[idx].arrivalTime;
		arr[idx].waitingTime = arr[idx].turnAroundTime - arr[idx].burstTime;
		arr[idx].responseTime = arr[idx].start - arr[idx].arrivalTime;
		totalTurnAround += arr[idx].turnAroundTime;
		totalResponse += arr[idx].responseTime;
		arr[idx].rem = 0;
		completed++;
	}
        printf("SJF Scheduling: \n");
        printf("PID\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\tResponse Time\n");
        for (int i = 0; i<n; i++){
                printf("%d\t\t%d\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%d\n",
                                arr[i].PID,
                                arr[i].arrivalTime,
                                arr[i].burstTime,
                                arr[i].completionTime,
                                arr[i].turnAroundTime,
                                arr[i].waitingTime,
                                arr[i].responseTime
                      );
        }
        printf("Average Response Time: %.2f\n", (float)totalResponse/n);
        printf("Average Turn Around Time: %.2f\n", (float)totalTurnAround/n);

}

void srtf(Algorithm arr[], int n){
	int totalTurnAround = 0;
	int totalResponse = 0;
	int count = 0;
	for (int i = 0; i<n; i++){
		arr[i].rem = arr[i].burstTime;
		arr[i].isDone = arr[i].burstTime;
		arr[i].start = -1;
	}
	arr[100].rem = INT_MAX;
	for (int j = 0; count != n; j++){
		int mini = 100;
		for (int i = 0; i < n; i++){
			if (arr[i].arrivalTime <= j
				&& arr[i].rem < arr[mini].rem
			       	&& arr[i].rem > 0){
				mini = i;
			}
		}
		if (arr[mini].start == -1){
			arr[mini].start = j;
			arr[mini].responseTime = arr[mini].start - arr[mini].arrivalTime;
			totalResponse += arr[mini].responseTime;
		}
		arr[mini].rem--;
		if (arr[mini].rem == 0){
			count++;
			int end = j + 1;
			arr[mini].completionTime = end;
			arr[mini].waitingTime = end - arr[mini].arrivalTime - arr[mini].isDone;
			arr[mini].turnAroundTime = end - arr[mini].arrivalTime;
			totalTurnAround += arr[mini].turnAroundTime;
		}
	}
	printf("SRTF Scheduling: \n");
        printf("PID\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\tResponse Time\n");
        for (int i = 0; i<n; i++){
                printf("%d\t\t%d\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%d\n",
                                arr[i].PID,
                                arr[i].arrivalTime,
                                arr[i].burstTime,
                                arr[i].completionTime,
                                arr[i].turnAroundTime,
                                arr[i].waitingTime,
                                arr[i].responseTime
                      );
        }
        printf("Average Response Time: %.2f\n", (float)totalResponse/n);
        printf("Average Turn Around Time: %.2f\n", (float)totalTurnAround/n);
}

void rr(Algorithm arr[], int n, int timeQuantam){
	int sum = 0;
	int count = 0;
	int totalResponse = 0;
	int totalTurnAround = 0;
	int time = 0;
	for (int i = 0; i < n; i++){
		arr[i].rem = arr[i].burstTime;
		arr[i].start = -1;
		arr[i].isDone = 0;
	}
	Queue* readyQueue = createQueue(100);
	int completed = 0;
	int idx = 0;
	while (completed < n){
		while (idx < n && arr[idx].arrivalTime <= time){
			enqueue(readyQueue, arr[idx]);
			idx++;
		}
		if (isEmpty(readyQueue)){
			time++;
			continue;
		}
		Algorithm curr = dequeue(readyQueue);
		if (curr.rem == curr.burstTime){
			curr.start = time;
		}
		int exec = (curr.rem <= timeQuantam) ? curr.rem : timeQuantam;
		time += exec;
		curr.rem -= exec;
		if (curr.rem == 0){
			completed++;
			curr.completionTime = time;
			curr.turnAroundTime = curr.completionTime - curr.arrivalTime;
			curr.waitingTime = curr.turnAroundTime - curr.burstTime;
			curr.responseTime = curr.start - curr.arrivalTime;
			totalResponse += curr.responseTime;
			totalTurnAround += curr.turnAroundTime;
		}
		else{
			enqueue(readyQueue, curr);
		}
	}
	/*while (count < n){
		int executed = 0;
		for (int i = 0; i < n; i++){
			if (arr[i].arrivalTime <= sum && arr[i].rem > 0){
				if (arr[i].start == -1){
					arr[i].start = sum;
				}
				if (arr[i].rem <= timeQuantam){
					sum += arr[i].rem;
					arr[i].completionTime = sum;
					arr[i].turnAroundTime = arr[i].completionTime - arr[i].arrivalTime;
					arr[i].waitingTime = arr[i].turnAroundTime - arr[i].burstTime;
					totalResponse += arr[i].start - arr[i].arrivalTime;
					totalTurnAround += arr[i].turnAroundTime;
					arr[i].rem = 0;
					count++;
				}
				else {
					arr[i].rem -= timeQuantam;
					sum += timeQuantam;
				}
				executed = 1;
			}
		}
		if (!executed){
			sum++;
		}
	}*/
	printf("RR Scheduling: \n");
        printf("PID\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\tResponse Time\n");
        for (int i = 0; i<n; i++){
                printf("%d\t\t%d\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%d\n",
                                arr[i].PID,
                                arr[i].arrivalTime,
                                arr[i].burstTime,
                                arr[i].completionTime,
                                arr[i].turnAroundTime,
                                arr[i].waitingTime,
                                arr[i].responseTime
                      );
        }
        printf("Average Response Time: %.2f\n", (float)totalResponse/n);
        printf("Average Turn Around Time: %.2f\n", (float)totalTurnAround/n);

	free(readyQueue->array);
	free(readyQueue);
}

int main(){
	int n;
	printf("Enter the number of Process: ");
	scanf("%d", &n);
	printf("Enter the details for each processes\n");
	Algorithm arr[n];

	for (int i = 0; i<n; i++){
		arr[i].PID = i+1;
		printf("Enter arrival time and burst time for process %d: ", i+1);
		scanf("%d %d", &arr[i].arrivalTime, &arr[i].burstTime);

		arr[i].completionTime = 0;
		arr[i].waitingTime = 0;
		arr[i].turnAroundTime = 0;
		arr[i].responseTime = 0;
	}

	fifo(arr, n);
	sjf(arr, n);
	srtf(arr, n);

	int timeQuantam;
	printf("Enter timeQauntam for round robin scheduling: ");
	scanf("%d", &timeQuantam);
	rr(arr, n, timeQuantam);

	return 0;
}
