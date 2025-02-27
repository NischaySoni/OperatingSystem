#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <float.h>

#define MAX_NAME_LEN 50
#define INITIAL_TABLE_SIZE 1000000

typedef struct {
    double min;
    double max;
    double sum;
    int count;
    char name[MAX_NAME_LEN];
} StationData;

typedef struct HashTable {
    StationData **table;
    size_t size;
    size_t count;
} HashTable;

unsigned int hash_function(const char *name, size_t size) {
    unsigned int hash = 5381;
    while (*name) {
        hash = ((hash << 5) + hash) + *name++;
    }
    return hash % size;
}

HashTable *create_hash_table(size_t size) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table = calloc(size, sizeof(StationData *));
    ht->size = size;
    ht->count = 0;
    return ht;
}

void free_hash_table(HashTable *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        if (ht->table[i]) free(ht->table[i]);
    }
    free(ht->table);
    free(ht);
}

void resize_hash_table(HashTable *ht) {
    size_t new_size = ht->size * 2;
    StationData **new_table = calloc(new_size, sizeof(StationData *));

    for (size_t i = 0; i < ht->size; i++) {
        if (ht->table[i]) {
            unsigned int hash = hash_function(ht->table[i]->name, new_size);
            while (new_table[hash]) {
                hash = (hash + 1) % new_size;
            }
            new_table[hash] = ht->table[i];
        }
    }
    free(ht->table);
    ht->table = new_table;
    ht->size = new_size;
}

StationData *get_or_create_station(HashTable *ht, const char *name) {
    if ((double)ht->count / ht->size > 0.7) {
        resize_hash_table(ht);
    }

    unsigned int hash = hash_function(name, ht->size);
    while (ht->table[hash]) {
        if (strcmp(ht->table[hash]->name, name) == 0) {
            return ht->table[hash];
        }
        hash = (hash + 1) % ht->size;
    }

    StationData *new_station = malloc(sizeof(StationData));
    strcpy(new_station->name, name);
    new_station->min = DBL_MAX;
    new_station->max = -DBL_MAX;
    new_station->sum = 0;
    new_station->count = 0;
    ht->table[hash] = new_station;
    ht->count++;
    return new_station;
}

void process_line(HashTable *ht, const char *line) {
    char station_name[MAX_NAME_LEN];
    double temperature;
    if (sscanf(line, "%[^;];%lf", station_name, &temperature) == 2) {
        StationData *station = get_or_create_station(ht, station_name);
        if (temperature < station->min) station->min = temperature;
        if (temperature > station->max) station->max = temperature;
        station->sum += temperature;
        station->count++;
    }
}

void print_results(HashTable *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        if (ht->table[i]) {
            StationData *station = ht->table[i];
            double mean = station->sum / station->count;
            printf("%s min=%.2f mean=%.2f max=%.2f\n",
                   station->name, station->min, mean, station->max);
        }
    }
}

void process_mmap(const char *filename, HashTable *ht) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t file_size = st.st_size;
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    madvise(file_data, file_size, MADV_SEQUENTIAL);

    char *line_start = file_data;
    char buffer[256];

    for (size_t offset = 0; offset < file_size; offset++) {
        if (file_data[offset] == '\n' || offset == file_size - 1) {
            size_t line_length = &file_data[offset] - line_start + (offset == file_size - 1 ? 1 : 0);
            if (line_length > sizeof(buffer) - 1) {
                fprintf(stderr, "Line too long to process\n");
                continue;
            }
            memcpy(buffer, line_start, line_length);
            buffer[line_length] = '\0';
            process_line(ht, buffer);
            line_start = &file_data[offset + 1];
        }
    }

    munmap(file_data, file_size);
    close(fd);
}

void process_fread(const char *filename, HashTable *ht) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        process_line(ht, line);
    }

    fclose(file);
}

int main() {
    HashTable *ht_mmap = create_hash_table(INITIAL_TABLE_SIZE);
    HashTable *ht_fread = create_hash_table(INITIAL_TABLE_SIZE);

    clock_t start, end;
    
    printf("USING FREAD\n");
    start = clock();
    process_fread("q1-10mil.txt", ht_fread); 
    end = clock();
    print_results(ht_fread);
    printf("Time taken using fread: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    printf("\n");

    printf("USING MMAP\n");
    start = clock();
    process_mmap("q1-10mil.txt", ht_mmap); 
    end = clock();
    print_results(ht_mmap);
    printf("Time taken using mmap: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);


    free_hash_table(ht_mmap);
    free_hash_table(ht_fread);

    return 0;
}
