//********************************************************************
//  模块: CLT_JTIR.cpp
//  描述: 本模块为库而特公司的JT_IR血球化验仪的联机程序(Win95版)
 
//********************************************************************
#include <stdio.h>
//#include <wchar.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	600		
#define	c_MaxItemNum		18		

#define c_TestIDOff			0x32
#define c_TestIDLen			3
#define	c_LabIDOff			0x50
#define	c_LabIDLen			8

#define c_ResultRecordLen	16
#define c_FirstDataOff		0x5d	
#define c_ItemIDOff			0		
#define c_ItemIDLen			4		//
#define c_ResultOff			6		//
#define c_ResultLen			5		//



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nRecordLen=0,ss=0;
	LPSTR lptemp1;
	BYTE	Achar;

	Init()
	nItemCount =0;
	ResetOpPointer()		
	ResetRxPointer()
	ClearCommRx()		
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)

	WriteCommChar(ACK)

	while (TRUE)
	{

		WaitEvent(EV_RXFLAG)


		GetInQueueLength(dwReadLength)
 
		ReadCommBlock(dwReadLength)

		*(lpOpPointer+dwReadLength)='\0';

		lptemp1=strchrnb(lpOpPointer,STX,1,10);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memmove( lpOpPointer, lptemp1,dwRxBufferLength );
		} 
	
		GetLocalTime(&SystemTime);	
		nItemCount =0;
		
		wSampleID =StrToInt(lpOpPointer+c_TestIDOff, c_TestIDLen);		
//		wSampleID =StrToInt(lpOpPointer+c_LabIDOff, c_LabIDLen);
		nRecordLen=c_FirstDataOff;
		for (i=1;i<=c_MaxItemNum;i++)
		{
				
			FillSampleID(nItemCount, wSampleID)	

	 		FillItemName(nItemCount, lpOpPointer+nRecordLen+c_ItemIDOff, c_ItemIDLen)

			
			if (strncpy(lpOpPointer+nRecordLen+c_ItemIDOff ,"EO #",4)==0 || strncpy(lpOpPointer+nRecordLen+c_ItemIDOff ,"BA #",4)==0)
			{
				FillResult(nItemCount, lpOpPointer+nRecordLen+c_ResultOff-2 , c_ResultLen+2) 
			}
			else
			{

				FillResult(nItemCount, lpOpPointer+nRecordLen+c_ResultOff , c_ResultLen) 
			}
			
			
				
 			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nRecordLen +=c_ResultRecordLen;
			nItemCount++;
		}				

		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		ResetRxPointer()		
		ResetOpPointer()
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

