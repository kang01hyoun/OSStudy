#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct { //프로세스 구조체
    int PID;
    int arrivalTime;
    int burstTime;
    int waitTime;
    int turnaroundTime;
} Process;

Process* queue[MAX]; // 프로세스 큐 구조체

void FCFS(int input) { //FCFS 구현 함수
    float totalWAIT = 0;
    float totalTURNAROUND = 0;
    int Time = 0;

    for (int i = 0; i < input; i++) {
        int arrivalT = queue[i]->arrivalTime; //도착 시간
        int burstT = queue[i]->burstTime; //실행 시간
        int waitT = Time - arrivalT; //대기 시간 : 현재 시간 - 도착 시간
        int returnT = waitT + burstT; // 반환 시간 : 대기 시간 + 실행 시간
        Time += burstT; // 현재 시간 = 실행 시간

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

void CreateProcess(Process *process, int i) { //프로세스 생성 함수
    process->PID = i + 1; //PID
    process->arrivalTime = i; //도착 시간
    process->burstTime = rand() % 3 + 1; //실행 시간
    process->waitTime = 0; //대기 시간
    process->turnaroundTime = 0; //반환 시간
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
        CreateProcess(&process[i], i); // process 구조체 배열의 i번째 주소값을 전달, 그리고 i 도 전달
        queue[i] = &process[i]; // queue 구조체 배열에 process 구조체 배열의 i번째 주소값을 전달
    }

    FCFS(input); // FCFS 함수 실행

    return 0;
}
