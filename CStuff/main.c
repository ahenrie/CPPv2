#include <stdio.h>
#include <stdint.h>

#define PAGE_SIZE 32
#define NUM_PAGE_TABLE_ENTRIES 8
#define OFF_SET_BITS 5
#define NUM_PAGE_TABLE_LEVELS 3
#define PAGE_FRAME_MASK 0x3FFF // Assuming 14-bit page frame numbers
#define NUM_PAGE_TABLE_ENTRIES_LOG2 3

// Structure for a page table entry 
typedef struct {
    unsigned int unused : 13;       // Bits 31-19 must be zero
    unsigned int page_frame_number : 14; // Bits 18-5
    unsigned int valid;
    unsigned int readable;
    unsigned int writable;
    unsigned int executable;
    unsigned int unused2 : 1;            // Bit 0 must be zero 
} PageTableEntry;

void translate_logical_to_physical(unsigned int logical_address, PageTableEntry (*page_tables)[NUM_PAGE_TABLE_ENTRIES], int num_page_tables, char operation, int count) {
    // Extract page frame number and offset
    unsigned int page_frame_number = (logical_address >> OFF_SET_BITS) & PAGE_FRAME_MASK;
    unsigned int offset = logical_address & 0x1F;

    // Check for invalid upper bits
    if ((logical_address & 0xFFFFC000) != 0) {
        printf("Op#%d (%c 0x%08X) Page Fault. Logical address has non-zero bits in bits 31-14\n", count, operation, logical_address);
        return;
    }

    // Traverse through page tables 
    for (int level = 0; level < NUM_PAGE_TABLE_LEVELS; level++) {
        int index_bits = NUM_PAGE_TABLE_ENTRIES_LOG2;  
        int index_mask = (1 << index_bits) - 1;  

        int table_index = (logical_address >> (OFF_SET_BITS + level * index_bits)) & index_mask;
        int entry_index = page_frame_number % NUM_PAGE_TABLE_ENTRIES; 

        // Access the page table entry
        PageTableEntry entry = page_tables[table_index][entry_index];

        // Check for page fault conditions (Temporarily Uncomment for Debugging)
        /*
        if (!entry.valid) {
            printf("Op#%d (%c 0x%08X) Page Fault. Entry %d in table at frame %d is not set valid\n", count, operation, logical_address, logical_address >> 5, logical_address / PAGE_SIZE);
            return;
        }
        if ((operation == 'R' || operation == 'X') && !entry.readable) {
            printf("Op#%d (%c 0x%08X) Page Fault. Illegal access to frame %d due to entry %d in table %d with access RW-\n", count, operation, logical_address, page_frame_number, entry_index, logical_address >> 5);
            return;
        }
        if (operation == 'W' && !entry.writable) {
            printf("Op#%d (%c 0x%08X) Page Fault. Entry %d in table at frame %d is not set valid\n", count, operation, logical_address, logical_address >> 5, logical_address / PAGE_SIZE);
            return;
        }
        */

        // Update page frame number for the next level (if not the final level)
        if (level < NUM_PAGE_TABLE_LEVELS - 1) {
            page_frame_number = entry.page_frame_number;
        } else {  // Final level
            // Construct physical address using the page frame number from the last entry
            unsigned int physical_address = (entry.page_frame_number << OFF_SET_BITS) | offset;

            // Print the translation result  
            printf("Op#%d (%c 0x%08X) Success. Physical address 0x%08X (Frame %d , Access R", count, operation, logical_address, physical_address, entry.page_frame_number);
            if (entry.executable == 1 && entry.writable != 1) printf("-X");
            if (entry.writable == 1) printf("W-");
            printf(")\n"); // End the line
        }
    }
}

int main(int argc, char *argv[]) {
    // Check for correct number of command line arguments
    if (argc != 2) {
        printf("Usage: %s <filename>\n",argv[0]);
        return -1;
    }

    // Open the file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return -1;
    }

    // Read the number of page tables
    int num_page_tables;
    fscanf(fp, "%d", &num_page_tables);

    // Read each page table
    PageTableEntry page_tables[num_page_tables][NUM_PAGE_TABLE_ENTRIES];
    for (int i = 0; i < num_page_tables; i++) {
        int frame_number; // Not directly used, but might be good to have
        fscanf(fp, " frame : %d", &frame_number); 

        for (int j = 0; j < NUM_PAGE_TABLE_ENTRIES; j++) {
            unsigned int entry_value;
            fscanf(fp, " %u", &entry_value);

            page_tables[i][j].page_frame_number = (entry_value >> 5) & 0x3FFF; 
            page_tables[i][j].valid = (entry_value >> 4) & 0x01;
            page_tables[i][j].readable = (entry_value >> 3) & 0x01;
            page_tables[i][j].writable = (entry_value >> 2) & 0x01;
            page_tables[i][j].executable = (entry_value >> 1) & 0x01;
        }
    }

    // Read the number of tests to perform
    int num_tests;
    fscanf(fp, "%d", &num_tests);

    // Print the assignment header
    printf("Assignment 4: MMU Simulator by Arza Henrie\n");

    // Process each test
    for (int i = 0; i < num_tests; i++) {
        char operation;
        unsigned int logical_address;
        fscanf(fp, " %c %u", &operation, &logical_address);

        // Translate logical address to physical address
	    translate_logical_to_physical(logical_address, page_tables, num_page_tables, operation, i);
    }

    // Close the file
    fclose(fp);

    return 0;
}