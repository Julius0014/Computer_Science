#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"
#define buff 4096
#define false 0
#define true 1
/*Check for usgaes errors*/
void checkFiles(FILE *input,FILE *output){
    if (input == NULL) {
        printf("Error: Input file pointer is null.\n");
        exit(1);
    }

    if (output == NULL) {
        printf("Error: Output file pointer is null.\n");
        exit(1);
    }
}
/*Using a switch case to find spaces ans letter
Then only allow one space to be present after a word if another space is their
it will be skipped*/
void removeSpaces(FILE *input,FILE *output){
    char newString[buff],buffer[buff];
    char OutLine[buff];
    int space = false;
    int i,j = 0;
    while(fgets(buffer,buff,input)){
        strcpy(OutLine,buffer);
        for(i = 0;i < strlen(buffer);i++){
            switch(OutLine[i]){
                case ' ':
                if(space == true){
                    continue;
                }else{
                newString[j] = OutLine[i];
                j++;
                space = true;
            }
            break;
               default:
                newString[j] = OutLine[i];
                j++;
                space = false;
                break;
            }
        }
    fprintf(output,"%s\n",newString);
j=0;    
    }


}
int main(int argc,char *argv[]){
    FILE *Open = fopen(argv[1],"r"); 
    FILE *OUTPUT = fopen(argv[2],"w");
    checkFiles(Open,OUTPUT);
    removeSpaces(Open,OUTPUT);

}





