#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "cs3743p1.h"
int hashCreate(char szFileNm[], HashHeader *pHashHeader){
    FILE *File;
    int WC;
    File = fopen(szFileNm,"rb");
    if(File == NULL){
        File = fopen(szFileNm,"wb+");
        pHashHeader->iHighOverflowRBN = pHashHeader->iNumPrimary;
        WC = fwrite(&pHashHeader,pHashHeader->iRecSize,1l,File);
        fclose(File);
    }else{
        fclose(File);
        return RC_FILE_EXISTS;
    }
}
FILE *hashOpen(char szFileNm[], HashHeader *pHashHeader){
    FILE *File;
    int WC;
    int read_value;
    HashHeader *pHash;
    File = fopen(szFileNm,"rb+");
    if(File != NULL){
        if ((read_value = fread(&pHash,sizeof(pHashHeader),1l,File)) !=  1 ){
            return NULL;
        }
        *pHashHeader = *pHash;
    }
    return File;
}
int readRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize){
    int nTest_seek;
    char buffer[iRecSize];
    if(pFile == NULL) return  RC_FILE_NOT_FOUND;
    nTest_seek = fseek(pFile,(iRBN -1)*iRecSize,SEEK_CUR);
    if ( nTest_seek != 0) return RC_LOC_NOT_FOUND;
    nTest_seek = fread(buffer,iRecSize,1L,pFile);
    if(strcmp(buffer,pRecord) == 0) return RC_OK;
    if ( nTest_seek != 1) return 2;

}
int writeRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize){
    int nTest_seek;
    if(pFile == NULL) return  RC_FILE_NOT_FOUND;
    nTest_seek = fseek(pFile,(iRBN - 1) * iRecSize,SEEK_SET);
    if ( nTest_seek != 0) return RC_LOC_NOT_FOUND;
    nTest_seek = fwrite(pRecord,sizeof(pRecord),1L,pFile);
    printf("WRITE %i RECORD(S)\n",nTest_seek);
    if ( nTest_seek == 0) return RC_LOC_NOT_WRITTEN;

}
int bookInsert(FILE *pFile, HashHeader *pHashHeader, Book *pBook){
    int RBN = hash(pBook->szBookId,pHashHeader);
    void *pRecord;
    pRecord = pBook;
    int rec_num = readRec(pFile,RBN,&pBook,pHashHeader->iRecSize);
    printf("NUM %i\n",rec_num);
    if(rec_num != RC_OK || pBook->szBookId[0] == '\0'){
        pBook->iNextChain = 0;
        printf("write \n");
        writeRec(pFile,RBN,&pBook,sizeof(pBook));
        return 0;
    }

}
int bookRead(FILE *pFile, HashHeader *pHashHeader, Book *pBook){
    printf("Staret");
}

