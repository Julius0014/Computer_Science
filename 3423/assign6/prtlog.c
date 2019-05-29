#include <unistd.h>
#include <stdio.h>
#include<string.h>  
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "prtlog.h"
int openFileR( char *cFileName){
    int iFileNum;
    if((iFileNum = open(cFileName,O_RDONLY,0)) < 0){
        perror(cFileName);
        exit(0);
    }
    return iFileNum;  
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
int iGrabPacketHeader(int iFileIn){

    int iRead,iWrite,i=0,firsttime = -1;;
    //char buff;
    struct pcap_file_header temp;
     int32_t b_sec,b_usec,c_sec,c_usec;
    read(iFileIn,&temp,sizeof(struct pcap_file_header));
    printf("Version major number = %i\n",temp.version_major);
    printf("Version minor number = %i\n",temp.version_minor);
    printf("GMT to local correction = %i\n", temp.thiszone);
    printf("Timestamp accuracy = %i\n",temp.sigfigs);
    printf("Snaplen = %i\n",temp.snaplen);
    printf("Linktype = %i\n",temp.linktype);
    struct my_pkthdr mytemp;

    while( (iRead = read(iFileIn,&mytemp,sizeof(struct my_pkthdr ))) ==  sizeof(struct my_pkthdr ) ){

        printf("\nPacket %i\n",i++);
        if(firsttime == -1){
            firsttime = 0;
            b_sec = (mytemp.ts.tv_sec);
            b_usec = (mytemp.ts.tv_usec);
        }
        c_sec = (unsigned)mytemp.ts.tv_sec - b_sec;
        c_usec = (unsigned)mytemp.ts.tv_usec - b_usec;
        while(c_usec < 0){
            c_usec += 1000000;
            c_sec--;
        }
        printf("%05u.%06u\n",(unsigned)c_sec,(unsigned)c_usec);
        printf("Captured Packet Length = %i\n",mytemp.caplen);
        printf("Actual Packet Length = %i\n",mytemp.len);
        char buff[mytemp.len+1];
        if(read(iFileIn,buff, mytemp.len) != mytemp.len)
            exit(-1);
        struct eth_hdr *eTemp ;
        eTemp = (struct eth_hdr * ) buff;
        printf("Ethernet Header\n");
        eHeader(ntohs(eTemp->eth_type),iFileIn,buff);
    }
}
int eHeader(int eTemp, int iFileIn,char *buff){

    switch(eTemp){

        case ETH_TYPE_IP:
            printf("   IP\n");
            struct ip_hdr *s;
            s = (struct ip_hdr *)(buff + sizeof(struct eth_hdr));
            ipHeader(s->ip_p);
            break;

        case ETH_TYPE_ARP:
            printf("   ARP\n");
            struct arp_hdr *s2; 
            s2 = (struct arp_hdr *)(buff + sizeof(struct eth_hdr));
            ARPHeader(htons(s2->ar_op));
            break;
        default:
            break;

    }

}

int  ARPHeader(int s){
    switch(s){
        case 1:
            printf("      arp operation = Arp Request\n");
            break;
        case 2:

            printf("      arp operation = Arp Reply\n");
            break;
        case 3:

            printf("      arp operation = Arp RevRequest\n");
            break;
        case 4:

            printf("      arp operation = Arp RevReplay\n");
            break;
        default:
            break;


    }
}
int  ipHeader(int s){
    switch(s){
        case 1:
            printf("      ICMP\n");
            break;
        case 2:
            printf("      IGMP\n");
            break;
        case 6:
            printf("      TCP\n");
            break;
        case 17:
            printf("      UDP\n");
            break;
        default:
            printf("      UNRECOGNIZED\n");
            break;
    }


}
int main(int argc,char *argv[] ){
    iCheckArg(argc,argv);
    int iFile1 = openFileR(argv[1]);
    int iFileOut = 1;
    struct my_pkthdr *mytemp;
    struct pcap_file_header *temp;
    iGrabPacketHeader(iFile1);
    closeL(iFile1);
    exit(0);
}
