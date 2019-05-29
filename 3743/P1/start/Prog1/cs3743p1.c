#include "cs3743p1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**********************************************************************
This function creates a hash file containing only the HashHeader record.
"	If the file already exists, return RC_FILE_EXISTS
"	Create the binary file by opening it.
"	Set pHashHeader->iHighOverflowRBN to the value of pHashHeader->iNumPrimary
"	Write the HashHeader record to the file at RBN 0.
"	fclose the file.
***********************************************************************/
int hashCreate(char szFileNm[], HashHeader *pHashHeader){
	int rc;
	FILE *pFile;
	//check to see if file exist

	if((pFile = fopen(szFileNm,"rb")) != NULL){
		fclose(pFile);
		return RC_FILE_EXISTS;
	}
	pFile = fopen(szFileNm, "wb+");
	pHashHeader->iHighOverflowRBN = pHashHeader->iNumPrimary;
	fwrite(pHashHeader,pHashHeader->iRecSize,1L,pFile); 
	fclose(pFile);
	return RC_OK;
}
/**********************************************************************
This function opens an existing hash file which must contain a HashHeader record and returns a file pointer to it.
"	Use fopen to open the file.  If it doesn't exist, return NULL.
"	Use fread to read the HashHeader record and return it through the parameter.  If the read fails, return NULL.
"	Return the opened FILE pointer as the function value when successful.
***********************************************************************/
FILE *hashOpen(char szFileNm[], HashHeader *pHashHeader){
	FILE *fpA;
	int readNUm;
	fpA = fopen(szFileNm,"rb");
	if (fpA == NULL){
		return NULL;
	}
	fpA = fopen(szFileNm,"rb+");
	if ( (readNUm =fread(pHashHeader,sizeof(pHashHeader),1L,fpA)) !=1){  
		return NULL;
	}
	return fpA;
}
/**********************************************************************
This function reads a record of the specified size at the specified RBN in the specified file. 
"	Determine the RBA based on iRBN and iRecSize.
"	Use fseek to position the file in that location.
"	Use fread to read that record and return it through pRecord.
"	If the location is not found, return RC_LOC_NOT_FOUND.  Otherwise, return RC_OK.
"	Note: if the location is found, that does NOT imply that a book was written to that location.  Why?
u********************************************************/
int readRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize){
	int RBA,i;
	Book *pB;
	pB = malloc(sizeof(Book));	
	RBA = (iRBN ) * iRecSize;
	if(fseek(pFile, RBA, SEEK_SET) != 0){//seek error
		return RC_LOC_NOT_FOUND;
	}
	fread(pRecord,iRecSize,1L,pFile);
	pB = pRecord;
	if(pB->szBookId[0] == '\0'){ 
		return  RC_LOC_NOT_FOUND;
	}
	return RC_OK;
}
/**********************************************************************
This function writes a record of the specified size at the specified RBN in the specified file. 
"	Determine the RBA based on iRBN and iRecSize.
"	Use fseek to position the file in that location.
"	Use fwrite to write that record to the file.
"	If the fwrite fails, return RC_LOC_NOT_WRITTEN.  Otherwise, return RC_OK.
***********************************************************************/
int writeRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize){
	int RBA,i;
	Book *pBk;
	pBk = malloc(sizeof(Book));
	pBk = pRecord;
	RBA = (iRBN ) * iRecSize;
	fseek(pFile, RBA, SEEK_SET);
	if (fwrite(pBk,iRecSize,1L,pFile) ==1)
                return RC_OK;
	return RC_LOC_NOT_WRITTEN;
}
/**********************************************************************
This function inserts a book into the specified file.
"	Determine the RBN using the driver's hash function.
"	Use readRec to read the record at that RBN.  
"	If that location doesn't exist or the record at that location has a szBookId[0] == '\0':
o	Sets its iNextChain to 0.
o	Write this new  book record at that location using writeRec.
"	If that record exists and that book's szBookId matches, return RC_REC_EXISTS.  (Do not update it.)
"	Otherwise, return RC_SYNONYM.
***********************************************************************/
int bookInsert(FILE *pFile, HashHeader *pHashHeader, Book *pBook){
	int RBN,rc1, rc2,i;
	Book *pRec;
	pRec = malloc(sizeof(Book));
	RBN = hash(pBook->szBookId,pHashHeader); 
	rc1 = readRec(pFile,RBN,pRec,pHashHeader->iRecSize);
	if(pBook->szBookId[0] =='\0' || rc1 != RC_OK ){
		pBook->iNextChain = 0;
		rc2 = writeRec(pFile,RBN,pBook,pHashHeader->iRecSize);
		return 0;
	}
	if(rc1 == RC_OK && (strcmp(pBook->szBookId, pRec->szBookId) == 0)){
	return RC_REC_EXISTS;
	}
	else
		return RC_SYNONYM;
}
/**********************************************************************
This function reads the specified book by its szBookId.
"	Determine the RBN using the driver's hash function.
"	Use readRec to read the record at that RBN. 
"	If the book at that location matches the specified szBookId, return the book via pBook and return RC_OK.
"	Otherwise, return RC_REC_NOT_FOUND 
***********************************************************************/
int bookRead(FILE *pFile, HashHeader *pHashHeader, Book *pBook){
	int RBN,rc,y=1;
	Book *pRecord;
	pRecord = malloc(sizeof(Book));
	RBN = hash(pBook->szBookId,pHashHeader);
	rc = readRec(pFile,RBN,pRecord,pHashHeader->iRecSize);
	if(strcmp(pBook->szBookId, pRecord->szBookId) == 0){//look at book insert for error 
		pBook = pRecord;
		return RC_OK;
	}
	else
		return RC_REC_NOT_FOUND;

}
