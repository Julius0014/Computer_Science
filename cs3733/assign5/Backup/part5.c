#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "functions.h" 
#include <time.h>
#define MAX 1024
sem_t cpuWait,ioWait;
int io = 1;
struct PCB *Head,*Tail,*IOHead,*IOTail;
int FileReadThread(char *file){
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
            pr = atoi(strtok_r(rest," ",&rest));
            listsize = atoi(strtok_r(rest," ",&rest));
              newNode(pr,listsize,rest);
              sem_post(&cpuWait);
        } 
        if(strcmp(str,"sleep") == 0 ){
            printf("sleep\n");
            usleep(atoi(strtok_r(rest," ",&rest)) * 1000);
        }
        if(strcmp(str,"stop\n") == 0 || strcmp(str,"stop") == 0 ){
            printf("Stop\n");
            break;
        }
    }
    return 0;
}
void printlist(){
    while (Head != NULL){
        printf("%i->",Head->PR);
        Head =  Head->next;
    }
    printf("\n");
}
int IO(){
    sem_wait(&ioWait);
    printf("IO\n");
    scheduler(0);
    sem_post(&cpuWait);
}
int CPUThread(){
    while(Head != NULL){
    sem_wait(&cpuWait);
    printf("CPU\n"); 
    sleep(1);
    scheduler(0);
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
    pthread_create(&mythread[0], NULL, (void *) FileThread, NULL);
    pthread_create(&mythread[1], NULL, (void *) CPUThread, NULL);
    pthread_create(&mythread[2], NULL,(void *) IO, NULL);
    pthread_join(mythread[0],NULL);
    pthread_join(mythread[1],NULL);
    pthread_join(mythread[2],NULL);
    printlist();
    Head = IOHead;
    printlist();
    return 0;
}




