#include <unistd.h>
#include <stdio.h>
#include<string.h>  
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define Buffsz 1024

int openFileR( char *cFileName){
    int iFileNum;
    if(strcmp(cFileName,"-")==0) return 0;
    if((iFileNum = open(cFileName,O_RDONLY,0)) < 0){
        perror(cFileName);
        exit(0);
    }
    return iFileNum;  
}
int openFileWC(char *cFileName){
    if(cFileName =='\0'){
        return 1; 
    }
    if(strcmp(cFileName,"-")==0) {return 0;}

    int iFileNum;
    if((iFileNum = open(cFileName, O_CREAT|O_RDWR,S_IRUSR)) < 0){
        perror(cFileName);
        exit(0);
    }
    return iFileNum;  
}
int iWriteFile(int iFileIn,int iFileOut){
    int iRead,iWrite;
    char buff[Buffsz];
    while((iRead = read(iFileIn,buff,Buffsz)) > 0 ){

        if(iRead != (iWrite = write(iFileOut,buff,iRead))){
            printf("WRITE ERROR\n");
            exit(0);
        }

    }
    if(iRead < 0) {
        printf("Read ERROR\n");
        exit(0);
    }
    closeL(iFileIn);

}
int closeL(int iFileIn){
    int close_err=close(iFileIn);  
    if(close_err==-1)  
    {  
        printf("The file cannot be closed\n");  
    }

}
int iCheckArg(int argc,char *argv[]){

    if(argc < 3){
        printf("Too few Arguments\nExample: ckit File1.in File2.in File3.out or ckit File1.in File2.in\n");
        exit(0);
    }
    int i=0,count =0;
    while(i<argc){
        if(strcmp(argv[i],"-" ) == 0)count++;
        if(count == 2){
            printf("Too many Commands for Standard In\n");
            exit(0);
        }
        i++;
    }
}
int main(int argc,char *argv[] ){
    iCheckArg(argc,argv);
    int iFile1 = openFileR(argv[1]);
    int iFile2 = openFileR(argv[2]);
    int iFileOut = openFileWC(argv[3]);
    iWriteFile(iFile1,iFileOut);
    iWriteFile(iFile2,iFileOut);
    closeL(iFileOut);

}
