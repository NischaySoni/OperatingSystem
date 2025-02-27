#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROTATIONAL_SPEED_RPM 72    
#define SEEK_TIME_PER_TRACK_MS 2
#define SECTORS_PER_TRACK 100  
#define MAX_TRACK 5                

double rotationalLatency(int currentSector, int targetSector) {
    int sector_difference = abs(currentSector - targetSector);
    double time_per_sector = (6.0/ ROTATIONAL_SPEED_RPM) / (SECTORS_PER_TRACK + 1); 
    return sector_difference * time_per_sector * 1000;
}

int Track(int sector) {
    return sector / (SECTORS_PER_TRACK+1);
}

int main() {
    int n; 
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    int *requests = (int *)malloc(n * sizeof(int)); 
    if (requests == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter the disk requests (sector positions):\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d", &requests[i]);
    }

    int initial_position;
    printf("Enter the initial position of the disk head: ");
    scanf("%d", &initial_position);

    int currentSector = initial_position;
    int currentTrack = Track(currentSector);

    double totalSeekTime = 0;
    double totalRotationalLatency = 0;

    printf("\nDisk Scheduling Results:\n");
    for (int i = 0; i < n; i++) {
        int targetSector = requests[i];
        int targetTrack = Track(targetSector);

        int trackDistance = abs(targetTrack - currentTrack);
        double seek_time = trackDistance * SEEK_TIME_PER_TRACK_MS;
        double rotational_latency = rotationalLatency(currentSector, targetSector);

        totalSeekTime += seek_time;
        totalRotationalLatency += rotational_latency;

        printf("Request %d:\n", i + 1);
        printf("  Seek Time: %.2f ms\n", seek_time);
        printf("  Rotational Latency: %.2f ms\n", rotational_latency);

        currentSector = targetSector;
        currentTrack = targetTrack;
    }

    printf("\nTotal Seek Time: %.2f ms\n", totalSeekTime);
    printf("Total Rotational Latency: %.2f ms\n", totalRotationalLatency);

    free(requests); 
    return 0;
}  
