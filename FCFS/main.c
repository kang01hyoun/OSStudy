#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define PROCESSMAX 10

typedef struct {
    int PID;
    int burst_time;
    int running_time;
    bool status;
} PCB;

typedef struct {
    PCB *queue[PROCESSMAX];
    int now_index;
} scheduler;

void printresult(int total_wait_time,int total_return_time,int input) {
    float avg_waittime = (float)total_wait_time / input;
    float avg_returntime = (float)total_return_time / input;

    printf("SCHEDULING COMPLETE\n");
    printf("AVERAGE WAITING TIME %f ms \n", avg_waittime * 100);
    printf("AVERAGE TURNAROUND TIME %f ms \n", avg_returntime * 100);

}

void createProcess(PCB *process,int pid) {
    process->status = true;
    process->PID = pid+1;
    process->burst_time = rand() % 3 + 1;
    process->running_time = 0;
}

void FCFS(scheduler *scheduler,int input) {
    int current_time = 0;
    int total_wait_time = 0;
    int total_return_time = 0;
    PCB *PCBF[input];

    for (int i = 0; i < input; i++) {
        PCBF[i] = scheduler->queue[i];
        if(PCBF[i]->status == false){
            continue;
       }

        printf("PID %d is starting\n", PCBF[i]->PID);
       printf("PID %d IS RUNNING\n", PCBF[i]->PID);

        for (int t = 0; t < PCBF[i]->burst_time; t++) {

            Sleep(100);
        }

        current_time += PCBF[i]->burst_time;
        PCBF[i]->running_time = PCBF[i]->burst_time;
        PCBF[i]->status = false;

        int waittime = current_time - PCBF[i]->burst_time;
        printf("PID %d IS FINISHED. WAIT TIME: %d ms, TURNAROUND TIME: %d ms \n\n", PCBF[i]->PID, waittime * 100, current_time * 100);

        total_wait_time += waittime;
        total_return_time += current_time;
    }
    printresult(total_wait_time,total_return_time,input);
}

int main() {
    int input = 0;
    scheduler scheduler;
    scheduler.now_index = 0;


    printf("Enter the number of process (MAX 10): ");
    scanf("%d", &input);

    if (input > 10 || input <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    srand(time(NULL));

    PCB pcb_array[PROCESSMAX];

    for (int i = 0; i < input; i++) {
        createProcess(&pcb_array[i], i);
        scheduler.queue[i] = &pcb_array[i];
    }

    FCFS(&scheduler,input);

    return 0;
}
