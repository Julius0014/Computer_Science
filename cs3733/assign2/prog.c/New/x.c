#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
int i;

main(){
char buf[6] = "1234";
i = open("temp.c",O_RDONLY); 
fork();
read(i, buf, 2); 
read(i, buf+2, 2); 
printf("%d:%s\n",getpid(), buf);
}
