// Array360.cpp : Defines the entry point for the DLL application.
////********************************************************************
//  本模块为Beckman公司的Array化验仪(血清免疫蛋白)的联机程序(Win95版)
//  描述: 全称：Array Protein System （360/360E） 
 

#include "stdafx.h"

#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"

#define c_RxBufferLength	256	
#define	c_MaxItemNum		30	
#define c_FuncNumOff		1	
#define c_SampleIDLen       15 
#define c_SampleIDOff       7
#define c_ItemIDOff         4
#define c_ItemIDLen         3	
#define c_ResultOff         20
#define c_ResultLen         6	
#define c_IndicatorOff      36	

#define	c_IndicatorLen		2

extern HANDLE hLogFile;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}






DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,j,nItemCount,nResultLen,SampleIDLen;
	BYTE  PackFun ;
	BOOL	OddFlag=FALSE;
	char *p_ACK1="\x10\x31";//接到奇记录的应答包
	char *p_ACK0="\x10\x30";//接到偶记录的应答包
	char TempWorkingID[7];
	bool bLogFile;
	char LSampleID[7];
	LPSTR lpPointer;
	Init()
	ResetOpPointer()				

	ResetRxPointer()
    if(NewLogFile("Array360.txt"))
	{
		bLogFile=TRUE;
	}
	wSampleID=0;
 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	WriteCommChar(XON)
	while (TRUE)
	{
		WaitEvent(EV_RXFLAG)
		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		OddFlag=!OddFlag;
		PackFun=*(lpOpPointer+c_FuncNumOff);
		j=atoi(lpOpPointer+c_FuncNumOff);
		switch(j)
		{
			case 20:
			
				OddFlag=FALSE;
				nItemCount =0;
				ResetRxPointer() 	
				WriteCommBlock(p_ACK1 ,strlen(p_ACK1))
				continue;
			case 21:
				i =atoi(lpOpPointer+c_SampleIDOff);
				nItemCount =0;
				if (i>0) 
				{
					wSampleID=i;
				}
				else
				{
					wSampleID=wSampleID+1;
				}
				IntToStr(&LSampleID[0],wSampleID);
				TempWorkingID[0]=lpDevice->DevItemType;
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
			    TempWorkingID[6]='\0';
			
				GetLocalTime(&SystemTime);
				nItemCount =0;
				ResetRxPointer() 	
				if(OddFlag)
				{
					WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	
				}
				else
				{
					WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	
				}
				continue;
			case 22://处理结果包

				FillSampleID(nItemCount, wSampleID)	
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
				FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)	
				lpPointer=lpOpPointer+c_ResultOff;
				nResultLen=c_ResultLen;
				DeleSpace(nResultLen)
				FillResult(nItemCount, lpPointer ,nResultLen )
				FillComment(nItemCount,lpOpPointer+c_IndicatorOff,c_IndicatorLen )
				if(OutResult[nItemCount].ItemID[2]==0x20)
				{
					OutResult[nItemCount].ItemID[2]='\0';
				}
				OutResult[nItemCount].ItemNo=nItemCount+1;
				if (*(lpOpPointer+c_IndicatorOff+1)=='A' )
					FillErrorCode(nItemCount, c_UnReliable)	
				else 
					FillErrorCode(nItemCount, c_Reliable)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nItemCount++;
				ResetRxPointer()		
				if(OddFlag)
				{
					WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	//
				}
				else
				{
					WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	//
				}
				continue;
			case 99://将结果放入队列
				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				nItemCount=0;	
				ResetRxPointer() 	
				if(OddFlag)
				{
					WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	//SEND DEL_0
				}
				else
				{
					WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	//SEND DEL_1
				}
				continue;
				//一个样本结束
			default:
				ResetRxPointer() 	
				if(OddFlag)
				{
					WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	//SEND DEL_0
				}
				else
				{
					WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	//SEND DEL_1
				}			
				continue;
		}

	}
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

