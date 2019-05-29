#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#define MAX 1024
sem_t cpuWait,ioWait;
typedef struct PCB { 
    int PR; 
    int numCPUBurst, numIOBurst; 
    int *CPUBurst, *IOBurst;
    int cpuindex, ioindex;
    int queueEnterClock;
    int waitingTime;
    struct PCB *prev, *next;
}PCB;
sem_t cpuWait,ioWait;
int cpu = 0,CLOCK;
int io = 0;
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
int Total_job = 0,setp = 0;
char *scheduling;
char *filename;
/*Error MESSAGE*/
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
int iosum = 0;
int cpusum = 0;
/*LinkList Create a node*/
void fillarray(int cpuburst[],int ioBurst[],char *ints,int size){
    int i = 0,j = 0,k = 0;
    for(i = 0;i < size;i++){
        if(i%2 == 1){
            ioBurst[j++] = atoi(strtok_r(ints," ",&ints));
            iosum+= ioBurst[j-1];
        }else{
            cpuburst[k++] = atoi(strtok_r(ints," ",&ints));
            cpusum+= cpuburst[k-1];
        }
    }
}
/*LinkList insert a node*/
struct PCB *insertList(struct PCB *node,struct PCB *inHead,struct PCB *inTail) {
    char *IOburst;
    if(setp == 0){
        min = inHead;
        max = inHead;
    }
    if(inHead == Head){
        IOburst = "Head";
    }else{

        IOburst = "IOHead";
    }
    if (inHead == NULL) {
        inHead = node;
    if(setp == 0){
        min = inHead;
        max = inHead;
    }
        inTail = node;
        return inHead;
    }else{
        struct PCB* temp;
        temp = inHead;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = node;


    }
    return inHead;
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
    node->queueEnterClock = 0;
    node->waitingTime = 0;
    fillarray(node->CPUBurst,node->IOBurst,ints,size);
    Head = insertList(node,Head,Tail);
    setp++;
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
    struct PCB *temp ;
    temp = Head;
    if (min == NULL);
    while (temp != NULL) {
        printf("InList\n");
          if (min->CPUBurst[min->cpuindex] > temp->CPUBurst[temp->cpuindex]) 
            min = temp;
            if (max->PR < temp->PR) {
            max = temp;
            }
            temp = temp->next;
        
    }
}
/*LinkList insert a node*/

/*LinkList update the list */
void updateList(struct PCB *Mmin) {
    if (Head == Mmin) {
        Head = Head->next;
        return;
    }
    if(Mmin->prev != NULL) 
    Mmin->prev->next = Mmin->next;
    if(Mmin->next != NULL)
    //Mmin->next->prev = Mmin->prev;
    return;
}
/*LinkList read input file into linklist*/
int SJF_Scheduling() {
  updateList(min);
   updateMAXMIN();
   int x = handlenode(min);
   updateMAXMIN();
  return x;


}

int PR_Scheduling() {

}

void datacollection(struct PCB *temp,int i) {
    temp->waitingTime = temp->waitingTime + CLOCK - temp->queueEnterClock;
    if(i == 1)
    Total_waiting_time +=  CLOCK + temp->IOBurst[temp->ioindex -1];
    CLOCK += temp->CPUBurst[temp->cpuindex-1];
    Total_turnaround_time += CLOCK;
}
int handlenode(struct PCB *temp){
    int sleep;
    Total_job++;
    temp->next = NULL;
    temp->prev = NULL;
    sleep =  temp->CPUBurst[temp->cpuindex];
    CLOCK+= sleep;
    temp->cpuindex++;
    datacollection(temp,0);
    usleep(sleep * 1000);
    if(temp->cpuindex >= temp->numCPUBurst ){
        if(temp==min)
            min = Head; 
        free(temp->CPUBurst);
        free(temp->IOBurst);
        free(temp);
        return -1;
    }
    IOHead = insertList(temp,IOHead,IOTail);
    return 1;
}
int FIFO_Scheduling(int IOBurst) {
    if (Head == NULL){ 
        return -1;
    }
    temp = Head;
    Head = Head->next;
    return handlenode(temp);
}

int RR_Scheduling() {

}
int IOSleep(){
    if(IOHead == NULL){
        return 0;
    }
    struct PCB *PCB;
    PCB = IOHead;
    IOHead = IOHead->next;
    PCB->next = NULL;
    PCB->prev = NULL;
    datacollection(PCB,1);
    int sleep = PCB->IOBurst[PCB->ioindex++];
    usleep(sleep * 1000);
    Head = insertList(PCB,Head,Tail);
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






pthread_mutex_t lock,lock2; 
int ifile =0;
struct PCB *Head,*Tail,*IOHead,*IOTail;
struct timespec sec;
int FileReadThread(char *file){
    sec.tv_sec = 1;
    char buffer[MAX];
    FILE *open;
    char *str;
    int i =0;
    int pr,listsize;
    open = fopen(file,"r");
    if( open == NULL){
        perror("Failed to Open file");
        exit(0);
    }

    while(fgets(buffer,sizeof(buffer),open) ){
        char *rest = buffer;
        str = strtok_r(rest," ",&rest);
        if( strcmp(str,"proc") == 0){
            pr = atoi(strtok_r(rest," ",&rest));
            listsize = atoi(strtok_r(rest," ",&rest));
            newNode(pr,listsize,rest);
            sem_post(&cpuWait);

        } 
        if(strcmp(str,"sleep") == 0 ){
            usleep(atoi(strtok_r(rest," ",&rest)) * 1000);
        }
        if(strcmp(str,"stop\n") == 0 || strcmp(str,"stop") == 0 ){
            ifile = 1;
            break;
        }
    }
    return 0;
}
int IO(){
    while(1){
        if(Head == NULL && IOHead == NULL&& ifile == 1 && cpu == 0){
            break;
        }
        int x = sem_timedwait(&ioWait,&sec);
        if(x == -1 && errno==ETIMEDOUT ) continue ;
        io = 1;
        pthread_mutex_lock(&lock);
        scheduler(1);
        pthread_mutex_unlock(&lock);
        io = 0;
        sem_post(&cpuWait);

    }
}
int CPUThread(){
    int r,i =0;
    while(1){
        if(Head == NULL && IOHead == NULL&& ifile == 1 && io == 0 ){
            break;
        }

        r = sem_timedwait(&cpuWait,&sec);
        if(r == -1 && errno==ETIMEDOUT ) continue ;
        cpu = 1;

        pthread_mutex_lock(&lock);
        r =  scheduler(0);
        pthread_mutex_unlock(&lock);
        if (r == 1){
            sem_post(&ioWait);
        }

        cpu = 0;
    }
}
void FileThread(){

    FileReadThread(filename);
}
int main(int argc,char *argv[]) 
{
    checkarg(argc,argv);
    sem_init(&cpuWait,0,0);
    sem_init(&ioWait,0,0);
    pthread_t mythread[3];
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_create(&mythread[0], NULL, (void *) FileThread, NULL);
    pthread_create(&mythread[1], NULL, (void *) CPUThread, NULL);
    pthread_create(&mythread[2], NULL,(void *) IO, NULL);
    pthread_join(mythread[0],NULL);
    pthread_join(mythread[1],NULL);
    pthread_join(mythread[2],NULL);
    printf("Student Name: Julius Jenkins\n");
    printf("Input File Name   : %s\n", filename);
    printf("CPU Scheduling Alg: %s\n", scheduling);
    printf("CPU utilization   : %.2f%%\n",(double) CLOCK/(CLOCK + iosum)*100);
    printf("Average Waiting time in R queue\t: %.2f ms\n", (double)  iosum / Total_job);
    printf("Average Turnaround time: %.2f ms\n", (double) iosum+cpusum / Total_job);
    printf("Throughput: %.3f jobs per ms\n", (double) Total_job / CLOCK);
    return (EXIT_SUCCESS);

}




