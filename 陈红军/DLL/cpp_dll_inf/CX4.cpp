//********************************************************************
//  本模块为Beckman公司的CX4/CX5化验仪的联机程序(Win95版) 
 

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	800		
#define	c_MaxItemNum		1		

#define c_FuncNumOff		8					
#define	c_StreamIDOff		4		
#define c_FuncNumLen		2		
//#define c_SampleIDOff		72		
//#define c_SampleIDLen		11		

#define c_03TrayOff			0x2d		
#define c_03CupOff			0x30		
#define c_TrayLen			2			
#define c_CupLen			2			
#define c_11TrayOff			0x23		
#define c_11CupOff			0x26		
//#define c_13TrayOff			0x2d		
//#define c_13CupOff			0x30		

#define c_ItemIDOff			51		
#define c_ItemIDLen			4		
#define c_ResultOff			76		
#define c_ResultLen			9		

#define c_ResultFlagOff		91		
#define c_DeviceFlagOff		94		
#define c_ResultFlagLen		2		

#define c_SPE11ItemIDOff		44		
#define c_SPE11ItemIDLen		2		
#define c_SPE11ResultOff		50		

#define c_SPE13ItemIDOff		51	
#define c_SPE13ItemIDLen		3		
#define c_SPE13ResultOff		76		

#define	c_CupsOfOneTray			10		
#define	c_MultiConst			100		


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i=0,j,nItemCount,nResultLen,nPackLen=0,nDataOff=0;
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
 


		nItemCount =0;
		j=atoi(lpOpPointer+c_FuncNumOff);
	switch(j)
	{
		case 1:
			if (strncmp(lpOpPointer+c_StreamIDOff ,"402",3)==0)
			{
//				i =atoi(lpOpPointer+c_SampleIDOff);
//				wSampleID = (i==0) ? wSampleID+1 : i;//if i==0 then wSampleID++ else wSampleID=i

//				wSampleID =((int)StrToInt(lpOpPointer+ c_TrayOff, c_TrayLen )-1)
//				*c_CupsOfTray+(int)StrToInt(lpOpPointer+c_CupOff ,c_CupLen );

//				GetLocalTime(&SystemTime);		
				nItemCount =0;
				ResetRxPointer() 	
//				PurgeComm( hComm, PURGE_RXCLEAR );

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
		case 13:

			GetLocalTime(&SystemTime);		
			wSampleID =((int)StrToInt(lpOpPointer+ c_03TrayOff, c_TrayLen )-1)
				*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_03CupOff ,c_CupLen );
			FillSampleID(nItemCount, wSampleID)	
//填项目名称
			FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)	
//填状态注解
//			FillComment(nItemCount,lpOpPointer+c_ResultFlagOff,c_ResultFlagLen )
//填日期
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		if (j==4)
		{

			switch (*(lpOpPointer+c_ResultFlagOff))
			{
				case 'L'://LO
					PreFix_Result[0] =' ';
//					PreFix_Result[0] ='<';
					break;
				case 'H'://HI
					PreFix_Result[0] =' ';
//					PreFix_Result[0] ='>';
					break;
				default :
					PreFix_Result[0] =' ';
			}
			PreFix_Result[1] ='\0';
//			lpPointer=lpOpPointer+c_ResultOff;
			nResultLen=c_ResultLen;

			DeleZero(lpOpPointer, nResultLen)
			strncpy(TempResult, lpPointer,nResultLen );
			TempResult[nResultLen ]=0;
			strcat(PreFix_Result,TempResult );

			strncpy(OutResult[nItemCount].AssayResult,PreFix_Result , strlen(PreFix_Result));
			OutResult[nItemCount].AssayResult[strlen(PreFix_Result)] ='\0';
		}
		else
		{

//			lpPointer=lpOpPointer+c_ResultOff;
			nResultLen=c_ResultLen;
			DeleZero(lpOpPointer, nResultLen)			
			FillResult(nItemCount,lpPointer,nResultLen)
		}
			nItemCount++;
			break;
		case 11:	
			GetLocalTime(&SystemTime);		
			if (strncmp(lpOpPointer+c_StreamIDOff ,"402",3)==0 )
			{
//填样本号
				wSampleID =((int)StrToInt(lpOpPointer+ c_11TrayOff, c_TrayLen)-1)
					*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_11CupOff ,c_CupLen );
				FillSampleID(nItemCount, wSampleID)	
//填项目名称
				FillItemName(nItemCount,lpOpPointer+c_SPE11ItemIDOff ,c_SPE11ItemIDLen)	
//填结果
				FillResult(nItemCount,lpOpPointer+c_SPE11ResultOff,c_ResultLen)
//填日期
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nItemCount++;
				break;
			}
			else	
			{
				nItemCount =0;
				ResetRxPointer() 	
//				PurgeComm( hComm, PURGE_RXCLEAR );			
				TranCommChar(XON)			
				continue;
			}
		default:
			nItemCount =0;
			ResetRxPointer() 	
//			PurgeComm( hComm, PURGE_RXCLEAR );			
			TranCommChar(XON)			
			continue;
	}

	if(nItemCount>0)
	{
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;
	}
	nItemCount=0;	
	ResetRxPointer()		
	TranCommChar(XON)
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

