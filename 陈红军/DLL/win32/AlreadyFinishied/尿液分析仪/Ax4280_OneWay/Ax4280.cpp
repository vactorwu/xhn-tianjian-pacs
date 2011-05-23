// Ax4280.cpp : Defines the entry point for the DLL application.
//
#include <stdlib.h>
#include "stdafx.h"
#include "const.h"
#include "device.h"
#include "logfile.h"
#include "str.h"
#include "devmacro.h"

#define c_RxBufferLength	800
#define	c_MaxItemNum		15
//First Block
#define c_KindsOfMeas_Off   50
#define c_kindsOfMeas_Len   2 
#define c_MeasNo_Off        53
#define c_MeasNo_Len        4
#define c_RackNoOff         58
#define c_RackNoLen         3
#define c_PortNoOff         62
#define c_PortNoLen         2
#define c_TestStripTypeOff  65
#define c_TestStripTypeLen  10
//Second Block
#define c_MeasResultLen     25	
#define c_ItemIdOff         1
#define c_ItemIdLen         4
#define c_NormIndOff        7
#define c_NormIndLen        1
#define c_SemiQuant_Symbol_Off  8
#define c_SemiQuant_Symbol_Len  6
#define c_ConcentrationOff      13
#define c_ConcentrationLen      6
#define c_MesurementUnitOff     19
#define c_MesurementUnitLen     6
//Third Block
#define c_Tubidity_ItemIdLen   126
#define c_Tubidity_ItemIdOff   4
#define c_TurbABNOff           136
#define c_TurbABNLen           1

//Fourth Block 			
#define c_SG_ITEM_OFF          1
#define c_SG_ITEM_LEN          4
#define c_SGABN_INDC           7
#define c_SGGravity_Off        14
#define c_SGGravity_Len        12
#define c_ColorOff             27
#define c_ColorLen             12
#define c_TempOff            40
#define c_TempLen            2
#define c_TempVALOFF           42
#define c_TEMPVAL_Len          4


extern  HANDLE  hLogFile;
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
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
	char SampleID[5];
 	SYSTEMTIME SystemTime;
	bool bLogFile=FALSE;
	int	 nItemCount;
	int  i,j,SampleIDLen;
	int PackageNo;
	bool isdataPacket=FALSE;
	Init() 
	if(NewLogFile("Ax4280.txt"))
		bLogFile=TRUE;
	nItemCount = 0;
	int ReciveChars=0;
	ResetOpPointer()				
 	FillDevName()	
	TranCommChar(XON)			
	SetCommMask(hComm,EV_RXFLAG);
	PackageNo=0;
	while (TRUE)
	{						
		WaitEvent(EV_RXFLAG)//等待接受数据包
		TranCommChar(XOFF)//中断仪器传送
		GetInQueueLength(dwReadLength)//分析数据
		lpOpPointer=lpRxPointer;
		ReadCommBlock(dwReadLength)
		if (bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		//处理结果数据！
		//处理First Block
		GetLocalTime(&SystemTime);
		wSampleID=StrToInt(lpOpPointer+c_MeasNo_Off,c_MeasNo_Len);
		TempWorkingID[0]=lpDevice->DevItemType;
		itoa(wSampleID,&SampleID[0],10);
		SampleIDLen=strlen(SampleID);
		memset(&TempWorkingID[1],0x30,5);
		strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
		TempWorkingID[6]='\0';
		
		//处理Second Block
		lpOpPointer=lpOpPointer+76;
		for(j=0;j<6;j++)
		{
			
			strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
			FillItemName(nItemCount,lpOpPointer+j*c_MeasResultLen+c_ItemIdOff,c_ItemIdLen)
			//判断项目名是否为空，若唯空，则跳过！
			for(i=0;i<4;i++)
			{
				if(OutResult[nItemCount].ItemID[3-i] != 0x20)
					break;
				else
					OutResult[nItemCount].ItemID[3-i]=0;
			}
			if(i!=4)
			{
				FillResult(nItemCount,lpOpPointer+j*c_MeasResultLen+c_SemiQuant_Symbol_Off,18)
				FillDate(nItemCount, SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
			}
		}
	
			//处理Third Block
		lpOpPointer=lpOpPointer+152;
		for(j=0;j<6;j++)
		{
			
			strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
			FillItemName(nItemCount,lpOpPointer+j*c_MeasResultLen+c_ItemIdOff,c_ItemIdLen)
			for(i=0;i<4;i++)
			{
				if(OutResult[nItemCount].ItemID[3-i] != 0x20)
					break;
				else
					OutResult[nItemCount].ItemID[3-i]=0;
			}
			if(i!=4)
			{
				FillResult(nItemCount,lpOpPointer+j*c_MeasResultLen+c_SemiQuant_Symbol_Off,18)
				FillDate(nItemCount, SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
			}
		}
		
		//处理Fourth Block
		lpOpPointer=lpOpPointer+152;
		strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
		FillItemName(nItemCount,lpOpPointer+c_SG_ITEM_OFF,c_SG_ITEM_LEN)
		FillDate(nItemCount, SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		OutResult[nItemCount].ItemNo=nItemCount+1;
		FillResult(nItemCount,lpOpPointer+c_SGGravity_Off,c_SGGravity_Len);
		nItemCount++;
		strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
		FillItemName(nItemCount,"Color",5)
		FillDate(nItemCount, SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		OutResult[nItemCount].ItemNo=nItemCount+1;
		FillResult(nItemCount,lpOpPointer+c_ColorOff,c_ColorLen);
		nItemCount++;
		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;
		ResetRxPointer()
		TranCommChar(XON)//恢复仪器发送数据
	}
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;

}
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
	// 设置通信事件字符,当串口接收到该字符并移入缓冲区后,产生一个事件通知串口监控线程
	//此字符根据仪器不同而改变. 此注释  陈红军于2000.8.15 
}