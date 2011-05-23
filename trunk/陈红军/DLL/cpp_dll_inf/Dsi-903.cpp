//********************************************************************
//  模块: DSI-903.cpp
//  描述: 本模块为国内组装的DSI-903化验仪的联机程序(Win95版)

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
 
#define c_RxBufferLength	800		  
#define	c_MaxItemNum		3		  
 
#define c_SampleIDLen		5		
#define c_OneDataLen		15		
#define c_ItemNum			3		
static const CHAR * _gItemID[c_ItemNum] =		
	{"Na", "K", "Cl"};
static const int _gResultLen[c_ItemNum] =		
	{ 3, 4, 3};

#define	c_DataNumOff		1		

#define c_SampleIDOff		0		
static const int _gResultOff[c_ItemNum] =	
	{ 5, 8, 12};
 

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, j,nItemCount, nDataOff, nDataNum,nResultLen;
	LPSTR lpPointer;

	Init()
	ResetOpPointer()				
	nItemCount =0;
	FillDevName()					
	while (TRUE)
	{		  

		ClearCommRx()
		TranCommChar(EOT)		
		GetInQueueLength(dwReadLength)
		if (dwReadLength ==0)
		{
			WaitEvent(EV_RXFLAG)
			GetInQueueLength(dwReadLength)
		}
		ReadCommBlock(dwReadLength)

		int jjj;
		*(lpOpPointer+dwReadLength)=0;
		jjj=strlen(lpOpPointer);


		GetLocalTime(&SystemTime);
		
		nDataNum =lpOpPointer[c_DataNumOff];	
		nDataOff =2;	//ACK+Num
		
	for(i =0; i<nDataNum && nDataOff<dwRxBufferLength; i++, nDataOff+=c_OneDataLen)
	{
		
		wSampleID =(int)StrToInt(lpOpPointer+nDataOff+c_SampleIDOff, c_SampleIDLen);
		nItemCount =0;	
			for (j =0; j<c_ItemNum;j++)	
			{
				FillSampleID(nItemCount, wSampleID)	
				strcpy(OutResult[nItemCount].ItemID, _gItemID[j]);


				lpPointer= lpOpPointer+nDataOff+_gResultOff[j];
				nResultLen=_gResultLen[j] ;
				for( ; *lpPointer=='0'; nResultLen--, lpPointer++)	
				{								
					if (*(lpPointer+1)!='0')	
					{							
						if (*(lpPointer+1)!='.')
						{						
							lpPointer++;		
							nResultLen--;		
							break;				
						}						
						else break;				
					}							
				}
 
				if (nResultLen==0)
				{			
					lpPointer--;
					nResultLen++;
				}
			
				FillResult(nItemCount, lpPointer ,nResultLen )
			
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nItemCount++;
			}
			if (nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
	}
		
		ResetRxPointer()

//		Sleep(10000);
		WriteCommChar(STX)	
		Sleep(10000);
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =0x1a;	 
}

