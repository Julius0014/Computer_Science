#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wordcountfreg.h"
#include <sys/types.h>
#define MAXSIZE 4096
#include <ctype.h>

/* wordcount uses a filename then read each line into a buffer is
   incruments for each newline,Space,and TAB chararter*/
int count = 0;
struct word *wordList;
pthread_mutex_t lock;
struct word *newWord(char word[]){
    struct word *node;
    node = malloc(sizeof(struct word));
    strcpy(node->value,word);
    node->count = 1;
    node->pnext = NULL;
    return node;
}
/* Print the list of words and count then free the word from the list*/
void printlist(struct word *list){
    struct word *temp,*last;
    temp = list;
    while(temp != NULL){
        printf("%s appears %d times\n",temp->value,temp->count);
            last = temp;
        temp = temp->pnext;
        free(last);
    }
}
/* check list for word if found add 1 to word count*/
int match(char word[],struct word *list){
    struct word *temp;
    temp = list;
    while(temp != NULL){
        if(strcmp(word,temp->value) == 0){
            temp->count++;
            return 0;
        }
        temp = temp->pnext;
    }
    return 1;
}
/*Update list by adding a new word or add 1 to count to word that exist*/
struct word *update(char word[],struct word *wordList){
    struct word *temp,*last;
    temp = wordList;
    if(wordList == NULL){
        wordList = newWord(word);
        return wordList;
    }
    if(match(word,wordList) == 1){
        while(temp != NULL){
            last = temp;
            temp = temp->pnext;
        }
        last->pnext = newWord(word);  
    }
    return wordList;
}
/* main functions counts word in file then add the word to list*/
int wordcount(char *filename){
    char buffer[MAXSIZE];
    char word[MAXSIZE];
    FILE *file;
    int i = 0,wc = 0,n = 0;
    file = fopen(filename,"r");
    if(file == NULL){
        fprintf(stderr,"Can't open file %s\n",filename);
        exit(-1);
    }
    while(fgets(buffer,MAXSIZE,file)){
        while(buffer[i] != '\0' ){
            if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t'){
                word[n] = '\0';
                pthread_mutex_lock(&lock);
                wordList = update(word,wordList);
                pthread_mutex_unlock(&lock);
                wc++;
                n=0;
                word[n] = '\0';
            }else{
                word[n++] = tolower(buffer[i]);
            }
            i++;
        }
        i=0;
    }
    fclose(file);
    printf("Child process for %s :number of words is %i\n",filename,wc);
    count += wc;
    return;
}
/*check for usage errors*/
void usage(int argc){
    if (argc  -1 < 1){
        fprintf(stderr,"USAGE ERROR: Must Have at least 1 arguments\nExample: ./wordcount Input.txt\n");
        exit(1);
    }                 
}
void *word( void * arg){
    char *filename;
    filename = (char *) malloc(sizeof(char));
    strcpy(filename,(char *) arg);
    wordcount(filename);
}
/* fork children for each file*/
void processfile(char *argv[],int argc){
    int i = 1;
    pthread_t thread_id[argc];
    for(i = 1 ;i < argc;i++){
        pthread_create(&thread_id[i], NULL,word,(void *)argv[i]);
    }
    for(i = 1 ;i < argc ;i++){
        pthread_join(thread_id[i],NULL);
    }


}
/*Swap Data for bubbleSort*/
void swapData(struct word *x,struct word *y){
     struct word temp;
     strcpy(temp.value,x->value);
     temp.count = x->count;
     strcpy(x->value,y->value);
     x->count = y->count;
     strcpy(y->value,temp.value);
     y->count = temp.count;
}
/*BubbleSort for linkList*/
void sort(){
    struct word *temp,*temp2,swap;
    int swapped = 1;
    temp = wordList;
    temp2 = NULL;
    while(swapped){
        swapped = 0;
        temp = wordList;
        while(temp->pnext != temp2 ){
            if(strcmp(temp->value,temp->pnext->value) > 0){
                swapData(temp,temp->pnext);
                swapped = 1;
            }
            temp = temp->pnext;
        }
        temp2 = temp;
    }
}
/* the main function creates the children process base on the number of files that need to be
   in argc then had to the parent wait for each child to finish. When each child finished the parent 
   print ist own method*/
int main(int argc,char *argv[]){
    usage(argc);
    processfile(argv,argc);
    sort();
    printf("All %i files have been counted and the total of %i words found !\n",argc-1,count);
    printlist(wordList);
    return 0;
}



