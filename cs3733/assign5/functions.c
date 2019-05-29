#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
typedef struct PCB { 
    int PR; 
    int numCPUBurst, numIOBurst; 
    int *CPUBurst, *IOBurst;
    int cpuindex, ioindex;
    struct PCB *prev, *next;
}PCB;
/*CPU Struct*/
sem_t cpuWait,ioWait;
 int cpu;
 int io;
 int quantum; 
 struct PCB *min, *max,*temp;
 int CPUreg[8];
 struct PCB *Head;
 struct PCB *IOHead;
 struct PCB *IOTail;
 struct PCB *Tail;
 int CLOCK;
 int Total_waiting_time;
 int Total_turnaround_time;
 int Total_job;
 char *scheduling;
 char *filename;
/*Error MESSAGE*/

struct PCB *insertList(struct PCB *node,int IOBurst); 
void updateList(struct PCB *Mmin); 


void UsageErrMsg() {
    printf("Usage ERROR: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n"
            "quantum time must be given with RR\n"
            );
}
struct PCB *updatetail(int IOBurst){
    if(IOBurst == 1){
    temp = IOHead;
    }else{

       temp = Head; 
    }

    while(temp != NULL){
        if(temp->next == NULL)
            return temp;
          temp =  temp->next; 
    }
}
/*LinkList Create a node*/
void fillarray(int cpuburst[],int ioBurst[],char *ints,int size){
    int i = 0,j = 0,k = 0;
    for(i = 0;i < size;i++){
        if(i%2 == 1){
            ioBurst[j++] = atoi(strtok_r(ints," ",&ints));
       //     printf("io[%i] = %i\n",j-1,ioBurst[j-1]);
        }else{
            cpuburst[k++] = atoi(strtok_r(ints," ",&ints));
         //   printf("Cpu[%i] = %i\n",k-1,cpuburst[k-1]);
        }
    }
}
struct PCB*newNode(int pr, int size, char *ints){
    struct PCB *node;
    node = malloc(sizeof(struct PCB));
    node->PR = pr; 
    node->numCPUBurst = (size/2) + 1;
    node->numIOBurst= size/2; 
    node->CPUBurst = malloc(sizeof(int) * node->numCPUBurst);
    node->IOBurst = malloc(sizeof(int) * node->numIOBurst);
    node->cpuindex = 0; 
    node->ioindex = 0;
    fillarray(node->CPUBurst,node->IOBurst,ints,size);
    Head = insertList(node,0);
    Tail = updatetail(0);
    return Head;

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
        }
        if(strcmp(argv[i], "-quantum") == 0){
            i++;
            quantum = atoi(argv[i]);
        }
        i++;
    }

}
/*LinkList  FIND MIN and MAX of CPUs*/
void updateMAXMIN() {
    temp = Head;
    while (temp != NULL) {
      /*  if (min->CPUburst > temp->CPUburst)
            min = temp;
        if (max->ProcPR < temp->ProcPR) {
            max = temp;
        }
        temp = temp->next;
        */
    }
    //free(temp);
}
/*LinkList insert a node*/
struct PCB *insertList(struct PCB *node,int IOBurst) {
    struct PCB *inHead,*inTail;
    if(IOBurst == 1){
        inHead = IOHead;
        inTail = IOTail;
    }else{
        inHead = Head;
        inTail = Tail;
    }

    if (inHead == NULL) {
        printf("HEad NULL PR = %i:%i\n",node->PR,IOBurst);
        inHead = node;
        return inHead;
    }
    if(inTail != NULL){
    inTail->next = node;
    node->prev = inTail;
    inTail = node;
    }else{
        printf("tail null\n");
    }
    return inHead;
}
/*LinkList update the list */
void updateList(struct PCB *Mmin) {

    if (Head == Mmin) {
        Head = Head->next;
        return;
    }
    Mmin->prev->next = Mmin->next;
    return;
}
/*LinkList read input file into linklist*/

int SJF_Scheduling() {
}

int PR_Scheduling() {

}

int FIFO_Scheduling(int IOBurst) {
    int sleep;
    if (Head == NULL){ 
        printf("CPU HEAD NULL\n");
        return -1;
    }
    struct PCB *temp;
    temp = Head;
    
    Head = Head->next;
    //Head->prev = NULL;

    temp->next = NULL;
    temp->prev = NULL;

    sleep =  temp->CPUBurst[temp->cpuindex];
    printf("Proc = %i index = %i CPU Sleep = %i NUmBurst = %d\n",temp->PR,temp->cpuindex,sleep,temp->numCPUBurst);
    temp->cpuindex++;
    usleep(sleep * 1000);
    //zero indexing
    printf("index = %i > numburst = %i\n",temp->cpuindex,temp->numCPUBurst);
    if(temp->cpuindex >= temp->numCPUBurst ){
        printf("Free Proc %i\n",temp->PR);
        free(temp->CPUBurst);
        free(temp->IOBurst);
        free(temp);
        return -1;
    }
        printf("PR = %i BACK IN IO LIST\n",temp->PR);
        IOHead = insertList(temp,1);
        IOTail = updatetail(1);
    return 1;
}

int RR_Scheduling() {
     
}
int IOSleep(){
    if(IOHead == NULL){
        printf("IOHEad NULL\n");
        return 0;
    }
    struct PCB *PCB;
    PCB = IOHead;
    IOHead = IOHead->next;
    //IO:Head->prev  = NULL;
    PCB->next = NULL;
    PCB->prev = NULL;
    int sleep = PCB->IOBurst[PCB->ioindex++];
    printf("Proc = %i index = %i IO Sleep = %i\n",PCB->PR,PCB->ioindex-1,sleep);
    usleep(sleep * 1000);
    printf("PR = %d BACK IN QLIST\n",temp->PR);
    Head = insertList(PCB,0);
    Tail = updatetail(0);
    return 1;

}
/*chose the right alg to run*/
int scheduler(int IOBurst) {
    if(IOBurst == 1){
       return IOSleep();
    }
    if (strcmp(scheduling, "FIFO") == 0) {
        return FIFO_Scheduling(IOBurst);
    }
    if (strcmp(scheduling, "SJF") == 0) {
       return SJF_Scheduling();
    
    }
    if (strcmp(scheduling, "PR") == 0) {
        return PR_Scheduling();
        
    }
    if (strcmp(scheduling, "RR") == 0) {
        if (quantum == -1) {
            printf("Quantum Time was not set\n");
            UsageErrMsg();
            exit(-1);
        }
        return RR_Scheduling();
    }
}

