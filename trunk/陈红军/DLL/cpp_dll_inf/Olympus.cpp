//********************************************************************
//  模块:  olympus.cpp
//  描述: 本模块为奥林巴斯公司的olympus_AU1000化验仪的联机程序(Win95版)

 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		  
#define	c_MaxItemNum		32		
#define c_PackTypeOff		1
#define c_TranTypeOff		4
 
#define c_SampleNoOff		5
#define c_SampleIdOff		4
#define c_SampleIDLen		3
 
#define c_DataOff			188
#define c_OneDataLen		10
#define c_ItemIDOff			0		
#define c_ItemIDLen			2
#define c_ResultOff			2		
#define c_ResultLen			6
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nPackType, nTranType, nDataOff,nResultLen;
	BYTE  btCalCheckSum, btRxCheckSum;
	WORD 		wSampleNo;						
	WORD 		wRSampleNo;	
 	BYTE		btResultType;
	LPSTR		lpPointer;

	Init()
	nItemCount =0;
	ResetOpPointer()				
	FillDevName()					
	PurgeComm( hComm, PURGE_RXCLEAR );
	SetCommWaitMask(EV_RXFLAG)
	
	while (TRUE)	
	{

		ResetRxPointer()		
		do
		{
			GetInQueueLength(dwReadLength)
			if(dwReadLength==0 &&dwRxBufferLength==0)
			{
				WaitEvent(EV_RXFLAG)  
				GetInQueueLength(dwReadLength)
			}
 
			ReadCommBlock(dwReadLength)	
			if ( dwRxBufferLength>=2)
			{
				if( (*(lpRxPointer-2)== ETX) )
				break;
			}
				
		}while(TRUE);				
 
		nPackType = lpOpPointer[c_PackTypeOff];
		btResultType =0xff;	
		nItemCount =0;
		switch (nPackType)	
		{
		case	'R':
			break;

		case	'D':
			nTranType =lpOpPointer[c_TranTypeOff];
		
			switch (nTranType)	
			{				  
			case	'B':
				break;
			case	'E':

				if (lpOpPointer[c_TranTypeOff+1]==ETX) 	break;

			default:
 
 				
				GetLocalTime(&SystemTime);
 				switch (*(lpOpPointer+c_SampleIdOff))	
				{
				case	'0':
					wSampleNo =StrToInt(lpOpPointer+c_SampleIdOff, c_SampleIDLen+1);
					btResultType =c_TypeSample;
					break;
				case	'1':
					wSampleNo =StrToInt(lpOpPointer+c_SampleIdOff, c_SampleIDLen+1);
					btResultType =c_TypeSample;
					break;
				case	'2':
					wSampleNo =StrToInt(lpOpPointer+c_SampleIdOff, c_SampleIDLen+1);
					btResultType =c_TypeSample;
					break;
				case	'3':
					wSampleNo =StrToInt(lpOpPointer+c_SampleIdOff, c_SampleIDLen+1);
					btResultType =c_TypeSample;
					break;
				case	'Q':
					wRSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleIDLen);
					wSampleNo = wRSampleNo+8100;
					btResultType =c_TypeQC;
					break;
				case	'P':
					wRSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleIDLen);
					wSampleNo = wRSampleNo+6000;
					btResultType =c_TypeStat;
					break;
				case	'E':
					wRSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleIDLen);
					wSampleNo = wRSampleNo+4000;
					btResultType =c_TypeEmergency;
					break;
				case	'U':
					wRSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleIDLen);
					wSampleNo = wRSampleNo+5000;
					btResultType =c_TypeUrine;
					break;

				}

			}
 
			if(btResultType != 0xff)	
				for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX  ;)
				{
				FillSampleID(nItemCount, wSampleNo)	
				FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)
 

				DeleZero(lpOpPointer, c_ResultLen)
			
				FillResult(nItemCount, lpPointer ,nResultLen )

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)

				FillResultType(nItemCount, btResultType)	

				FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	//填状态注解

				FillErrorCode(nItemCount, c_Reliable)	
				nItemCount++;
				nDataOff+=c_OneDataLen;
				}
			break;
		}


		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		ResetRxPointer()

		PurgeComm( hComm, PURGE_RXCLEAR );
 		WriteCommChar(ACK)
 

  }

	return TRUE;

} 

void WINAPI BeforeSetCommState(DCB *pComDcb)
{

	pComDcb->EvtChar =ETX;
}

