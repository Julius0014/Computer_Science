#include <pthread.h>  
#include <sys/types.h>
#include <semaphore.h>
extern sem_t cpuWait,ioWait;
typedef struct PCB { 
int PR; 
int numCPUBurst, numIOBurst; 
int *CPUBurst, *IOBurst;
int cpuindex, ioindex;
struct PCB *prev, *next;
}PCB;
extern int cpu;
extern int io ;
extern int quantum; 
extern struct PCB *min, *max,*temp;
extern int CPUreg[8];
extern struct PCB *Head;
extern struct PCB *IOHead;
extern struct PCB *IOTail;
extern struct PCB *Tail;
extern int CLOCK;
extern int Total_waiting_time;
extern int Total_turnaround_time;
extern int Total_job;
extern char *scheduling;
extern char *filename;
void UsageErrMsg();
int makeArray(int size);
struct PCB*newNode(int pr, int size, char *ints);
void checkarg(int argc, char *argv[]);
void updateMAXMIN();
struct PCB *insertList(struct PCB *node,struct PCB *inHead,struct PCB *inTail);
void updateList(struct PCB *Mmin);
