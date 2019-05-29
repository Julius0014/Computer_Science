#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"
#define MAXNUM -99
/*Check for usgaes errors*/
void usage(int argc){
    if (argc  - 1 < 3){
        fprintf(stderr,"USAGE ERROR: Must Have 3 arguments\nExample: ./driver1 x y z\n");
        exit(1);
    }

}
//Define my own max function instead of using the math library;
double max(double a , double b ){
    if( a > b)
        return a;
    return b;
}
/*
The next three function use the the same code but has a few changes base on what
type of variable needs;
Mainly just scanning the input then loop util the counter is met;
Use the define Max function to compare ints,doubles, and string lenghts;
*/
int loopInt(int counter){
    int i = 0;
    int maxnum = MAXNUM ;
    int num;
    while(i < counter){
        num = ReadInteger();  
        maxnum = max(num,maxnum);
        i++;
    }
    return maxnum;

}
double loopDouble(int counter){ 
    int i = 0;
    double maxnum = MAXNUM;
    double  num;
    while(i < counter){
        num = ReadDouble();  
        maxnum = max(num,maxnum);
        i++;
    }
    return maxnum;    
}
char *loopString(int counter){ 
    int i = 0;
    int maxnum=0;
    int numlast =0;
    char *maxstring = " "; 
    char *string;
    while(i < counter){
        printf("Enter String:\n");
        string  =  ReadLine();
        printf("String: %s\n",string);
        numlast = max(strlen(string),strlen(maxstring));
        if(numlast > maxnum){
            maxnum = numlast;
            maxstring = string;
        }
        i++;
    }
    return maxstring;    
}

int main(int argc,char *argv[]){
    usage(argc);
    int LargestInt = loopInt(atoi(argv[1]));
    double LargestDouble = loopDouble(atoi(argv[2]));
    char *LargestString = loopString(atoi(argv[3]));
    printf("Largest INT %i\nLargest Double %lf\nLongest String:%s\n",LargestInt,LargestDouble,LargestString);
}



