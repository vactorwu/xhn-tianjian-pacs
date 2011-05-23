// H100.cpp : Defines the entry point for the DLL application.
//长春迪瑞公司尿液分析仪H100

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "LogFile.h"
#include "str.h"
#define c_RxBufferLength	1024 //定义缓冲区大小		
#define	c_MaxItemNum		18  //定义项目数
#define Working_idOff       7
#define ResultOff           7
#define resultLength        8
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
//监控串口接收数据，并完成数据格式分析，将结果放入结果队列
	CommData
	DWORD   i,j;
    int * FSPos;//结果中的域分割符在字符串中的位置
	int nItemCount,nCheckSum,CK1,CK2,RxCheckSum,iSampleID;//校验和计算
	int SumOfResult,SampleIDLen;//接收样本具有的结果数
	SYSTEMTIME SystemTime;
	bool bLogFile;
	char TempWorkingID[7];
	char LSampleID[10];//指向样本号(字符串)的指针
	Init()
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	bLogFile=NewLogFile("H100.txt");	
	while (TRUE)
	{


		WaitEvent(EV_RXFLAG)//等待接受数据包
		GetInQueueLength(dwReadLength)//分析数据
		ReadCommBlock(dwReadLength)
		if (dwReadLength<=10)
			continue;
		//数据校验
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		j=0;
		for(i=0;i<dwReadLength;i++)
		{
			if (*(lpOpPointer+i)==0x0d)
				j++;
		}
		FSPos=new int[j];
		j=0;
		for(i=0;i<dwReadLength;i++)
		{
			if (*(lpOpPointer+i)==0x0d)
			{
				FSPos[j]=i;
				j++;
			}
		}
		GetLocalTime(&SystemTime);
		iSampleID=StrToInt(lpOpPointer+FSPos[0]+Working_idOff,3);
		_itoa(iSampleID,LSampleID,10);
		TempWorkingID[0]=lpDevice->DevItemType;
		SampleIDLen=strlen(LSampleID);
		memset(&TempWorkingID[1],0x30,5);
		if(SampleIDLen>=6)
		{
			strncpy(&TempWorkingID[1],&LSampleID[SampleIDLen-5],5);
		}
		else
			strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
		TempWorkingID[6]='\0';
		nItemCount=0;
		for(j=1;j<12;j++)
		{
			//结果处理		
                FillResult(nItemCount,lpOpPointer+FSPos[j] + ResultOff, resultLength)

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

				OutResult[nItemCount].ItemNo=nItemCount+1;

				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
				nItemCount++;

		}
		for(j=0;j<11;j++)
		{
			OutResult[j].ItemID[0]=0x41+j;
			OutResult[j].ItemID[1]='\0';				
		}

		//结果入库
		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;	
		ResetRxPointer()		
		//清空缓冲区
		delete FSPos;

	}
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =0x0c;
	//设定串口监控事件
}