//********************************************************************
//  本模块为Beckman公司的CX3化验仪的联机程序(Win95版)	

#include <stdio.h>
#include <Const.h> 
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		
#define	c_MaxItemNum		1		

#define c_FuncNumOff		8				
#define	c_StreamIDOff		4		
#define c_FuncNumLen		1		
#define c_SampleIDOff		56		
#define c_SampleIDLen		11		

#define c_TrayOff			34		
#define c_CupOff			37		
#define c_TrayLen			2		
#define c_CupLen			2		

	

#define c_ItemIDOff			59		
#define c_ItemIDLen			4		
#define c_ResultOff			122		
#define c_ResultLen			9		

#define c_SPEItemIDOff		52		
#define c_SPEItemIDLen		8		
#define c_SPEResultOff		76		

#define c_ResultFlagOff		108		
#define c_DeviceFlagOff		110		
#define c_ResultFlagLen		2		



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i=0,j,nItemCount,nResultLen,nPackLen=0;
	BYTE  btResultType,PackFun ;
	LPSTR lpPointer,lptemp;
	char PreFix_Result[20],TempResult[10]; 

	Init()
	ResetOpPointer()				

	ResetRxPointer()

	i=0;
 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	wSampleID=0;
	TranCommChar(XON)
	while (TRUE)
	{
		


		WaitEvent(EV_RXFLAG)

		TranCommChar(XOFF)
		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 
		j=atoi(lpOpPointer+c_FuncNumOff);
	switch(j)
	{
		case 1:
			if (strncmp(lpOpPointer+c_StreamIDOff ,"702",3)==0)
			{
				i =atoi(lpOpPointer+c_SampleIDOff);
				wSampleID =(i==0) ? wSampleID+1 : i;//if i==0 then wSampleID++ else wSampleID=i


				GetLocalTime(&SystemTime);		
				nItemCount =0;
				ResetRxPointer() 	

				TranCommChar(XON)
				continue;
			}
			else	
			{
				nItemCount =0;
				ResetRxPointer() 	
//				PurgeComm( hComm, PURGE_RXCLEAR );			
				TranCommChar(XON)			
				continue;
			}
		case 3:

//填样本号
			FillSampleID(nItemCount, wSampleID)	
//填项目名称
			FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)	
//填结果
			FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)
//填状态注解
//			FillComment(nItemCount,lpOpPointer+c_ResultFlagOff,c_ResultFlagLen )
//填日期
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		
/*			
//填状态注解
			if (strcmp(lpOpPointer+c_ResultFlagOff ,"NR")==0)

			nItemCount++;

			if(nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
			nItemCount=0;	
			ResetRxPointer()		

//		PurgeComm( hComm, PURGE_RXCLEAR );	
			TranCommChar(XON)
			continue;//处理END
		case 11:	
		case 13:
//填样本号
			FillSampleID(nItemCount, wSampleID)	
//填项目名称
			FillItemName(nItemCount,lpOpPointer+c_SPEItemIDOff ,c_SPEItemIDLen)	
//填结果
			FillResult(nItemCount,lpOpPointer+c_SPEResultOff,c_ResultLen)
//填日期
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nItemCount++;
			if(nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
			nItemCount=0;	
			ResetRxPointer()		
			TranCommChar(XON)	
		default:
			nItemCount =0;
			ResetRxPointer() 	
//			PurgeComm( hComm, PURGE_RXCLEAR );			
			TranCommChar(XON)			
			continue;
	}


	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

