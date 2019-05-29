#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define Buffsz 1024
    struct vector{
        float x;
        float y;
        float z; };
    struct particle{
        float mass;
        struct vector pos;
        struct vector vel;
    };
int main(int argc,char *argv[]){
    int iFileIn,nByteRead,i = 1,place=0;
    char buff[Buffsz],str[Buffsz];
    iFileIn = open(argv[1],O_RDONLY,0);
    struct particle* a;
    a = (struct particle*)malloc(sizeof(struct particle));
    while( (nByteRead = read(iFileIn,a,sizeof(struct particle)))  == sizeof(struct particle)){
            int len = sprintf(buff,"Particle\t%d\n\tmass\t%f\n\tpos\t(%f,%f,%f)\n\tvel\t(%f,%f,%f)\n",
            i,
            a->mass,
            a->pos.x,
            a->pos.y,
            a->pos.z,
            a->vel.x,
            a->vel.y,
            a->vel.z);

            write(STDOUT_FILENO,buff,len);
            i++;

    } 
    if( nByteRead > 0){
         printf("Error: Incomplete Record\n");
    }


}   
