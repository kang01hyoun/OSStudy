#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int PID; //프로세스 PID 1부터 시작
    int arrivalTime; // 도착시간
    int burstTime; // 실행시간
    int waitTime; // 대기시간
    int turn_aroundTime; //반환시간
    int priority; //우선순위 1~3
}Process;
Process *queue[20];

void PrintResult(char *title, Process **queue, int count) {
    float sumwait = 0;
    float sumturnaround = 0;
    printf("\n %s scheduling ", title);
    for (int i = 0; i < count; i++) {
        printf("\nPID %d WAIT %d TURNAROUND %d ", queue[i]->PID, queue[i]->waitTime, queue[i]->turn_aroundTime);
        sumwait += queue[i]->waitTime;
        sumturnaround += queue[i]->turn_aroundTime;
    }
    printf("\nAVERAGE WAIT TIME = %.3f, AVERAGE TURNAROUND TIME = %.4f\n", sumwait / count, sumturnaround / count);
}

void SJF(int input) {
    int TotalTime = 0, completed = 0;
    bool isDone[20] = {false};

    while (completed < input) {
        int idx = -1;
        // 후보 중 가장 작은 burst 찾기
        for (int i = 0; i < input; i++) {
            if (!isDone[i] && queue[i]->arrivalTime <= TotalTime) {
                if (idx == -1 || queue[i]->burstTime < queue[idx]->burstTime) {
                    idx = i;
                }
            }
        }
        if (idx < 0) {
            TotalTime++;
            continue;
        }
        Process *p = queue[idx];
        p->waitTime = TotalTime - p->arrivalTime;
        TotalTime += p->burstTime;
        p->turn_aroundTime = p->waitTime + p->burstTime;
        isDone[idx] = true;
        completed++;
    }
    PrintResult("SJF", queue, input);
}

void HRN(int input) {
    int totalTime = 0;              // 시간
    int completed = 0;              // 완료된 프로세스 수
    bool isDone[20] = {false};      // 완료 여부
    float responseRatio[20];

    while (completed < input) {
        // responesratio 계산
        for (int i = 0; i < input; i++) {
            if (!isDone[i] && queue[i]->arrivalTime <= totalTime) {
                int wait = totalTime - queue[i]->arrivalTime;
                responseRatio[i] = (float)(wait + queue[i]->burstTime)
                                   / queue[i]->burstTime;
            } else {
                responseRatio[i] = -1.0f;
            }
        }

        // 2) 가장 큰 반응시간을 찾기
        int next = -1;
        float maxRatio = -1.0f;
        for (int i = 0; i < input; i++) {
            if (responseRatio[i] > maxRatio) {
                maxRatio = responseRatio[i];
                next = i;
            }
        }

        // 프로세스 실행
        queue[next]->waitTime = totalTime - queue[next]->arrivalTime;
        totalTime += queue[next]->burstTime;
        queue[next]->turn_aroundTime
            = totalTime - queue[next]->arrivalTime;
        isDone[next] = true;
        completed++;
    }

    // 결과 출력
    PrintResult("HRN", queue, input);
}

void FCFS(int input) {
    int TotalTime = 0;
    for (int i = 0; i < input; i++) {
        Process *temp = queue[i];
        if (TotalTime < temp->arrivalTime) {
            while (TotalTime < temp->arrivalTime) {
                TotalTime++;
            }
        }
        temp->waitTime = TotalTime - temp->arrivalTime;
        TotalTime += temp->burstTime;
        temp->turn_aroundTime = temp->waitTime + temp->burstTime;
    }
    PrintResult("FCFS",queue,input);
}

void RR(int input, int timeSlice)
{
    int TotalTime = 0, completed = 0;

    //실행시간 초기화
    int remainTime[20];
    for (int i = 0; i < input; ++i)
        remainTime[i] = queue[i]->burstTime;

    //준비 큐
    int ready[100];
    int front = 0, rear = 0;
    bool inQueue[20] = {false};  //중복 삽입 방지

    while (completed < input) {
        int slice;
        for (int i = 0; i < input; ++i)
            if (!inQueue[i] && queue[i]->arrivalTime <= TotalTime && remainTime[i] > 0) {
                ready[rear++] = i;
                inQueue[i] = true;
            }

        int PCBindex = ready[front++];
        inQueue[PCBindex] = false;

        if (remainTime[PCBindex] < timeSlice) {
            slice = remainTime[PCBindex];
        } else {
            slice = timeSlice;
        }
        remainTime[PCBindex] -= slice;
        TotalTime += slice;

        //새로 온 프로세스 삽입
        for (int i = 0; i < input; ++i)
            if (!inQueue[i] &&
                queue[i]->arrivalTime > TotalTime - slice &&
                queue[i]->arrivalTime <= TotalTime &&
                remainTime[i] > 0) {
                ready[rear++] = i;
                inQueue[i] = true;
                }

        // 완료 여부 확인
        if (remainTime[PCBindex] > 0) {//미완료시 큐 뒤로
            ready[rear++] = PCBindex;
            inQueue[PCBindex] = true;
        } else {                         /* 완료 */
            queue[PCBindex]->turn_aroundTime = TotalTime - queue[PCBindex]->arrivalTime;
            ++completed;
        }
    }

    //대기시간 계산
    for (int i = 0; i < input; ++i)
        queue[i]->waitTime = queue[i]->turn_aroundTime - queue[i]->burstTime;

    PrintResult("RR", queue, input);
}

void Priority_Queue(int input) {
    int TotalTime = 0, completed = 0;
    bool isDone[20] = {false};

    while (completed < input) {
        int PCBIndex = -1;
        // 우선순위 선택
        for (int i = 0; i < input; i++) {
            if (!isDone[i] && queue[i]->arrivalTime <= TotalTime) {
                if (PCBIndex == -1 || queue[i]->priority < queue[PCBIndex]->priority) {
                    PCBIndex = i;
                }
            }
        }
        Process *p = queue[PCBIndex];
        p->waitTime = TotalTime - p->arrivalTime;
        TotalTime += p->burstTime;
        p->turn_aroundTime = p->waitTime + p->burstTime;
        isDone[PCBIndex] = true;
        completed++;
    }
    PrintResult("Priority", queue, input);
}

void SRTF(int input) {
    int time = 0, completed = 0;
    int remainTime[20], compTime[20];
    bool done[20] = {false};

    for (int i = 0; i < input; i++)
        remainTime[i] = queue[i]->burstTime;

    while (completed < input) {
        int idx = -1;
        // 최소 남은시간 선택
        for (int i = 0; i < input; i++) {
            if (!done[i] && queue[i]->arrivalTime <= time && remainTime[i] > 0) {
                if (idx == -1 || remainTime[i] < remainTime[idx]) {
                    idx = i;
                }
            }
        }
        remainTime[idx]-=2;
        time+=2;
        if (remainTime[idx] == 0) {
            done[idx] = true;
            completed++;
            compTime[idx] = time;
        }
        else if (remainTime[idx] == -1) {
            done[idx] = true;
            completed++;
            compTime[idx] = time - 1;
        }
    }
    for (int i = 0; i < input; i++) {
        queue[i]->turn_aroundTime = compTime[i] - queue[i]->arrivalTime;
        queue[i]->waitTime        = queue[i]->turn_aroundTime - queue[i]->burstTime;
    }
    PrintResult("SRTF", queue, input);
}

void MLQ(int input) {
    int remainTime[20];
    int TotalTime = 0;
    int completed = 0;
    int queue1[20], front1 = 0, rear1 = 0; // RR 큐
    int queue2[20], front2 = 0, rear2 = 0; // FCFS 큐

    // 초기 남은 실행 시간 설정
    for (int i = 0; i < input; i++) {
        remainTime[i] = queue[i]->burstTime;
    }

    // 프로세스들을 우선순위에 따라 큐에 추가
    for (int i = 0; i < input; i++) {
        if (queue[i]->arrivalTime <= TotalTime) {
            if (queue[i]->priority == 1)
                queue1[rear1++] = i;
            else
                queue2[rear2++] = i;
        }
    }

    int PCBIndex = -1;
    int timeSlice = 2; // RR타임슬라이스

    // 큐1 RR방식 처리
    while (front1 < rear1) {
        PCBIndex = queue1[front1++];
        while (remainTime[PCBIndex] > 0) {
            int runTime;
            if (remainTime[PCBIndex] < timeSlice) {
                runTime = remainTime[PCBIndex];
            } else {
                runTime = timeSlice;
            }

            remainTime[PCBIndex] -= runTime;
            TotalTime += runTime;

            if (remainTime[PCBIndex] > 0) {
                queue1[rear1++] = PCBIndex; // 끝나지 않았으면 다시 뒤로
            } else {
                queue[PCBIndex]->turn_aroundTime = TotalTime - queue[PCBIndex]->arrivalTime;
                completed++;
            }
        }
    }

    // 큐2 FCFS 방식 처리
    while (front2 < rear2) {
        PCBIndex = queue2[front2++];
        TotalTime += remainTime[PCBIndex];
        remainTime[PCBIndex] = 0;
        queue[PCBIndex]->turn_aroundTime = TotalTime - queue[PCBIndex]->arrivalTime;
        completed++;
    }

    // 대기시간 계산
    for (int i = 0; i < input; i++) {
        queue[i]->waitTime = queue[i]->turn_aroundTime - queue[i]->burstTime;
    }

    PrintResult("MLQ", queue, input);
}

void MLFQ(int input) {
    int remainTime[20];
    int time =  0;
    int completed = 0;
    bool isAdded[20] = {false};
    int queue1[20], front1 = 0, rear1 = 0;
    int queue2[20], front2 = 0, rear2 = 0;
    int queue3[20], front3 = 0, rear3 = 0;

    for (int i = 0; i < input; i++) {
        remainTime[i] = queue[i]->burstTime;
    }

    while (completed < input) {
        for (int i = 0; i < input; i++) {
            if (!isAdded[i] && queue[i]->arrivalTime <= time) {
                if (queue[i]->priority == 1) {
                    queue1[rear1++] = i;
                }
                else if (queue[i]->priority == 2) {
                    queue2[rear2++] = i;
                }
                else{
                    queue3[rear3++] = i;
                }
                isAdded[i] = true;
            }
        }
        //우선순위 별 큐 삽입
        int PCBIndex = -1;
        int timeSlice = 0;
        int levelflag = 0;

        if (front1 < rear1) {
            PCBIndex = queue1[front1++];
            if (remainTime[PCBIndex] < 2) {
                timeSlice = remainTime[PCBIndex];
            }
            else {
                timeSlice = 2;
            }
            levelflag = 1;
        }
        else if (front2 < rear2) {
            PCBIndex = queue2[front2++];
            if (remainTime[PCBIndex] < 2) {
                timeSlice = remainTime[PCBIndex];
            }
            else {
                timeSlice = 2;
            }
            levelflag = 2;
        }
        else if (front3 < rear3) {
            PCBIndex = queue3[front3++];
            if (remainTime[PCBIndex] < 2) {
                timeSlice = remainTime[PCBIndex];
            }
            else {
                timeSlice = 2;
            }
            levelflag = 3;
        }

        remainTime[PCBIndex] -= timeSlice;
        time += timeSlice;

        if (remainTime[PCBIndex] > 0) {
            if (levelflag == 1) {
                queue2[rear2++] = PCBIndex;
            }
            else {
                queue3[rear3++] = PCBIndex;
            }
        }
        else {
            queue[PCBIndex]->turn_aroundTime = time - queue[PCBIndex]->arrivalTime;
            completed++;
        }
    }

    for (int i = 0; i < input; i++) {
        queue[i]->waitTime = queue[i]->turn_aroundTime - queue[i]->burstTime;
    }

    PrintResult("MLFQ", queue, input);
}

void CreateProcess(Process *process, int i) { //프로세스 생성 함수
    process->PID = i + 1; //PID
    if (i == 0) {
        process->arrivalTime = 0;            // 첫 프로세스는 무조건 0
    } else {
        process->arrivalTime = rand() % 5 + 1;  // 도착시간 1~5 랜덤
    }
    if (i == 0) {
        process->burstTime = 6; //첫 프로세스 무조건 실행시간 6 안하면 가끔씩 음수가 나오네요
    }else {
        process->burstTime = rand() % 5 + 1; //실행 시간 1~5 랜덤
    }
    process->waitTime = 0; //대기 시간
    process->turn_aroundTime = 0; //반환 시간
    process->priority = rand() % 3 + 1; // 우선 순위
}


int main() {
    int input = 0;
    srand(time(NULL));
    while (1) {
        printf("ENTER NUMBER OF PROCESSES (1 ~ 20) : ");
        scanf("%d",&input);
        if (input > 20 || input <= 0) {
            printf("\n WRONG NUMBER. TRY AGAIN ");
        }
        else break;
    }
    Process PCB[20];
    for (int i = 0; i < input; i++) {
        CreateProcess(&PCB[i],i);
        queue[i] = &PCB[i];
        printf("PID = %d, arrival = %d, burst = %d, priority= %d\n", PCB[i].PID, PCB[i].arrivalTime, PCB[i].burstTime, PCB[i].priority);
    }
    FCFS(input);
    SJF(input);
    RR(input,2);
    HRN(input);
    Priority_Queue(input);
    SRTF(input);
    MLQ(input);
    MLFQ(input);
    return 0;
}