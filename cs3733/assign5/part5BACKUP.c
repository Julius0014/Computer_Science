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
int Total_job = 0;;
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
/*LinkList insert a node*/
struct PCB *insertList(struct PCB *node,struct PCB *inHead,struct PCB *inTail) {
    char *IOburst;
    if(inHead == Head){
        IOburst = "Head";
    }else{

        IOburst = "IOHead";
    }
    if (inHead == NULL) {
        printf("HEad NULL PR = %i  in %s \n",node->PR,IOburst);
        inHead = node;
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

void datacollection(struct PCB *temp,int i) {
    temp->waitingTime = temp->waitingTime + CLOCK - temp->queueEnterClock;
    if(i == 1)
    Total_waiting_time += temp->IOBurst[temp->ioindex -1];
    CLOCK += temp->CPUBurst[temp->cpuindex-1];
    Total_turnaround_time += CLOCK;
    if(i == 0)
    Total_job++;
}
int FIFO_Scheduling(int IOBurst) {
    int sleep;
    if (Head == NULL){ 
        return -1;
    }

    struct PCB *temp;
    temp = Head;
    Head = Head->next;
    temp->next = NULL;
    temp->prev = NULL;

    sleep =  temp->CPUBurst[temp->cpuindex];
    printf("Proc = %i index = %i CPU Sleep = %i NUmBurst = %d\n",temp->PR,temp->cpuindex,sleep,temp->numCPUBurst);
    temp->cpuindex++;
    datacollection(temp,0);
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
    IOHead = insertList(temp,IOHead,IOTail);
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
    PCB->next = NULL;
    PCB->prev = NULL;
    datacollection(PCB,1);
    int sleep = PCB->IOBurst[PCB->ioindex++];
    printf("Proc = %i index = %i IO Sleep = %i\n",PCB->PR,PCB->ioindex-1,sleep);
    usleep(sleep * 1000);
    printf("PR = %d BACK IN QLIST\n",PCB->PR);
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
    printf("Read Thread\n");
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
            printf("proc\n");
            pr = atoi(strtok_r(rest," ",&rest));
            printf("pr = %i\n",pr);
            listsize = atoi(strtok_r(rest," ",&rest));
            newNode(pr,listsize,rest);
            sem_post(&cpuWait);

        } 
        if(strcmp(str,"sleep") == 0 ){
            printf("sleep\n");
            usleep(atoi(strtok_r(rest," ",&rest)) * 1000);
            printf("woke\n");
        }
        if(strcmp(str,"stop\n") == 0 || strcmp(str,"stop") == 0 ){
            printf("Stop\n");
            ifile = 1;
            break;
        }
    }
    return 0;
}
void printlist(struct PCB* Head){
    while (Head != NULL){
        printf("%i->",Head->PR);
        Head =  Head->next;
    }
    printf("\n");
}
int IO(){
    while(1){
        if(Head == NULL && IOHead == NULL&& ifile == 1 && cpu == 0){
            printf("IO2\n");
            break;
        }
        int x = sem_timedwait(&ioWait,&sec);
        if(x == -1 && errno==ETIMEDOUT ) continue ;
        printf("IO Started\n");
        printlist(IOHead);
        io = 1;
        pthread_mutex_lock(&lock);
        scheduler(1);
        pthread_mutex_unlock(&lock);
        io = 0;
        printf("IO Called Cpu\n");
        sem_post(&cpuWait);

    }
}
int CPUThread(){
    int r,i =0;
    while(1){
        if(Head == NULL && IOHead == NULL&& ifile == 1 && io == 0 ){
            printf("CPU Break\n");
            break;
        }

        r = sem_timedwait(&cpuWait,&sec);
        if(r == -1 && errno==ETIMEDOUT ) continue ;
        cpu = 1;

        printf("CPU Started\n");
        printlist(Head);
        pthread_mutex_lock(&lock);
        r =  scheduler(0);
        pthread_mutex_unlock(&lock);
        if (r == 1){
            printf("Call IO\n");
            sem_post(&ioWait);
        }

        printf("CPU END\n");
        cpu = 0;
        //printlist(Head);
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
    printf("Input File Name: %s\n", filename);
    printf("CPU Scheduling Alg : %s\n", scheduling);
    printf("Average Waiting time in R queue = %.2f ms\n", (double) Total_waiting_time / Total_job);
    printf("Average Turnaround time = %.2f ms\n", (double) Total_turnaround_time / Total_job);
    printf("Throughput = %.2f jobs per ms\n", (double) Total_job / CLOCK);
    return (EXIT_SUCCESS);

}




