#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
struct cmd
{
    int redirect_in;     /* Any stdin redirection?         */
    int redirect_out;    /* Any stdout redirection?        */
    int redirect_append; /* Append stdout redirection?     */
    int background;      /* Put process in background?     */
    int piping;          /* Pipe prog1 into prog2?         */
    char *infile;        /* Name of stdin redirect file    */
    char *outfile;       /* Name of stdout redirect file   */
    char *argv1[10];     /* First program to execute       */
    char *argv2[10];     /* Second program in pipe         */
};
//execvp(command.argv1[0],command.argv1);
int reDirOut(char *outfile){
    int fdout = open(outfile,O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fdout < 0){
        perror("Failed to Open OutFile\n");
        exit(-1);
    }
    dup2(fdout,STDOUT_FILENO);
    close(fdout);
}
int reDirIn(char *infile){
    int fdin = open(infile,O_RDONLY);
    if(fdin < 0){
        perror("Failed to Open InFile\n");
        exit(-1);
    }
    dup2(fdin,STDIN_FILENO);
    close(fdin);
}
int reDirApp(char *outfile){
    int fdout = open(outfile,O_WRONLY|O_APPEND,0644);
    if(fdout < 0){
        perror("Failed to Open File for Appending\n");
        exit(-1);
    }
    dup2(fdout,STDOUT_FILENO);
    close(fdout);
    return; 
}

int rePDirOut(char *outfile){
    int fdout = open(outfile,O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fdout < 0){
        perror("Failed to Open OutFile\n");
        exit(-1);
    }
    return fdout;
}
int pipefork(struct cmd command){
    int fd[2];
    switch(fork()){
        case 0:
            if (pipe(fd) == -1){ 
            perror("PIPE ERROR");
            exit(-1);
            }
            switch(fork()){
                case -1:
                    perror("FORK ERROR\n");
                    exit(-1);
                case 0:
                    dup2(fd[1],STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    execvp(command.argv1[0],command.argv1);
                    perror("EXEC ERROR");
                    exit(-1);
                default:
                    if(command.redirect_out == 1){
                        if(command.redirect_append == 1){
                            reDirApp(command.outfile);
                        }else{
                            reDirOut(command.outfile);
                        }
                    }
                    if(command.redirect_in == 1){
                        reDirIn(command.infile);
                    }
                    dup2(fd[0],STDIN_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    execvp(command.argv2[0],command.argv2);
                    perror("EXEC ERROR");
                    exit(-1);
            }
            break;
        default:
            if(command.background == 1)exit(0);
            wait(0);
            break;
    }
}
int noPipe(struct cmd command){
    int status =0;
    pid_t wid;
    switch(fork()){
        case -1:
            perror("FORK ERROR\n");
            exit(-1);
        case 0:
            if(command.redirect_out == 1){
                if(command.redirect_append == 1){
                    reDirApp(command.outfile);
                }else{
                    reDirOut(command.outfile);
                }
            }
            if(command.redirect_in == 1){
                reDirIn(command.infile);
            }
            execvp(command.argv1[0],command.argv1);
            perror("EXEC ERROR");
            exit(-1);
            break;
        default:

            if(command.background == 1)exit(0);
            wait(0);
            break;
    }
}
int runBG(struct cmd command){
    switch(fork()){
        case -1:
            perror("FORK ERROR\n");
            exit(-1);
        case 0:
            if(command.piping == 1){
                pipefork(command);
            }else{
                noPipe(command);
            }
            break;
        default:
            break;
    }
}
int main(){
    char buff[1024];
    struct cmd command;
    int i =0,fdin,fdout;
    while((gets(buff) != NULL )){
        if( cmdscan(buff,&command)){
            printf("ILLEGAL FORMAT: \n");
            continue;
        }
        if(strcmp(buff,"exit") == 0) exit(0);
        if(command.background == 1){
            runBG(command);
        }else if(command.piping == 1){
            pipefork(command);
        }else{
            noPipe(command);
        }

    }

    exit(0);
}
