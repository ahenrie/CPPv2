#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_PAGE_TABLES 3  // Adjust if needed 
#define NUM_ENTRIES_PER_TABLE 8
#define OFFSET_BITS 5

typedef struct {
    unsigned int page_frame_number : 14;
    unsigned int valid : 1;
    unsigned int readable : 1;
    unsigned int writable : 1;
    unsigned int executable : 1;
    unsigned int unused : 13; 
} PageTableEntry;

void translate_logical_to_physical(unsigned int logical_address, PageTableEntry page_tables[MAX_PAGE_TABLES][NUM_ENTRIES_PER_TABLE][NUM_ENTRIES_PER_TABLE], int operation) {
    // Extract indices from the logical address
    int index1 = (logical_address >> (OFFSET_BITS + 3)) & 0x7;  // Bits 13-11
    int index2 = (logical_address >> (OFFSET_BITS)) & 0x7;      // Bits 10-8
    int index3 = (logical_address >> 3) & 0x7;                 // Bits 7-5

    // Extract offset
    int offset = logical_address & 0x1F;  // Bits 4-0

    // Traverse page tables 
    PageTableEntry current_entry = page_tables[0][index1][index2]; // Start at Level 1

    for (int level = 1; level < 3; level++) {
        if (!current_entry.valid) {
            printf("Page Fault: Entry %d in table at frame %d is not set valid\n", index1 * 8 + index2 * 8 + index3, current_entry.page_frame_number);
            return;
        }
        int next_table_index = current_entry.page_frame_number;
        current_entry = page_tables[next_table_index][index2][index3];
    }

    // Final Entry: Check validity and permissions 
    if (!current_entry.valid) {
        printf("Page Fault: Entry %d in table at frame %d is not set valid\n", index1 * 8 + index2 * 8 + index3, current_entry.page_frame_number);
        return;
    }
    if ((operation == 'R' && !current_entry.readable) || (operation == 'W' && !current_entry.writable) || (operation == 'X' && !current_entry.executable)) {
        printf("Page Fault. Illegal access to frame %d with access RW-%c%c\n", current_entry.page_frame_number, (current_entry.readable ? 'R' : '-'), (current_entry.writable ? 'W' : '-'));
        return;
    }

    // Construct physical address
    unsigned int physical_address = (current_entry.page_frame_number << OFFSET_BITS) | offset;

    printf("Success. Physical address 0x%08X (Frame %d, Access %c%c%c)\n", physical_address, current_entry.page_frame_number, current_entry.readable ? 'R' : '-', current_entry.writable ? 'W' : '-', current_entry.executable ? 'X' : '-');
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return -1;
    }

    // Read the number of page tables
    int num_page_tables;
    fscanf(fp, "%d", &num_page_tables);

    // Read each page table
    PageTableEntry page_tables[MAX_PAGE_TABLES][NUM_ENTRIES_PER_TABLE][NUM_ENTRIES_PER_TABLE];
    for (int i = 0; i < num_page_tables; i++) {
    int frame_number; 
    fscanf(fp, " frame : %d", &frame_number); 

    for (int j = 0; j < NUM_ENTRIES_PER_TABLE; j++) {
        unsigned int entry_value;
        fscanf(fp, " %u", &entry_value);

        // Use dot operator to access members of the PageTableEntry
        page_tables[i][j][0].page_frame_number = (entry_value >> 5) & 0x3FFF; 
        page_tables[i][j][0].valid = (entry_value >> 4) & 0x01;
        page_tables[i][j][0].readable = (entry_value >> 3) & 0x01;
        page_tables[i][j][0].writable = (entry_value >> 2) & 0x01;
        page_tables[i][j][0].executable = (entry_value >> 1) & 0x01;
    }
}

    // Read the number of tests to perform
    int num_tests;
    fscanf(fp, "%d", &num_tests);

    // Process each test
    for (int i = 0; i < num_tests; i++) {
        char operation;
        unsigned int logical_address;
        fscanf(fp, " %c %u", &operation, &logical_address);

        translate_logical_to_physical(logical_address, page_tables, operation);
    }

    fclose(fp);
    return 0;
}
