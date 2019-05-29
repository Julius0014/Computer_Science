#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"
#define buffer 4096
#define ERROR 0.0000001



char *ReadLine(void){
    return ReadLineFile(stdin);
}
char *ReadLineFile(FILE *infile){
    int i = 0;
    char line[buffer];
    if(infile == NULL && feof(infile)){
        return NULL;    
    }
    fgets(line,buffer,infile);
    printf("%c\n",line[strlen(line)]);
    line[strlen(line)] = '\0';
    char *chline = malloc(strlen(line));
    chline = line;
    return chline;
}

double ReadDouble(void){
    char buf[buffer];
    char *strend;
    int i = 0;
    printf("Enter A Number(Double):\n");
    scanf("%s",buf);
    while(i < strlen(buf)){
        if(!isdigit(buf[i]) && buf[i] != '.' ){
            printf("%c\n",buf[i]);
            fprintf(stderr,"NOT A DOUBLE\n");
            printf("Enter A Number(DOUBLE):\n");
            scanf("%s",buf);
            i = 0;
        }
        i++;
    }
    
    double x = strtof(buf,NULL);
    printf("%lf\n",x);
    return x;
}
int ReadInteger(void){
    char buf[buffer];
    int i = 0;
    printf("Enter A Number(INT):\n");
    scanf("%s",buf);
    while(i < strlen(buf)){
        if(!isdigit(buf[i]) ){
            fprintf(stderr,"NOT A INT\n");
            printf("Enter A Number(INT):\n");
            scanf("%s",buf);
            i = 0;
        }
        i++;
    }
    return atoi(buf);
}
