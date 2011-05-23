//********************************************************************
//  模块: clk100.cpp
//  描述: 本模块为clinitek100尿十联化验仪的联机程序(Win95版)
//	函数:
 
#include <stdio.h>     
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		
#define	c_MaxItemNum		12		
#define c_DataOff			24
#define c_SequenceIDOff		6		
#define c_SequenceIDLen		3
//#define c_SampleIDOff		29
//#define c_SampleIDLen		10
#define c_ColorOff			9		
#define c_ColorLen			11
#define c_ClarityOff		9
#define c_ClarityLen		11
#define c_ItemIDOff			0		
#define c_ItemIDLen			3
#define c_ResultOff			3		
#define c_ResultLen			12
#define c_Col_Cla_LineLen	22	
#define c_SEQLineLen		21	
#define c_IDLineLen			17	
#define c_RESULTLineLen		20	
#define c_CheckSumLen		2

  
 



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nDataOff, nResultLen,k;
	BYTE	btCalCheckSum, btRxCheckSum;
	LPCSTR lpPointer,lphkf;


	Init()
	ResetOpPointer()				
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	ClearCommRx()
	WriteCommChar(XON)
	while (TRUE)
	{

		ResetRxPointer()		
		ResetOpPointer()

			
			WaitEvent(EV_RXFLAG)
			GetInQueueLength(dwReadLength)



		ReadCommBlock(dwReadLength)



		k=dwRxBufferLength-3;	
		btCalCheckSum =0;
		for ( i=1; i<k;i++)  btCalCheckSum +=lpOpPointer[i];
			btRxCheckSum =(BYTE)HStrToInt(lpOpPointer+k,c_CheckSumLen);
		if (btCalCheckSum !=btRxCheckSum)
		{
			ClearCommRx()
			WriteCommChar(DC2)
			continue;
		}


		GetLocalTime(&SystemTime);
	
		for (i=0; i<c_MaxItemNum;i++)
		{
			memset(LPSTR(OutResult[i].AssayResult), 0, c_MaxResultLength);
			memset(LPSTR(OutResult[i].Comment), 0, c_MaxCommentLength);
		}
		nDataOff = c_DataOff;

		if (*(lpOpPointer+nDataOff)=='I')
		{
			nDataOff +=c_IDLineLen;	

//			wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		}
//		else	
	
		wSampleID =StrToInt(lpOpPointer+c_SequenceIDOff, c_SequenceIDLen);
		nItemCount =0;
		lphkf=lpOpPointer+nDataOff;
		if (strncmp(lphkf,"Col",3)==0)	
		{
			lpPointer=lphkf+c_ColorOff;

			for(nResultLen =c_ColorLen; *lpPointer==SPACE; nResultLen--, lpPointer++); //去掉前导空格
//填项目名
			FillItemName(nItemCount,"COLOR" ,5)
//填样本号
			FillSampleID(nItemCount, wSampleID)
//填结果串 
			FillResult(nItemCount, lpPointer,  nResultLen)
//填日期,小时
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nItemCount++;
			nDataOff +=c_Col_Cla_LineLen;
			lphkf=lpOpPointer+nDataOff;
		}
			
		if (strncmp(lphkf,"Cla",3)==0)	
		{
			lpPointer=lphkf+c_ClarityOff;

			for(nResultLen =c_ClarityLen; *lpPointer==SPACE; nResultLen--, lpPointer++); //去掉前导空格
 			FillItemName(nItemCount,"CLARITY", 7)
			FillSampleID(nItemCount, wSampleID)
			FillResult(nItemCount, lpPointer,  nResultLen )
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nItemCount++;
			nDataOff +=c_Col_Cla_LineLen; 
			lphkf=lpOpPointer+nDataOff;
		}
		
		
		lphkf +=2;
		while( *(lphkf+2)!=ETX )
		{
			FillSampleID(nItemCount, wSampleID)	//样本号

			FillItemName(nItemCount,lphkf , c_ItemIDLen)

			lpPointer =lphkf+c_ResultOff;

			DeleSpace(c_ResultLen)
//			for(nResultLen =c_ResultLen; *lpPointer==SPACE; nResultLen--, lpPointer++); //去掉前导空格
 			FillResult(nItemCount, lpPointer,  nResultLen)
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nItemCount++;
			lphkf +=c_RESULTLineLen;
		}			

		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;

		ClearCommRx()
//		TranCommChar(ACK)
		WriteCommChar(ACK)
//		Sleep(1);
//		TranCommChar(XON)
//		WriteCommChar(XON)
	}
 
	return TRUE;
} // END OF COMMWATCHPROC()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

