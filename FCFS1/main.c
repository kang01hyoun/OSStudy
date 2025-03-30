#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct { //���μ��� ����ü
    int PID;
    int arrivalTime;
    int burstTime;
    int waitTime;
    int turnaroundTime;
} Process;

Process* queue[MAX]; // ���μ��� ť ����ü

void FCFS(int input) { //FCFS ���� �Լ�
    float totalWAIT = 0;
    float totalTURNAROUND = 0;
    int Time = 0;

    for (int i = 0; i < input; i++) {
        int arrivalT = queue[i]->arrivalTime; //���� �ð�
        int burstT = queue[i]->burstTime; //���� �ð�
        int waitT = Time - arrivalT; //��� �ð� : ���� �ð� - ���� �ð�
        int returnT = waitT + burstT; // ��ȯ �ð� : ��� �ð� + ���� �ð�
        Time += burstT; // ���� �ð� = ���� �ð�

        queue[i]->waitTime = waitT;
        queue[i]->turnaroundTime = returnT;

        printf("PROCESS PID %d IS STARTING\n", queue[i]->PID);
        printf("PROCESS PID %d IS FINISHED\n", queue[i]->PID);
        printf("WAIT TIME %d BURST TIME %d RETURN TIME %d ARRIVAL TIME %d\n",waitT, burstT, returnT, arrivalT);

        totalWAIT += waitT;
        totalTURNAROUND += returnT;
    }

    printf("\nAVG WAITING TIME %f", totalWAIT / (float)input);
    printf("\nAVG TURN AROUND TIME %f", totalTURNAROUND / (float)input);
}

void CreateProcess(Process *process, int i) { //���μ��� ���� �Լ�
    process->PID = i + 1; //PID
    process->arrivalTime = i; //���� �ð�
    process->burstTime = rand() % 3 + 1; //���� �ð�
    process->waitTime = 0; //��� �ð�
    process->turnaroundTime = 0; //��ȯ �ð�
}

int main() {
    int input = 0;

    while (1) {
        scanf("%d", &input);
        if (input <= 0 || input > 10) {
            printf("\nWRONG NUMBER. PLEASE TRY (0 < N <= 10)\n");
            continue;
        }
        else break;
    }

    Process process[input];

    for (int i = 0; i < input; i++) {
        CreateProcess(&process[i], i); // process ����ü �迭�� i��° �ּҰ��� ����, �׸��� i �� ����
        queue[i] = &process[i]; // queue ����ü �迭�� process ����ü �迭�� i��° �ּҰ��� ����
    }

    FCFS(input); // FCFS �Լ� ����

    return 0;
}
