#include "cs3743p2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/********************hashCreate**************************************
	int hashCreate(char szFileNm[], HashHeader *pHashHeader)
Purpose:
    This function creates a hash file containing only the HashHeader record.
Parameters:
    I char szFileNm[]    name of the file 
    I HashHeader *pHashHeader    pointer to the hashheader
Returns:
	RC_FILE_EXISTS If the file already exists
	RC_OK If the file doesn't exist
**************************************************************************/
int hashCreate(char szFileNm[], HashHeader *pHashHeader)
{
	int rc;
	FILE *fp1, *fp2;
//open with rb to see if file exists, since it only one to 
//return NULL if it does not exist

	if((fp1 = fopen(szFileNm,"rb")) != NULL)
	{
		fclose(fp1);
		return RC_FILE_EXISTS;
	}

	fp2 = fopen(szFileNm, "wb+");

	pHashHeader->iHighOverflowRBN = pHashHeader->iNumPrimary;

	rc = fwrite(pHashHeader,pHashHeader->iRecSize,1L,fp2); 

	if(rc != 1)
	{
		printf("write didn't work\n");
		fclose(fp2);
		exit(0);
	}

	fclose(fp2);

	return RC_OK;
}
/********************hashOpen**************************************
        FILE  *hashOpen(char szFileNm[], HashHeader *pHashHeader)
Purpose:
	This function opens an existing hash file which must contain 
	a HashHeader record and returns a file pointer to it.
Parameters:
    I char szFileNm[]    name of the file 
    I HashHeader *pHashHeader    pointer to the hashheader
Returns:
        File Pointer If the file already exists
        NULL If the file doesn't exist
**************************************************************************/
FILE *hashOpen(char szFileNm[], HashHeader *pHashHeader)
{
	FILE *fpA,*fpB;
	int byts_rd;
	fpA = fopen(szFileNm,"rb");

	if (fpA == NULL)
	{
		return NULL;
	}

	fpB = fopen(szFileNm,"rb+");

	if ( (byts_rd=fread(pHashHeader,sizeof(pHashHeader),1L,fpB)) !=1)
	{  
		return NULL;
	}

	return fpB;
}
/********************readRec**************************************
	int readRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize)
Purpose:
	This function reads a record of the specified size at the 
	specified RBN in the specified file. 
Parameters:
    I FILE *pFile    file pointer, of where rec is to be read from 
    I int iRBN    Relative byte number, used to get relative byte address
    I void *pRecord    where the record will be read into
    I int iRecSize    the size of a record
Returns:
        RC_LOC_NOT_FOUND    If the location is empty
        RC_OK    if location is filled with a record
**************************************************************************/
int readRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize)
{
	int RBA,i;
	Book *pB;
	pB = malloc(sizeof(Book));	

	RBA = (iRBN ) * iRecSize;

	if(fseek(pFile, RBA, SEEK_SET) != 0)
	{
		return RC_LOC_NOT_FOUND;
	}

	fread(pRecord,iRecSize,1L,pFile);

	pB = pRecord;

	if(pB->szBookId[0] == '\0')
	{ 
		return  RC_LOC_NOT_FOUND;
	}

	return RC_OK;
}
/********************writeRec**************************************
        int writeRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize)
Purpose:
        This function writes a record of the specified size at the 
        specified RBN in the specified file. 
Parameters:
    I FILE *pFile    file pointer, of where rec is to be written 
    I int iRBN    Relative byte number, used to get relative byte address
    I void *pRecord    record to be written
    I int iRecSize    the size of a record
Returns:
        RC_LOC_NOT_WRITTEN    If write fails
        RC_OK    if write works
**************************************************************************/
int writeRec(FILE *pFile, int iRBN, void *pRecord, int iRecSize)
{
	int RBA,i;
	Book *pBk;
	pBk = malloc(sizeof(Book));

	pBk = pRecord;

	RBA = (iRBN ) * iRecSize;

	fseek(pFile, RBA, SEEK_SET);

	if (fwrite(pBk,iRecSize,1L,pFile) !=1)
	      return RC_LOC_NOT_WRITTEN;
	return RC_OK;
}
/********************bookInsert**************************************
	int bookInsert(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
Purpose:
	This function inserts a book into the specified file.
Parameters:
    I FILE *pFile    file pointer 
    I HashHeader *pHashHeader    pointer to the header of the hash
    I Book *pBook    book to be inserted
Returns:
        RC_REC_EXISTS    If book has already been written 
	RC_SYNONYM    If synonym exists at the specified location
        RC_OK    if insert works
**************************************************************************/
int bookInsert(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
{
	int RBN,rc1, rc2,i;
	Book *pRec;
	Book *tempbook;
	pRec = malloc(sizeof(Book));
	tempbook = malloc(sizeof(Book));
	RBN = hash(pBook->szBookId,pHashHeader); 

	rc1 = readRec(pFile,RBN,pRec,pHashHeader->iRecSize);
	tempbook = pRec;
	if(rc1 != RC_OK ||  pBook->szBookId[0] =='\0')
	{
		pBook->iNextChain = 0;
		writeRec(pFile,RBN,pBook,pHashHeader->iRecSize);
		return RC_OK;
	}
	else if(rc1 == RC_OK && (strcmp(pBook->szBookId, pRec->szBookId) == 0))
	{
		return RC_REC_EXISTS;
	}
	else
	{
		while(pRec->iNextChain != 0)
		{
				tempbook = pRec;
				rc1 = readRec(pFile,pRec->iNextChain, pRec,pHashHeader->iRecSize);
				if( rc1 == RC_OK && (strcmp(pRec->szBookId,pBook->szBookId) == 0) )
				{
					return RC_REC_EXISTS;
				}
		}	
		
		int highOverFlow = pHashHeader->iHighOverflowRBN;
		highOverFlow++;
		pHashHeader->iHighOverflowRBN = highOverFlow;
		rc1 = writeRec(pFile,0,pHashHeader,pHashHeader->iRecSize);
		tempbook->iNextChain = pHashHeader->iHighOverflowRBN;
		int iRBN = hash(tempbook->szBookId,pHashHeader);
		rc1 = writeRec(pFile,iRBN,tempbook,pHashHeader->iRecSize);
		rc1 = writeRec(pFile,tempbook->iNextChain,pBook,pHashHeader->iRecSize);
		return RC_OK;
	}
}
/********************bookRead**************************************
        int bookRead(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
Purpose:
	This function reads the specified book by its szBookId
Parameters:
    I FILE *pFile    file pointer 
    I HashHeader *pHashHeader    pointer to the header of the hash
    I Book *pBook    book to be read
Returns:
        RC_REC_NOT_FOUND    if the book was not found
	RC_OK    if the book at that location matches the specified szBookId
**************************************************************************/
int bookRead(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
{
	int RBN,rc,rc1;
	Book *pRecord;

	pRecord = malloc(sizeof(Book));
	RBN = hash(pBook->szBookId,pHashHeader);

	rc = readRec(pFile,RBN,pRecord,pHashHeader->iRecSize);

	if(strcmp(pBook->szBookId, pRecord->szBookId) == 0)
	{
		memcpy(pBook,pRecord,pHashHeader->iRecSize);
		return RC_OK;
	}
	else{
		while(pRecord->iNextChain != 0)
		{
				rc1 = readRec(pFile,pRecord->iNextChain, pRecord,pHashHeader->iRecSize);
				if( rc1 == RC_OK && (strcmp(pRecord->szBookId,pBook->szBookId) == 0) )
				{
					memcpy(pBook,pRecord,pHashHeader->iRecSize);
					return RC_OK;
				}
		}

			return RC_REC_NOT_FOUND;
	}
}
/********************bookUpdate**************************************
	bookUpdate(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
Purpose:
	This function reads the specified book by its szBookId.  If 
	found, it updates the contents of the book without losing the 
	record's iNextChain.  If not found, it returns RC_REC_NOT_FOUND.
Parameters:
    I FILE *pFile    file pointer 
    I HashHeader *pHashHeader    pointer to the header of the hash
    I Book *pBook    book to be read
Returns:
        RC_REC_NOT_FOUND    if the book was not found
        RC_OK    if the book at that location matches the specified szBookId
**************************************************************************/
int bookUpdate(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
{
	int RBN,rc;
	RBN = hash(pBook->szBookId,pHashHeader);
	rc = bookRead(pFile,pHashHeader,pBook);
	
	if( rc == RC_OK)
	{
		writeRec(pFile,RBN,pBook,pHashHeader->iRecSize);
		return RC_OK;
	}
	else
		return RC_REC_NOT_FOUND;
}
/********************bookDelete**************************************
	bookDelete(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
Purpose:
	If you did not do the extra credit, create a simple function 
	that just returns RC_NOT_IMPLEMENTED.
Parameters:
    I FILE *pFile    file pointer 
    I HashHeader *pHashHeader    pointer to the header of the hash
    I Book *pBook    book to be read
Returns:
	RC_NOT_IMPLEMENTED
**************************************************************************/
int bookDelete(FILE *pFile, HashHeader *pHashHeader, Book *pBook)
{
	return RC_NOT_IMPLEMENTED;
}
