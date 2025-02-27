#include <stdio.h>
#include <stdlib.h>

#define PAGE_FAULT_PD                                                   \
        printf("Page Fault: Page table does not exist in memory\n");    \
        printf("Allocating a page table\n");

#define PAGE_FAULT_PT                                                           \
        printf("Page Fault: Page table entry does not exist in memory\n");      \
        printf("Allocating memory\n");

int miss=0,hit=0;
int pd_size=0,pt_size[1024],curr_table;

unsigned int load(unsigned int va,unsigned int **pd[]) {
    int pde=va>>22;
    if (pd[pde]==NULL) {
        miss++;
        pd_size++;
        PAGE_FAULT_PD;
        pd[pde]=(unsigned int**)malloc(1024*sizeof(unsigned int*));
        for (int i=0; i<1024; i++) {
            pd[pde][i]=NULL;
        }
        printf("Page Directory Updated\n");
        printf("Page Table added at PD Index: %d\n",pde);
    }
    else
        hit++;
    int pte=(va<<10)>>22;
    if (pd[pde][pte]==NULL) {
        miss++;
        pt_size[pde]++;
        PAGE_FAULT_PT;
        pd[pde][pte]=(unsigned int*)malloc(1024*sizeof(unsigned int));
        printf("Page Table Updated\n");
        printf("Page added at PT Index: %d\n",pte);
    }
    else
        hit++;
    curr_table=pde;
    int offset=(va<<20)>>20;
    unsigned int num=pd[pde][pte][offset];
    int part=offset%4;
    return (num<<(8*part))>>24;
}

void store(unsigned int va,unsigned int **pd[]) {
    int pde=va>>22;
    if (pd[pde]==NULL) {
        miss++;
        pd_size++;
        PAGE_FAULT_PD;
        pd[pde]=(unsigned int**)malloc(1024*sizeof(unsigned int*));
        for (int i=0; i<1024; i++) {
            pd[pde][i]=NULL;
        }
        printf("Page Directory Updated\n");
        printf("Page Table added at PD Index: %d\n",pde);
    }
    else
        hit++;
    int pte=(va<<10)>>22;
    if (pd[pde][pte]==NULL) {
        miss++;
        pt_size[pde]++;
        PAGE_FAULT_PT;
        pd[pde][pte]=(unsigned int*)malloc(1024*sizeof(unsigned int));
        printf("Page Table Updated\n");
        printf("Page added at PT Index: %d\n",pte);
    }
    else
        hit++;
    curr_table=pde;
    int offset=(va<<20)>>20;
    int part=offset%4;
    unsigned int num;
    printf("Enter Value from 0-255: ");
    scanf("%d",&num);
    num=num<<24;
    num=num>>(8*part);
    unsigned int access=pd[pde][pte][offset];
    if (part==0) {
        access=(access<<8)>>8;
    }
    else if (part==1) {
        unsigned int save=(access>>24)<<24;
        access=(access<<16)>>16;
        access=save|access;
    }
    else if (part==2) {
        unsigned int save=(access<<24)>>24;
        access=(access>>16)<<16;
        access=save|access;
    }
    else {
        access=(access>>8)<<8;
    }
    num=num|access;
    pd[pde][pte][offset]=num;
}

unsigned int hex_to_dec(char hex[9]) {
    unsigned int dec=0,base=1;
    for (int i=7; i>=0; i--) {
        int val;
        if ('0'<=hex[i] && hex[i]<='9') {
            val=hex[i]-'0';
        }
        else {
            val=hex[i]-'a'+10;
        }
        dec+=val*base;
        base=base<<4;
    }
    return dec;
}

int main() {
    unsigned int **pd[1024];
    for (int i=0; i<1024; i++) {
        pd[i]=NULL;
        pt_size[i]=0;
    }
    while (1) {
        int ch;
        printf("Choose An Option:\n");
        printf("1. Load\n");
        printf("2. Store\n");
        printf("3. Exit\n");
        scanf("%d",&ch);
        if (ch==1) {
            char hex_va[9];
            printf("Enter Virtual Address: ");
            scanf("%s",hex_va);
            unsigned int va=hex_to_dec(hex_va);
            printf("%u\n",load(va,pd));
            printf("Total Number of Hits: %d\n",hit);
            printf("Total Number of Misses: %d\n",miss);
            printf("Page Directory Size: %d\n",pd_size);
            printf("Accessed Page Table Size: %d\n",pt_size[curr_table]);
        }
        else if (ch==2) {
            char hex_va[9];
            printf("Enter Virtual Address: ");
            scanf("%s",hex_va);
            unsigned int va=hex_to_dec(hex_va);
            store(va,pd);
            printf("Total Number of Hits: %d\n",hit);
            printf("Total Number of Misses: %d\n",miss);
            printf("Page Directory Size: %d\n",pd_size);
            printf("Accessed Page Table Size: %d\n",pt_size[curr_table]);
        }
        else if (ch==3) {
            break;
        }
        else {
            printf("Invalid Input\n");
        }
    }
    printf("Session Summary:\n");
    printf("Final Page Directory Size: %d\n",pd_size);
    printf("PD Index - Final Page Table Size:\n");
    for (int i=0; i<1024; i++) {
        if (pt_size[i]!=0) {
            printf("Page Table %d - %d\n",i,pt_size[i]);
        }
    }
    printf("Final Number of Hits: %d\n",hit);
    printf("Final Number of Misses: %d\n",miss);
    if (miss!=0)
        printf("Hit/Miss Ratio: %f\n",(hit*1.0)/miss);
    else
        printf("Hit/Miss Ratio: null\n");
}
