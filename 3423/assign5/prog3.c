#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define Buffsz 1024
int main(int argc,char *argv[]){
    int iFileIn,nByteRead,i = 0,place=0;
    char buff[Buffsz],str[Buffsz];
    iFileIn = open(argv[1],O_RDONLY,0);
    if(iFileIn == -1){
        perror(argv[1]);
        exit(-1);
    }

    while( (nByteRead = read(iFileIn,buff,Buffsz)) > 0){
        for( i = 0; i < nByteRead; i++){
            if(buff[i] != '\n'){
                str[place] = buff[i];
                
        //printf("NEXT CHAR %c\n",str[place]);
                place++;
            }

            if(buff[i] == '\n'){
                str[place] = buff[i];
                str[++place] = '\n';
                str[++place] = '\0';
                place = 0;
        printf("%s",str);
        if(( write(STDOUT_FILENO,str,sizeof(str))) == -1 ){
            perror("WRITE ERROR");
            exit(-1);

        }
            }

        }
    }
}
