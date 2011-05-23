//********************************************************************
//  模块: STARLYTE_3.cpp
//  描述: 本模块为美国造TARLYTE_3钾钠氯化验仪的联机程序(Win95版)

 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	800	 
#define	c_MaxItemNum		3		 
#define	c_PackTypeOff		0x37		
#define c_SampleIDOff		0x7d		
#define c_SampleIDLen		4			

#define c_K_ResultOff			0x8b		
#define	c_Na_ResultOff			0x9e		
#define	c_Cl_ResultOff			0xb1		

#define c_ResultLen				5		





DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, j,nItemCount;


	Init()
	ResetOpPointer()				
	ResetRxPointer()
	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)

	PurgeComm( hComm, PURGE_RXCLEAR );	
	while (TRUE)
	{		  
		
		ResetOpPointer()				
		ResetRxPointer()

		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
		
		

		
		*(lpOpPointer+dwReadLength)=0;
		if (*(lpOpPointer+c_PackTypeOff)=='*')	
		{
			PurgeComm( hComm, PURGE_RXCLEAR );	 
			continue;
		}
	
		GetLocalTime(&SystemTime);
			
		wSampleID =(int)StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		nItemCount =0;	

		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"K" ,1)
		FillResult(nItemCount,lpOpPointer+c_K_ResultOff,c_ResultLen )
				
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"Na" ,2)
		FillResult(nItemCount,lpOpPointer+c_Na_ResultOff,c_ResultLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"Cl" ,2)
		FillResult(nItemCount,lpOpPointer+c_Cl_ResultOff,c_ResultLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
//	}
		
		ResetRxPointer()

	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;	 
}

