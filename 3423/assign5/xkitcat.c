#include <unistd.h>
#include <stdio.h>
#include<string.h>  
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define Buffsz 4
char *wString;
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
int readbufferNL(char buffer[], int place, int numRead){
    while(1){
        if(buffer[place] == '\n') return place;
        if(place == numRead && buffer[place]  != '\n'){return place;
    
        }else{
            return -1;
        }
    place++;
    }
    return -1;
}
char *CreateString(int iEndplace,int iStartplace,char buff[],int iFileOut,int unFstr){
    int i =0;
    if(buff[iEndplace] != '\n' && unFstr == -1){

        while( iStartplace <= iEndplace){
            wString[i] = buff[iStartplace];
            iStartplace++;
            i++; 
        }
    }else{
        char str[iEndplace - iStartplace];
        while( iStartplace <= iEndplace){
            str[i] = buff[iStartplace];
            iStartplace++;
            i++; 
        }
        str[i] = '\0';
        write(iFileOut,str,strlen(str));
    }
}
int iWriteFile(int iFileIn,int iFileIn2,int iFileOut){
    int iRead,iWrite,iRead2;
    char buff[Buffsz],buff2[Buffsz];
    int istart1= 0,istart2 =0;
    int x ,y,z = 0;
    while((iRead = read(iFileIn,buff,Buffsz)) > 0  && (iRead2 = read(iFileIn2,buff2,Buffsz)) > 0 ){
        while(1){
            if((x = readbufferNL(buff,istart1,iRead)) != -1){
                if(x == iRead){
                    z = -1;

                }else{
                    CreateString(x,istart1,buff,iFileOuti,z);
                    istart1 = x + 1;
                    x = 0;
                }
                if((y = readbufferNL(buff2,istart2,iRead2)) != -1){
                    CreateString(y,istart2,buff2,iFileOut);
                    istart2 = y + 1;
                }
                if(x == -1 && y == -1){
                    break;
                }
            }
        }
        if(iRead < 0) {
            printf("Read ERROR\n");
            exit(0);
        }
        closeL(iFileIn);
        closeL(iFileIn2);
        closeL(iFileOut);

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
        iWriteFile(iFile1,iFile2,iFileOut);
    }
