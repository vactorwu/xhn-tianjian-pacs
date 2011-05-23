//********************************************************************
//  模块: ylt200.cpp
//  描述: 本模块为优利特200尿十联化验仪的联机程序(Win95版)	
 
#include <stdio.h>     
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	300		
#define	c_MaxItemNum		11		
#define c_FirstItemCodeOff	46		
#define	c_NextItemCodeDlt	21		
//#define c_SequenceIDOff		6
//#define c_SequenceIDLen		3
#define c_SampleIDOff		5
#define c_SampleIDLen		3
#define c_AssayDateMonthOff		10	
#define c_AssayDateDayOff		13	
#define c_AssayDateYearOff		18								
#define c_AssayDateHourOff		33	
//#define c_ColorOff			7
//#define c_ColorLen			10
//#define c_ClarityOff		9
//#define c_ClarityLen		10
//#define c_ItemIDOff			0		
#define c_ItemIDLen			3
#define c_ResultOff			4		
#define c_ResultLen			14
//#define c_LineLen			22
//#define c_CheckSumLen		2
//

//static const CHAR * _gItemID[c_ItemNum] =
//      {"pH ","NIT","GLU","?c ","?G ","BLD","PRO","BIL","URO","KET","?BC"};    
//     {"pH ","NIT","GLU","Vc ","SG ","BLD","PRO","BIL","URO","KET","WBC"};
//


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nDataOff, nResultLen,k;
	BYTE	btCalCheckSum, btRxCheckSum;
	LPSTR	lptemp,lptemp1,hRXBuffer=(LPSTR)RxBuffer;
//
	Init()
	ResetOpPointer()				
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	ClearCommRx()
	TranCommChar(XON)
	nItemCount=0;
	while (TRUE)
	{

		ResetRxPointer()		
		ResetOpPointer()

		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)

		for (i=0;*lpOpPointer!=STX;lpOpPointer++,i++); 
		memmove(hRXBuffer , lpOpPointer ,  dwRxBufferLength-i );

		lptemp=strchrnb(lpOpPointer,LF,1,10);

		SystemTime.wDay	 =(WORD)StrToInt(lptemp +c_AssayDateDayOff,2);
		SystemTime.wMonth=(WORD)StrToInt(lptemp +c_AssayDateMonthOff,2);
		SystemTime.wYear =(WORD)StrToInt(lptemp +c_AssayDateYearOff,2);
		SystemTime.wHour =(WORD)StrToInt(lptemp +c_AssayDateHourOff ,2);

		wSampleID =(int)StrToInt( lptemp+c_SampleIDOff, c_SampleIDLen);		

		lptemp=lptemp+c_FirstItemCodeOff ;

		while( *(lptemp)!=LF )	
		{
			FillSampleID(nItemCount, wSampleID)	

			FillItemName(nItemCount, lptemp , c_ItemIDLen)	
			lptemp1=lptemp+c_ResultOff;

for (nResultLen=0 ;(*lptemp1>='+' && *lptemp1<='9') || *lptemp1==' ';lptemp1++,nResultLen++) ;

 			FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen) , nResultLen)	
//			FillResult(nItemCount,lptemp1 , c_ResultLen)		

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)
			nItemCount ++;	
			lptemp +=c_NextItemCodeDlt ;
		}			
		
		

		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		nItemCount=0;
		PurgeComm( hComm, PURGE_RXCLEAR );	
		WriteCommChar(ACK)
		Sleep(100);
		TranCommChar(XON)
	}
 
	return TRUE;
} // END OF COMMWATCHPROC()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}


 

