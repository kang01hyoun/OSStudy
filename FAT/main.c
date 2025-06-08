#include <stdio.h>;
#include <stdlib.h>;

typedef struct FileTable {
    char file;
    int location;
}FileTable;

typedef struct FileAlloc {
    char data[10];
    int next;
}FileAlloc;

FileTable ControlTable[10];
FileAlloc Alloc_Table[50];

void appendcont(FILE *fptr) {
    for (int i = 0; i < 10; i++) {
        fscanf(fptr, "%d", &ControlTable[i].location);
        ControlTable[i].file = i + 65;
    }
}

void appendalloc(FILE *fptr) {
    for (int i = 0; i < 50; i++) {
        fscanf(fptr, "%d", &Alloc_Table[i].next);
    }
}

void print() {
    for (int i = 0; i < 10; i++) {
        printf("File %c : ", ControlTable[i].file);
        int temp = ControlTable[i].location;
        if (temp == -1) {
            printf("NULL\n");
            continue;
        }
        while (temp != -1) {
            printf("[%d] -> ", temp);
            temp = Alloc_Table[temp].next;
        }
        printf("NULL\n");
    }
}


int main() {
    FILE *fcont;
    FILE *falloc;
    fcont = fopen("data.txt","r");
    falloc = fopen("alloc.txt","r");

    appendcont(fcont);
    appendalloc(falloc);

    print();
}




