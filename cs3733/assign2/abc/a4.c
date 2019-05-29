#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
typedef struct x {
    double *array;
    int size;
} x;
double *mainarray,*a,*b;
x ar1,ar2,ar3;
/*add random double to array*/
double addrandom(double *array,int size){
    int i;
    srand(time(0));
    for(i = 0;i < size;i++){
        array[i] = rand()%1000;
    }
    return *array;
}
/* split array in halves */
double *copyarrayhalf(int start,int end,double array[]){
    int i =0;
    double *halfarray = (double *) malloc(sizeof(double)*(end - start));
    for(start;start < end;start++){
        halfarray[i++] = array[start]; 
    }
    return halfarray;
}
double *bubbleSort(double arr[], int n) 
{
    int i, j;
    int temp;
    for (i = 0; i < n-1; i++){      
        for (j = 0; j < n-i-1; j++){  
            if (arr[j] > arr[j+1]){
                temp=arr[j];
                arr[j]= arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
    return arr;
} 
void pt(double array[],int size){
    int i = 0;
    for(i;i< size;i++){
        printf("%.2f ",array[i]);
    }
    printf("\n");
}
/*fix the struct for bubblesort*/
void *bubblepthread(void *arg){
    x *temp;
    temp = (x *) arg;
    temp->array = bubbleSort(temp->array,temp->size); 
}
/*Merge arrays*/
double *mergearray(double a[],double b[],int size){
    int i = 0,j = 0,k = 0;
    double *m = (double *) malloc(sizeof(double) * size);
    int half = (size)/2;
    while( i != (size ) && j != (half) && k != (half)){
        if(a[j] < b[k]){
            m[i++] = a[j++];
        }else if(a[j] > b[k]) {
            m[i++] = b[k++];
        }else{
        m[i++] = a[j++];
        }
    }
    while( k!= half) m[i++] = b[k++];
    while( j!= half) m[i++] = a[j++];
    return m;
}
int main(int argc,char *argv[]){
    int size = atoi(argv[1]);
    mainarray =(double *) malloc(sizeof(double)*(size));
    addrandom(mainarray,size);
    int ar = (size+1)/2; 
    a = copyarrayhalf( 0,ar,mainarray);
    b = copyarrayhalf(ar,size,mainarray);
    ar1.size = ar;
    ar1.array = a;
    ar2.size = ar;
    ar2.array = b;
    ar3.size = size;
    ar3.array = mainarray;
    
    pthread_t thread[3];
    long start = clock();
    pthread_create(&thread[0],NULL,bubblepthread,(void *) &ar1);
    pthread_create(&thread[1],NULL,bubblepthread,(void *) &ar2);
    pthread_join(thread[0],NULL);
    pthread_join(thread[1],NULL);
    double *merge = mergearray(a,b,size);
    long end  = clock();
    printf("Sorting is done in %.2fms when two threads are used\n",((double)(end - start)/CLOCKS_PER_SEC) * 1000); 
    start  = clock();
    pthread_create(&thread[2],NULL,bubblepthread,(void *) &ar3);
    pthread_join(thread[2],NULL);
    end = clock();
    printf("Sorting is done in %.2fms when one threads are used\n",((double)(end - start)/CLOCKS_PER_SEC) * 1000); 
}
