#include <stdio.h>
#include <stdlib.h>

#define NUM_FILES 10
#define NUM_BLOCKS 50

typedef struct {
    char file;
    int location;
} FileTable;

typedef struct {
    int blocks[10];
} FileAlloc;

FileTable ControlTable[NUM_FILES];
FileAlloc Alloc_Table[NUM_BLOCKS];

void appendcont(FILE *fptr) {
    for (int i = 0; i < NUM_FILES; i++) {
        fscanf(fptr, "%d", &ControlTable[i].location);
        ControlTable[i].file = 'A' + i;
    }
}

void appendalloc(FILE *fptr) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        for (int j = 0; j < 10; j++) {
            fscanf(fptr, "%d", &Alloc_Table[i].blocks[j]);
        }
    }
}

void print() {
    for (int i = 0; i < NUM_FILES; i++) {
        printf("File %c : ", ControlTable[i].file);
        int temp = ControlTable[i].location;
        if (temp == -1) {
            printf("NULL\n");
            continue;
        }
        for (int j = 0; j < 10; j++) {
            int b = Alloc_Table[temp].blocks[j];
            if (b == -1) break;
            printf("[%d] -> ", b);
        }
        printf("NULL\n");
    }
}

int main() {
    FILE *fcont = fopen("data.txt",  "r");
    FILE *falloc = fopen("alloc.txt", "r");

    appendcont(fcont);
    appendalloc(falloc);

    fclose(fcont);
    fclose(falloc);

    print();
    return 0;
}
