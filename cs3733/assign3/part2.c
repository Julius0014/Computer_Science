#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <string.h>
//global varable for pagetable and files
typedef struct page{
    int vi;
    int fnum;
}page;
page pagetable[32];
int frame[8];
int infile,outfile;
int count[8],clock = 0,pf = 0;
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
    unsigned long dnum,fnum,pnum;
    pnum = la >> 7;
    dnum = la & 0x7F;
    fnum = pagetable[pnum].fnum;
    return (fnum << 7) + dnum;
}
/*find free frames*/
int fframe(){
    static int index = 1;
    if(index > 8)
        return -1;
    if(frame[index] == 0){
        index++;
        return index -1;

   } else{
        index++;
        return fframe();
    }
}
/*find lowest clock*/
int findlowcost(){
    int min = 1,i=0;
    for(i=1;i < 8;i++){
        if(count[i] < count[min]){
            min = i;
        }
    }
    return min;
}
//add to page table base on valibe bit;
void add2pagetable(unsigned long pa, int pnum,int offset){
    int fnum,pos,y;
    if(pagetable[pnum].vi == 1){
        return;
    }else if( (pos= fframe() ) >= 0 ){
        pf++;
        pagetable[pnum].fnum = pos;
        pagetable[pnum].vi = 1;
        frame[pos] = pnum;        
    }else{
        pf++;
        y = findlowcost();
        pagetable[frame[y]].vi = 0;
        frame[y] = pnum;
        pagetable[pnum].fnum = y;
        pagetable[pnum].vi = 1;
    }
}
/*read values from file and write */
int fillpagetable(void){
    int readbytes,pnum,offset;
    unsigned long buf,pa;
    while( (readbytes = read(infile,&buf,sizeof(buf))) > 0 ){
        clock++;
        pnum = buf >> 7;
        offset = buf & 0x7F;
        add2pagetable(pa,pnum,offset);
        pa = convPA(buf);
        count[pagetable[pnum].fnum] = clock;
        if( write(outfile,&pa,sizeof(unsigned long)) < 0){
            perror("WRITE ERROR");
            exit(-1);
        }
    }
    return ;
}
//set all bits to zero
void setbit(){
    int i = 0 ;
    for(i;i<33;i++){
        pagetable[i].vi =0;
    }
    i = 1;
    for(i =1;i<8;i++){
        frame[i] =0;
        count[i] = 0;
    }

}
int main(int argc, char* argv[]) {
    checkarg(argc,argv);
    setbit();
    fillpagetable();
    printf("part 2 pagefault = %d\n",pf);
}

