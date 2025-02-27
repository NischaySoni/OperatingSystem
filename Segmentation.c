#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 65536    // 64KB 
#define CODE_BASE 32768      // 32KB
#define CODE_BOUND 2048      // 2KB
#define HEAP_BASE 34816      // 34KB
#define HEAP_BOUND 3072      // 3KB
#define STACK_BASE 28672     // 28KB
#define STACK_BOUND 2048     // 2KB
#define SEG_MASK 0xC000      // to extract top 2 bits for segment (0b1100000000000000)
#define SEG_SHIFT 14         // Right shift by 14 to get top 2 bits for segment
#define OFFSET_MASK 0x3FFF   // to extract offset (lower 14 bits 0b0011111111111111)

enum SegmentType {CODE, HEAP, STACK};

int Base[] = {CODE_BASE, HEAP_BASE, STACK_BASE};
int Bounds[] = {CODE_BOUND, HEAP_BOUND, STACK_BOUND};

int AccessMemory(int physicalAddress) {
    return physicalAddress;
}

void translate(int virtualAddress) {
    // extracting top 2 bits
    int Segment = (virtualAddress & SEG_MASK) >> SEG_SHIFT;
    // remaining 14 bits
    int Offset = virtualAddress & OFFSET_MASK;
    //printf("%d \n", Offset);
    if (Offset >= Bounds[Segment]) {
        printf("Segmentation Fault: Address Out of Bounds\n");
        return;
    }
    int physicalAddress;
    if (Segment == STACK) {
        physicalAddress = Base[Segment] - (Bounds[Segment] - Offset);
        if (physicalAddress < (STACK_BASE - STACK_BOUND)) {
            printf("Segmentation Fault: Stack Overflow\n");
            return;
        }
        printf("The address is in Stack\n");
    } 
    else {
        physicalAddress = Base[Segment] + (Offset);
        if (Segment == HEAP) {
            printf("The address is in Heap\n");
        } 
        else if (Segment == CODE) {
            printf("The address is in Code\n");
        }
    }
    int Register = AccessMemory(physicalAddress);
    printf("Physical Address: %04X (Register Value: %d)\n", physicalAddress, Register);
}
int main() {
    char addressChar[5];
    int logicalAddress;
    printf("Enter a 16-bit logical address in hexadecimal: ");
    scanf("%4s", addressChar);
    char *endptr;
    logicalAddress = (int)strtol(addressChar, &endptr, 16);
    if (*endptr != '\0' || logicalAddress < 0 || logicalAddress > 65535) {
        printf("Invalid input. Please enter a valid 16-bit hexadecimal address.\n");
        return 1;
    }
    translate(logicalAddress);
    return 0;
}
