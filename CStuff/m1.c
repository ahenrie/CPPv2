#include <stdio.h>
#include <stdint.h>

#define PAGE_SIZE 32
#define NUM_PAGE_TABLE_ENTRIES 8
#define OFF_SET_BITS 5
#define NUM_PAGE_TABLE_LEVELS 3
#define PAGE_FRAME_MASK 0x3FFF 

typedef struct {
    unsigned int unused : 13;       
    unsigned int page_frame_number : 14; 
    unsigned int valid : 1;
    unsigned int readable : 1;
    unsigned int writable : 1;
    unsigned int executable : 1;
    unsigned int unused2 : 1;           
} PageTableEntry;

void translate_logical_to_physical(unsigned int logical_address, PageTableEntry (*page_tables)[NUM_PAGE_TABLE_ENTRIES], int num_page_tables, char operation, int count) {
    unsigned int page_number = logical_address >> OFF_SET_BITS; 
    int table_index_1 = (page_number >> 6) & 0x07; 
    int table_index_2 = (page_number >> 3) & 0x07; 
    int entry_index   = page_number & 0x07;        
    unsigned int offset   = logical_address & 0x1F;  

    PageTableEntry entry_1 = page_tables[table_index_1][entry_index];

    if ((logical_address & 0xFFFFC000) != 0) {
        printf("Op#%d (%c 0x%08X) Page Fault. Logical address has non-zero bits in bits 31-14\n", count, operation, logical_address);
        return;
    }
    if (!entry_1.valid) {
        printf("Op#%d (%c 0x%08X) Page Fault. Entry %d in table at frame %d is not set valid\n", count, operation, logical_address, logical_address >> 5, logical_address / PAGE_SIZE);
        return;
    }

    PageTableEntry entry_2 = page_tables[entry_1.page_frame_number][table_index_2];
    if (!entry_2.valid) {
        printf("Op#%d (%c 0x%08X) Page Fault. Entry %d in table at frame %d is not set valid\n", count, operation, logical_address, logical_address >> 5, logical_address / PAGE_SIZE);
        return;
    }

    PageTableEntry entry_3 = page_tables[entry_2.page_frame_number][entry_index];
    if (!entry_3.valid) {
        printf("Op#%d (%c 0x%08X) Page Fault. Entry %d in table at frame %d is not set valid\n", count, operation, logical_address, logical_address >> 5, logical_address / PAGE_SIZE);
        return;
    }
    if ((operation == 'R' || operation == 'X') && !entry_3.readable) {
        printf("Op#%d (%c 0x%08X) Page Fault. Illegal access to frame %d due to entry %d in table %d with access RW-\n", count, operation, logical_address, entry_3.page_frame_number, entry_index, logical_address >> 5);
        return;
    }
    if (operation == 'W' && !entry_3.writable) {
        printf("Op#%d (%c 0x%08X) Page Fault. Illegal access to frame %d due to entry %d in table %d with access --W\n", count, operation, logical_address, entry_3.page_frame_number, entry_index, logical_address >> 5);
        return;
    }

    unsigned int physical_address = (entry_3.page_frame_number << OFF_SET_BITS) | offset; 
    printf("Op#%d (%c 0x%08X) Success. Physical address 0x%08X (Frame %d , Access ", count, operation, logical_address, physical_address, entry_3.page_frame_number);
    if (entry_3.readable) printf("R");
    if (entry_3.writable) printf("W");
    if (entry_3.executable) printf("X");
    printf(")\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n",argv[0]);
        return -1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return -1;
    }
    int num_page_tables;
    fscanf(fp, "%d", &num_page_tables);
    PageTableEntry page_tables[num_page_tables][NUM_PAGE_TABLE_ENTRIES][NUM_PAGE_TABLE_ENTRIES];
    for (int i = 0; i < num_page_tables; i++) {
        int frame_number;
        fscanf(fp, " frame : %d", &frame_number); 
        for (int j = 0; j < NUM_PAGE_TABLE_ENTRIES; j++) {
            int frame_number2;
            fscanf(fp, " frame : %d", &frame_number2);
            for (int k = 0; k < NUM_PAGE_TABLE_ENTRIES; k++) {
                unsigned int entry_value;
                fscanf(fp, " %u", &entry_value);
                page_tables[i][j][k].page_frame_number = (entry_value >> 5) & 0x3FFF; 
                page_tables[i][j][k].valid = (entry_value >> 4) & 0x01;
                page_tables[i][j][k].readable = (entry_value >> 3) & 0x01;
                page_tables[i][j][k].writable = (entry_value >> 2) & 0x01;
                page_tables[i][j][k].executable = (entry_value >> 1) & 0x01;
            }
        }
    }
    int num_tests;
    fscanf(fp, "%d", &num_tests);
    printf("Assignment 4: MMU Simulator by Arza Henrie\n");
    for (int i = 0; i < num_tests; i++) {
        char operation;
        unsigned int logical_address;
        fscanf(fp, " %c %u", &operation, &logical_address);
        translate_logical_to_physical(logical_address, page_tables, num_page_tables, operation, i);
    }
    fclose(fp);
    return 0;
}
