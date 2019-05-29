#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define Buffsz 1024
struct product{
    unsigned int type;
    unsigned  int number;
} p ;
int main(int argc,char *argv[]){
    int iFileIn,nByteRead,i = 0;
    unsigned int place;

    char buff[Buffsz],str[Buffsz];
    iFileIn = open(argv[1],O_RDONLY,0);
    int size = sizeof(p);
    while( (nByteRead = read(iFileIn,&p,size))  == size){
        printf("Product\t%hu\tQuantity\t%hu\n",p.type,p.number);
        for( i = 0; i < p.number;i++){
            read(iFileIn,&place,sizeof(place));
            printf("\t%10u\n",place);
        }
        printf("\n");
    } 
    if( nByteRead > 0){
        printf("Error: Incomplete Record\n");
    }


}   
