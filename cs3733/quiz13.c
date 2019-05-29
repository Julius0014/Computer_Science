#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h> 
#include <math.h>
typedef struct temp{
    int a;
    double b;
} temp;
double balance = 0.0;
void *withdraw(void *args){
    int i;
    temp *t;
    t = (temp *) args; 
    for(i=0; i < t->a;i++) balance -=t->b;
    return NULL;
}
void *deposit(void *args){
    int i;
    temp *t;
    t = (temp *) args; 
    for(i=0; i < t->a;i++) balance +=t->b;
    return NULL;
}
int main(int argc,char *argv[]){
    temp b,c;
    b.a = atoi(argv[1]);
    b.b = atof(argv[2]);
    c.a = atoi(argv[3]);
    c.b = atof(argv[4]);
    pthread_t thread_id,thread_id2; 
    pthread_create(&thread_id,NULL,deposit,(void *) &b); 
    pthread_create(&thread_id,NULL,withdraw,(void *) &c);
    pthread_join(thread_id, NULL);
    pthread_join(thread_id2, NULL);
    double balance2 = (b.a * b.b) - (c.a * c.b);
    if(balance == balance2)
        printf("equal\n");
    printf("Balance = %.2lf\n",balance);
    printf("Thread Balance  = %.2lf\n",balance2);
}
