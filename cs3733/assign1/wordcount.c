#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#define MAXSIZE 4096
  /* wordcount uses a filename then read each line into a buffer is
incruments for each newline,Space,and TAB chararter*/
int wordcount(char *filename){
   char buffer[MAXSIZE];
   FILE *file;
   int i = 0,wc = 0;
   file = fopen(filename,"r");
   if(file == NULL){
      fprintf(stderr,"Can't open file %s\n",filename);
      exit(-1);
    }
    while(fgets(buffer,MAXSIZE,file)){
       while(buffer[i] != '\0' ){
          if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
             wc++;
          i++;
        }
        i =0;
    }
    fclose(file);
    printf("Child process for %s :number of words is %i\n",filename,wc);
    return;
}
    /*check for usage errors*/
void usage(int argc){
   if (argc  -1 < 1){
      fprintf(stderr,"USAGE ERROR: Must Have at least 1 arguments\nExample: ./wordcount Input.txt\n");
      exit(1);
     }                 
}
    /* the main function creates the children process base on the number of files that need to be
    in argc then had to the parent wait for each child to finish. When each child finished the parent 
    print ist own method*/
int main(int argc,char *argv[]){
   pid_t childpid = 0;
   pid_t parentpid = getpid();
   int i;
   usage(argc);
   for(i = 1;i < argc;i++){
      childpid = fork();
      if(childpid <= 0){
         wordcount(argv[i]);
         break;
      }
      if(childpid != wait(NULL)){
        perror("Parent failed to wait due to signal or error");
            exit(-1);
        }

    }
    if(parentpid == getpid()){
        printf("All %i files have been counted!\n",argc - 1);

    }

return 0;
}



