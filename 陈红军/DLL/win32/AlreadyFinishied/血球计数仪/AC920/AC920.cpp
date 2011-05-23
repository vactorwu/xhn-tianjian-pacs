// AC920.cpp : Defines the entry point for the DLL application.
//SWELAB AC920EO 血球计数仪的联机程序。

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "LogFile.h"
#include "str.h"

#define c_RxBufferLength	 3000 //定义缓冲区大小		
#define	c_MaxItemNum	     16  //定义项目数
#define c_SampleIDOff        5
#define c_SampleIDLen        4
#define c_DataOff            35
#define c_OneDataLen         13
#define c_ItemIDoff          1
#define c_ItemResOff         2
#define c_ItemUnitsOff       7
#define c_ItemRemarksOff     10

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DWORD WINAPI CommMonitor( LPSTR lpData)
{
//监控串口接收数据，并完成数据格式分析，将结果放入结果队列
	int   nItemCount;
	int   ItemID;
	int   SampleIDLen,nDataOff;
	char SampleID[6],TempWorkingID[7];
	SYSTEMTIME  SystemTime;
	LPSTR  lpPointer;
	CommData

	Init()
	ResetRxPointer()
	ResetOpPointer()				
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	while (TRUE)
	{

		WaitEvent(EV_RXFLAG)
		GetInQueueLength(dwReadLength)//分析数据
		ReadCommBlock(dwReadLength)
		GetLocalTime(&SystemTime);
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		TempWorkingID[0]=lpDevice->DevItemType;
		_itoa(wSampleID,SampleID,10);
		SampleIDLen=strlen(SampleID);
		memset(&TempWorkingID[1],0x30,5);
		strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
		TempWorkingID[6]='\0';
		nDataOff =c_DataOff;
		nItemCount=0;
		for(int i =0; i<18 ;i++)//for===A
		{
			OutResult[nItemCount].ItemNo=nItemCount+1;
			memset(OutResult[nItemCount].AssayResult,0,20);
			strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			lpPointer=lpOpPointer+nDataOff;
			ItemID=*(lpPointer+1);
			switch(ItemID)
			{
			case  'D':
				strncpy(OutResult[nItemCount].ItemID,"RBC",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'E':
				strncpy(OutResult[nItemCount].ItemID,"HCT",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'F':
				strncpy(OutResult[nItemCount].ItemID,"MCV",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'G':
				strncpy(OutResult[nItemCount].ItemID,"HGB",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'H':
				strncpy(OutResult[nItemCount].ItemID,"MCH",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'I':
				strncpy(OutResult[nItemCount].ItemID,"MCHC",4);
				OutResult[nItemCount].ItemID[4]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'J':
				strncpy(OutResult[nItemCount].ItemID,"PLT",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'K':
				strncpy(OutResult[nItemCount].ItemID,"MPV",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'L':
				strncpy(OutResult[nItemCount].ItemID,"WBC",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'M':
				strncpy(OutResult[nItemCount].ItemID,"LYM",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'N':
				strncpy(OutResult[nItemCount].ItemID,"LYM%",4);
				OutResult[nItemCount].ItemID[4]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;				
			case  'O':
				strncpy(OutResult[nItemCount].ItemID,"MID",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'P':
				strncpy(OutResult[nItemCount].ItemID,"MID%",4);
				OutResult[nItemCount].ItemID[4]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'Q':
				strncpy(OutResult[nItemCount].ItemID,"GRA",3);
				OutResult[nItemCount].ItemID[3]='\0';
		
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case  'R':
				strncpy(OutResult[nItemCount].ItemID,"GRA%",4);
				OutResult[nItemCount].ItemID[4]='\0';

				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
				break;
			case 'U':
				strncpy(OutResult[nItemCount].ItemID,"RDW",3);
				OutResult[nItemCount].ItemID[3]='\0';
				
				strncpy(OutResult[nItemCount].AssayResult,lpPointer+c_ItemResOff,4);
				OutResult[nItemCount].AssayResult[4]='\0';
		
				nItemCount++;
	        default:
				break;			
			}
		
			nDataOff+=c_OneDataLen; 
	

		}	
		//结果处理


		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;	
		ResetRxPointer()	
		ResetOpPointer()
		ClearCommRx()//清空接收缓冲区
	

	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
	//设定串口监控事件
}