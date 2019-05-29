#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <string.h>
#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {
    int pipe1[2],pipe2[2];
    int i = 0;
    int ch1pid,ch2pid,numread,numrite;
    pid_t parent = getppid();
    pipe(pipe1);
    ch1pid = fork();
    char buffer[1024];
    if(ch1pid == 0){
        if(dup2(pipe1[WRITE],STDOUT_FILENO) == -1){
            perror("DUP 1");
            exit(-1);
        }
        close(pipe1[WRITE]);
        close(pipe1[READ]);
        execl(argv[1],argv[1],NULL);
        perror("EXECL ERROR");
        exit(-1);
        }else if(ch1pid == -1){
        perror("FORK ERORR: ");
        exit(-1);
    }
     pipe(pipe2);
    if(parent == getppid()){
        close(pipe1[1]);
        if (read(pipe1[0],buffer,sizeof(buffer)) == -1){
            perror("READ ERROR");
            exit(-1);
        }
        close(pipe1[0]);
        int len = strlen(buffer);
        while(i != len){
            if( isdigit(buffer[i]) != 0){
                fprintf(stderr,"%d\n",buffer[i]);
            }
            i++;
        }
        if(write(pipe2[WRITE],buffer,len) != len){
            perror("ERORR");
            exit(-1);
        }
        
        close(pipe2[1]);

    }
    ch2pid = fork();
    if(ch2pid == 0){
        if (dup2(pipe2[READ],STDIN_FILENO) == -1){
            perror("ERROR DUP");
            exit(-1);
        }
        close(pipe2[1]);
        close(pipe2[0]);
        close(pipe1[1]);
        close(pipe1[0]);
        
        execl(argv[2],argv[2],NULL);
        perror("EXECL ERROR");
        exit(-1);
        }else if(ch2pid == -1){
        perror("FORK ERORR: ");
        exit(-1);
    }
    return (EXIT_SUCCESS);
}

