//********************************************************************
//  模块: CLT_T540.cpp
//  描述: 本模块为库而特公司的T540血球化验仪的联机程序(Win95版)

//********************************************************************
#include <stdio.h>
//#include <wchar.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	300		
#define	c_MaxItemNum		7		

#define c_TestIDOff			0x38
#define c_TestIDLen			3
#define	c_LabIDOff			0x50
#define	c_LabIDLen			8

#define c_ResultRecordLen	19
//#define c_FirstDataOff		0x44	
#define c_ItemIDOff			0		
#define c_ItemIDLen			4		//
#define c_ResultOff			7		//
#define c_ResultLen			6		//



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nRecordLen=0,ss=0,n_FirstDataOff;
	LPSTR lptemp;
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

		lptemp=strchrnb(lpOpPointer,STX,1,10);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp-lpOpPointer);
			memmove( lpOpPointer, lptemp,dwRxBufferLength );
		} 
	
		GetLocalTime(&SystemTime);	
		nItemCount =0;
		
		wSampleID =StrToInt(lpOpPointer+c_TestIDOff, c_TestIDLen);		
//		wSampleID =StrToInt(lpOpPointer+c_LabIDOff, c_LabIDLen);
		lptemp=strchrnb(lpOpPointer,'W',1,0x48);
		n_FirstDataOff=lptemp-lpOpPointer;
		nRecordLen=n_FirstDataOff;
		for (i=1;i<=c_MaxItemNum;i++)
		{
 				
			FillSampleID(nItemCount, wSampleID)	

	 		FillItemName(nItemCount, lpOpPointer+nRecordLen+c_ItemIDOff, c_ItemIDLen)

			
			if (*(lpOpPointer+nRecordLen+c_ResultOff)=='*')
			{
				FillResult(nItemCount, lpOpPointer+nRecordLen+c_ResultOff+1 , c_ResultLen-1) 
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

