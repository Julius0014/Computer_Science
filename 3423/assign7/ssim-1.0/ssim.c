#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUFFER 1024
#define NumProcess 9
#define writep 1
#define readp  0
#define Seed 5972261
int funct1(int n){
    char buff[BUFFER];
    int i =1,pid,index =0;
    for(i;i <= n;i++){
        switch( pid = fork()){
            case -1:
                perror("FORK ERROR");
                exit(-1);
                break;
            case 0:
                index = i;
                break;
        }
        if(pid == 0){
            break;
        }
    }
    return index;
}
int main(int argv, char *argc[]){
    int fd[NumProcess][2],size;
    for(size = 0; size <= NumProcess;size++){
        if(pipe(fd[size]) == -1 ){
            perror("pipe error");
            exit(-1);
        }
    }

    char buff[BUFFER];
    int index = funct1(NumProcess),i= 1;
    for( i = 0;i < NumProcess;i++){
        if(i != index){
            close(fd[i][readp]);
        }
    }
    int iWrite;
    srand(Seed * index );
    int rng = RAND_MAX/10,j,n,num;
    rng *=10;
    for( n = 0; n < 12; n++){

        do {
            num = rand();
            j = num%10;
        }

        while(num >= rng || index == j);
        size = sprintf(buff,"process%i",index);
     if (iWrite =write(fd[j][writep],buff,size + 1) < 0 ){
            perror("WRITE ERROR");
            exit(-1);
    }
        }

        for( i = 0;i <= NumProcess;i++){
            close(fd[i][writep]);
        }
        char buff2[BUFFER];
        int iread,size2;
        char pMSG[BUFFER];

        while( (iread = read(fd[index][readp],buff2,size + 1)) == size +1 ){
            size2 = sprintf(pMSG,"process%i received a message from %s\n",index, buff2);
            write(STDOUT_FILENO,pMSG, size2);
        }
        if(iread  < 0 ){
            perror("READ ERROR");
            exit(-1);
        }
        close(fd[index][readp]);


    }

