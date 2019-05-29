#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <string.h>
//global varable for pagetable and files
int infile,outfile;
unsigned long pagetable[10] = {2,4,1,7,3,5,6};
//usage error check
void checkarg(int argc,char* argv[] ){
    if(argc - 1 < 2){
        printf("USAGE ERROR: Example part1 infile outfile\n");
        exit(-1);
    }
    if((infile = open(argv[1], O_RDONLY)) == -1 ){
        perror("infile ERROR : ");
        exit(-1);
    }
    if((outfile = open(argv[2], O_CREAT|O_TRUNC | O_WRONLY)) == -1 ){
        perror("outfile ERROR :");
        exit(-1);
    }
}
// convert logical address to physical address; 
unsigned long convPA(unsigned long la){
    unsigned long d_num,f_num,p_num;
    p_num = la >> 7;
    d_num = la & 0x7F;
    f_num = pagetable[p_num];
    return (f_num << 7) + d_num;
}
/*read values from file and write */
int fillpagetable(unsigned long pagetable[]){
    int readbytes;
    unsigned long buf,pa;
    while( (readbytes = read(infile,&buf,sizeof(buf))) > 0 ){
        pa = convPA(buf);
        if( write(outfile,&pa,sizeof(unsigned long)) < 0){
            perror("WRITE ERROR");
            exit(-1);
        }
    }
    return ;
}
int main(int argc, char* argv[]) {
    checkarg(argc,argv);
    fillpagetable(pagetable);
}

