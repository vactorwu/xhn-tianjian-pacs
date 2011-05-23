  //********************************************************************
//  模块: Reflo.cpp
//  描述: 本模块为曼海姆Reflotron联机程序(Win95版)
 
#include "stdio.h"
#include "const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength		256		
#define	c_MaxItemNum			3		
#define c_SubstrateResultOff	5		 
#define nResultLen				6		
#define c_EnzymeResultOff		4		
#define c_ItemNameLength		4		
#define	nResultMaxLen			20		


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,nItemCount;
	char *pTemp1,*pTemp2;
   
	Init()
	nItemCount =0;
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	WriteCommChar(XON)
	
	while (TRUE)
	{

		ResetRxPointer()		

		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)




		GetLocalTime(&SystemTime);
		nItemCount =0;
 
							 
		pTemp1=lpOpPointer;
		for(i=0;i<2;i++)
		{
			pTemp1=strchr(pTemp1,LF);
			pTemp1++;
		}

		pTemp2=strchr(pTemp1,LF);
		pTemp2++;		 

		wSampleID=StrToInt(pTemp1,(int)(pTemp2-pTemp1-2));
		FillSampleID(nItemCount,wSampleID)
		pTemp1=pTemp2;	

		memset(OutResult[nItemCount].ItemID,' ',strlen(OutResult[nItemCount].ItemID));

		FillItemName(nItemCount , pTemp1 , c_ItemNameLength)

		if( !(strncmp(pTemp1 ,"CK  ",4)*
			  strncmp(pTemp1 ,"HDLP",4)*
			  strncmp(pTemp1 ,"AMYL",4)*
			  strncmp(pTemp1 ,"P-AM",4)*
			  strncmp(pTemp1 ,"GPT ",4)*
			  strncmp(pTemp1 ,"GOT ",4)*
			  strncmp(pTemp1 ,"GGT ",4)))	
		{	

				FillResult(nItemCount,pTemp1+c_EnzymeResultOff ,nResultLen )


		}
		else	
		{

				FillResult(nItemCount,pTemp1+c_SubstrateResultOff  ,nResultLen ) 
		}
 

		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		pTemp1=strchr(pTemp1,LF)+1;	
		if ( *pTemp1 !=CR )	
		{
			
			pTemp2=strchr(pTemp1,CR);
			FillResult(nItemCount,pTemp1 ,min(nResultMaxLen ,int(pTemp2-pTemp1)) ) 			
			FillItemName(nItemCount ,"Rela_Meg",8 )
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)			
			FillSampleID(nItemCount,wSampleID)
			nItemCount++;
		}

		pTemp1=strchr(pTemp1,LF)+1;	
		if ( *pTemp1 !=CR )	
		{
 			
			pTemp2=strchr(pTemp1,CR);
			FillResult(nItemCount,pTemp1 ,min(nResultMaxLen ,int(pTemp2-pTemp1)) ) 			
			FillItemName(nItemCount ,"AddTInfo" ,8  )
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)			
			FillSampleID(nItemCount,wSampleID)
			nItemCount++;
		}

	if(nItemCount>0)
	{
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;
	}

		ClearCommRx()
//		WriteCommChar(ACK)
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

