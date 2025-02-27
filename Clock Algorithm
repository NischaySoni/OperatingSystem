#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct page {
    uint8_t page_number;
    bool reference_bit;
    bool dirty_bit;
};

int findAndUpdate(int referenced_page, struct page frames[], int num_frames, int pointer) {
    while (true) {
        if (frames[pointer].reference_bit == 0) {
            frames[pointer].page_number = referenced_page;
            frames[pointer].reference_bit = 1;  // Set reference bit for the new page
            frames[pointer].dirty_bit = false;  // New page is clean initially
            pointer = (pointer + 1) % num_frames;
            return pointer;
        }
        // second chance
        frames[pointer].reference_bit = 0;
        pointer = (pointer + 1) % num_frames;
    }
}

bool needUpdate(int reference_page, int num_frames, struct page frames[]) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].page_number == reference_page) {
            frames[i].reference_bit = 1;  // Set reference bit to 1
            frames[i].dirty_bit = true;   // The page is modified (dirty)
            return false;  // No replacement needed
        }
    }
    return true;  // replacement needed
}

int clockReplacement(int page_requests[], int n, int num_frames, struct page frames[]) {
    int pageFault = 0;
    int pointer = 0;
    for (int i = 0; i < n; i++) {
        if (needUpdate(page_requests[i], num_frames, frames)) {
            pointer = findAndUpdate(page_requests[i], frames, num_frames, pointer);
            pageFault++;
        }
    }
    return pageFault;
}

int main() {
    int n;
    printf("Enter the number of pages:\n");
    scanf("%d", &n);

    int num_frames;
    int pageFault = 0;
    int page_requests[n];

    printf("Enter the page numbers:\n");
    for (int i = 0; i < n; i++){
        scanf("%d", &page_requests[i]);
    }

    printf("Enter the number of frames:\n");
    scanf("%d", &num_frames);
    
    struct page frames[num_frames];
    for (int i = 0; i < num_frames; i++) {
        frames[i].page_number = -1;
        frames[i].reference_bit = false;
        frames[i].dirty_bit = false; 
    }
    pageFault = clockReplacement(page_requests, n, num_frames, frames);

    printf("\nTotal Number of Page Faults = %d", pageFault);
    printf("\nTotal Number of Page Hits = %d", n - pageFault);
    printf("\nPage Hit Rate = %.2f\n\n", (float)(n - pageFault) / n);

    return 0;
}
