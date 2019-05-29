#include <unistd.h>
#include <stdio.h>
#include<string.h>  
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "logprt.h"
#define Buffsz 24

int openFileR( char *cFileName){
    int iFileNum;
    if((iFileNum = open(cFileName,O_RDONLY,0)) < 0){
        perror(cFileName);
        exit(0);
    }
    return iFileNum;  
}
int openFileWC(char *cFileName){
    int iFileNum;
    if((iFileNum = open(cFileName, O_CREAT|O_RDWR,S_IRUSR)) < 0){
        perror(cFileName);
        exit(0);
    }
    return iFileNum;  
}
int iGrabPacketHeader(int iFileIn,int iFileOut){
    int iRead,iWrite,i=0;
    static int firsttime = -1;
    struct    pcap_file_header temp;
    struct  my_pkthdr mytemp;
    int size = sizeof(struct my_pkthdr);
    static unsigned int b_sec,c_sec;
    int b_usec,c_usec;
    read(iFileIn,&temp,Buffsz);
    printf("Version major number = %i\n",temp.version_major); 
    printf("Version minor number = %i\n",temp.version_minor); 
    printf("GMT to local correction = %i\n", temp.thiszone);
    printf("Timestamp accuracy = %i\n",temp.sigfigs);
    printf("Snaplen = %i\n",temp.snaplen);
    printf("Linktype = %i\n\n",temp.linktype);

    while( (iRead =read(iFileIn,&mytemp,sizeof(struct my_pkthdr))) == size ){

        if(firsttime){
            firsttime = 0;
            b_sec = mytemp.ts.tv_sec;
            b_usec = mytemp.ts.tv_usec;
        }
        c_sec = (unsigned)mytemp.ts.tv_sec - b_sec;
        c_usec = (unsigned)mytemp.ts.tv_usec - b_usec;
        while(c_usec < 0){
            c_usec += 1000000;
            c_sec--;
        }
        printf("Packet %i\n",i++);
        printf("%05u.%06u\n",(unsigned)c_sec,(unsigned)c_usec);
        printf("Captured Packet Length = %i\n",mytemp.caplen);
        printf("Actual Packet Length = %i\n\n",mytemp.len);
        char buff[mytemp.len+1];
        iRead = read(iFileIn,buff,mytemp.len); 

    }

}
int closeL(int iFileIn){
    int close_err=close(iFileIn);  
    if(close_err==-1)  
    {  
        printf("The file cannot be closed\n");  
    }
}
int iCheckArg(int argc,char *argv[]){
    if(argc < 2){
        printf("Too few Arguments\nExample: %s File1\n",argv[0]);
        exit(0);
    }
}
int main(int argc,char *argv[] ){
    iCheckArg(argc,argv);
    int iFile1 = openFileR(argv[1]);
    int iFileOut = 1;
    iGrabPacketHeader(iFile1,iFileOut);
    closeL(iFile1);
    exit(0);
}
