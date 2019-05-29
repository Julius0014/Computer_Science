#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct PCB_st {
    int ProcId;
    int ProcPR;
    int CPUburst;
    int Reg[8];
    int queueEnterClock, waitingTime;
    struct PCB_st *next;
    struct PCB_st *plast;
};
int quantum = -1; 
struct PCB_st *min, *max,*temp;
int CPUreg[8] = {0};
struct PCB_st *Head;
struct PCB_st *Tail;
int CLOCK = 0;
int Total_waiting_time = 0;
int Total_turnaround_time = 0;
int Total_job = 0;

char *scheduling;
char *filename;

void UsageErrMsg() {
    printf("Usage ERROR: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n"
            "quantum time must be given with RR\n"
          );
}
/*Check Argument for SCHedling alg*/
void checkarg(int argc, char *argv[]) {
    int i = 1;
    if(argc < 2){
        UsageErrMsg();
        exit(0);
    }
    while (i != argc) {
        if (strcmp(argv[i], "-alg") == 0) {
            i++;
            if (strcmp(argv[i], "SJF") == 0) {
                scheduling = argv[i];
            } else if (strcmp(argv[i], "PR") == 0) {
                scheduling = argv[i];
            } else if (strcmp(argv[i], "RR") == 0) {
                scheduling = argv[i];
            } else if (strcmp(argv[i], "FIFO") == 0) {
                scheduling = argv[i];
            } else {
                UsageErrMsg();
                exit(0);
            }
        }
        if (strcmp(argv[i], "-input") == 0) {
            i++;
            filename = argv[i];
            pfile = fopen(argv[i], "r");
            if (pfile == NULL) {
                perror("INPUT FAILED: ");
                exit(0);
            }
        }
        if(strcmp(argv[i], "-quantum") == 0){
            i++;
            quantum = atoi(argv[i]);
        }
        i++;
    }
}
void updateList(struct PCB_st *Mmin) {

    if (Head == Mmin) {
        Head = Head->next;
        return;
    }
    Mmin->plast->next = Mmin->next;
    return;
}
/*LinkList read input file into linklist*/
void iFillLinkList() {
    int id, pr, burst;
    while (fscanf(pfile, "%d %d %d", &id, &pr, &burst) > 0) {
        insertList(newNode(id, pr, burst));
    }

}
void datacolletionQ(struct PCB_st *pcb) {
    pcb->waitingTime = pcb->waitingTime + CLOCK - pcb->queueEnterClock;
    CLOCK += quantum;
    Total_turnaround_time += CLOCK;
    pcb->CPUburst -= quantum;
    pcb->queueEnterClock = CLOCK;
}
/*Scheduling Alg*/
int SJF_Scheduling() {
    while (Head != NULL) {
        temp = min;
        cpyarry(CPUreg, temp->Reg);
        setReg(CPUreg,CPUreg[0] + 1);
        cpyarry(temp->Reg,CPUreg);
        datacolletion(temp);
        updateList(temp);
        min->CPUburst = 999999;
        updateMAXMIN();
    }
}

int PR_Scheduling() {
    while (Head != NULL) {
        temp = max;
        cpyarry(CPUreg, temp->Reg);
        setReg(CPUreg,CPUreg[0] + 1);
        cpyarry(temp->Reg,CPUreg);
        cpyarry(CPUreg, Head->Reg);    
        datacolletion(temp);
        updateList(temp);
        max->ProcPR = -1;
        updateMAXMIN();
    }

}

int FIFO_Scheduling() {
    while (Head != NULL) {
        cpyarry(CPUreg, Head->Reg);
        setReg(CPUreg,CPUreg[0] + 1);
        cpyarry(Head->Reg,CPUreg);
        temp = Head;
        datacolletion(Head);
        Head = temp->next;
    }
    free(Head);

}
void RR_Scheduling() {
    temp = Head;

    while (Head != NULL) {
        temp = Head;
        cpyarry(CPUreg, Head->Reg);
        setReg(CPUreg,CPUreg[0] + 1);
        cpyarry(Head->Reg,CPUreg);
        if (temp->CPUburst <= quantum) {
            Head = Head->next;
            datacolletion(temp);
        }else{
            datacolletionQ(temp);
            Head = Head->next;
            temp->next = NULL;
            insertList(temp);
        }

    }

}
void scheduler() {
    if (strcmp(scheduling, "FIFO") == 0) {
        FIFO_Scheduling();
        return;
    }
    if (strcmp(scheduling, "SJF") == 0) {
        SJF_Scheduling();
        return;
    }
    if (strcmp(scheduling, "PR") == 0) {
        PR_Scheduling();
        return;
    }
    if (strcmp(scheduling, "RR") == 0) {
        if (quantum == -1) {
            printf("Quantum Time was not set\n");
            UsageErrMsg();
            exit(-1);
        }
        RR_Scheduling();
        return;
    }
}
