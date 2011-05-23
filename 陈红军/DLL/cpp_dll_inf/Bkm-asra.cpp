//********************************************************************
//  本模块为Beckman公司的asra化验仪的联机程序(Win95版)
//  描述: 
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		
#define	c_MaxItemNum		10		
#define c_FuncNumOff		1			
#define c_FuncNumLen		2		
#define c_SampleIDOff		7		
#define c_SampleIDLen		9		
#define c_ItemIDOff			4		
#define c_ItemIDLen			3		
#define c_ResultOff			8		
#define c_ResultLen			6		
#define c_ResultTypeOff		35		
#define c_ResultTypeLen		2		
#define c_AssayDateOff		17		
#define c_RxCheckSumLen		2		
#define	nStateCodeOff		27 		
#define	nStateCodeLen		2		
#define	nNormalCodeOff		21		
#define	nNormalCodeLen		2		

#define c_ResultTypeNum		3
static const CHAR * _gResultTypeID[c_ResultTypeNum] =
	{"RO", "ST", "CO"};
static const int _gResultTypeCode[c_ResultTypeNum] =
	{ c_TypeRoutine, c_TypeStat, c_TypeQC};


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,nItemCount,nResultLen;
	BYTE  btResultType ;
	LPSTR lpPointer,lptemp;
	 

	Init()
	ResetOpPointer()				

	ResetRxPointer()

 
 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	while (TRUE)
	{



		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 


		if (*(lpRxPointer-8)!='[') continue;	

				wSampleID =(int)StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
			
		for(i =0; i<c_ResultTypeNum; i++)
		{
			if (strncmp(_gResultTypeID[i], lpOpPointer+c_ResultTypeOff, c_ResultTypeLen)==0)
			{
			btResultType =_gResultTypeCode[i];	
			break;
			}
		}

		SystemTime.wDay	 =(WORD)StrToInt(lpOpPointer+c_AssayDateOff,2);
		SystemTime.wMonth=(WORD)StrToInt(lpOpPointer+c_AssayDateOff+2,2);
		SystemTime.wYear =(WORD)StrToInt(lpOpPointer+c_AssayDateOff+4,2);
		SystemTime.wHour =(WORD)StrToInt(lpOpPointer+c_AssayDateOff+7,2);					


		lptemp=strchrnb(lpOpPointer,LF,1,50)+1;				
		while(*(lptemp+2)=='2')	
		{

			FillSampleID(nItemCount, wSampleID)	
//填项目名称
			FillItemName(nItemCount, lptemp +c_ItemIDOff, c_ItemIDLen)	

			lpPointer= lptemp+c_ResultOff ;
			for(nResultLen =c_ResultLen; *lpPointer==SPACE; nResultLen--, lpPointer++);  
//填结果
			FillResult(nItemCount,lpPointer ,  nResultLen)
//填日期
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)
//填样本结果类型
			FillResultType(nItemCount, btResultType)

			if (strncmp(lpPointer ,"T.O",3)==0 || strncmp(lpPointer,"*0000",5)==0)
			{	
				FillErrorCode(nItemCount,c_UnReliable )		

				FillComment(nItemCount,  lptemp+nStateCodeOff , nStateCodeLen)
			}

			else	FillComment(nItemCount,  lptemp+nNormalCodeOff , nNormalCodeLen)

			lptemp=strchrnb(lptemp ,LF,1,50)+1;				
			nItemCount++;
		}	//END--WHILE


 
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;

 
		nItemCount=0;	
		ResetRxPointer()		
		ResetOpPointer()
		PurgeComm( hComm, PURGE_RXCLEAR );
 
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

