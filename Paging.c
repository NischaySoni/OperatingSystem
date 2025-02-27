#include <stdio.h>

struct tlbe {
    int vpn;
    int pfn;
};

int hex_to_dec(char hex[5]) {
    int dec=0,base=1;
    for (int i=3; i>=0; i--) {
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

void dec_to_hex(int dec,char hex[5]) {
    int write=3;
    while (dec>0) {
        int val=dec%16;
        if (val<10) {
            hex[write--]='0'+val;
        }
        else {
            val-=10;
            hex[write--]='a'+val;
        }
        dec=dec>>4;
    }
    hex[4]='\0';
}

void swap(struct tlbe *e1,struct tlbe *e2) {
    struct tlbe temp=*e1;
    *e1=*e2;
    *e2=temp;
}

void update_tlb(struct tlbe tlb[],int vpn,int page_table[]) {
    for (int i=3; i>=0; i--) {
        if (tlb[i].vpn==-1) {
            for (int j=i; j<3; j++) {
                swap(&tlb[j],&tlb[j+1]);
            }
            tlb[3].vpn=vpn;
            tlb[3].pfn=page_table[vpn];
            return;
        }
    }
    for (int i=0; i<3; i++) {
        swap(&tlb[i],&tlb[i+1]);
    }
    tlb[3].vpn=vpn;
    tlb[3].pfn=page_table[vpn];
}

void move_to_back(struct tlbe tlb[],int ind) {
    for (int i=ind; i<3; i++) {
        swap(&tlb[i],&tlb[i+1]);
    }
}

int check_tlb(struct tlbe tlb[],int vpn,int page_table[]) {
    for (int i=3; i>=0; i--) {
        if (tlb[i].vpn==-1) {
            update_tlb(tlb,vpn,page_table);
            return -1;
        }
        if (tlb[i].vpn==vpn) {
            move_to_back(tlb,i);
            return tlb[3].pfn;
        }
    }
    update_tlb(tlb,vpn,page_table);
    return -1;
}

int main() {
    int page_table[16]={7,12,9,14,15,2,5,1,0,3,11,13,4,10,6,8};
    struct tlbe tlb[4];
    for (int i=0; i<4; i++) {
        tlb[i].vpn=-1;
        tlb[i].pfn=-1;
    }
    int n;
    printf("Enter Number of Memory Accesses: ");
    scanf("%d",&n);
    for (int i=0; i<n; i++) {
        char hex_vir_addr[5];
        printf("Enter Virtual Address: ");
        scanf("%s",hex_vir_addr);
        int vir_addr=hex_to_dec(hex_vir_addr);
        int offset=vir_addr & ((unsigned int)-1>>20);
        int vpn=vir_addr>>12;
        int pfn=check_tlb(tlb,vpn,page_table);
        if (pfn!=-1) {
            printf("TLB Hit\n");
        }
        else {
            printf("TLB Miss\n");
            pfn=page_table[vpn];
        }
        int phy_addr=(pfn<<12)|offset;
        char hex_phy_addr[5];
        dec_to_hex(phy_addr,hex_phy_addr);
        printf("%s\n",hex_phy_addr);
    }
}
