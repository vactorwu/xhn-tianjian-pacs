// Syn_ELISE.cpp : Defines the entry point for the DLL application.
//美国贝克曼公司的Synchron EL_ISE ELECTROLYTE  System 动态链接库

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"
#define c_RxBufferLength	800		//数据处理缓冲区长
#define	c_MaxItemNum		8		//一次最多处理的项目数

#define c_SequenceNoOff     8
#define c_SequenceNoLen     9

#define	c_TrayNumEOff		18		
#define	c_CupNumEOff		21		//杯号在杯尾包内的偏移
#define	c_TrayNumLen		2		//盘号长度
#define	c_CupNumLen			2		//杯号长度
#define c_RecordTypeOff     2       //记录类型偏移
#define c_FuncNumOff		9		//包类别第二字节偏移
#define	c_ItemNameOff		11		//非计算项目名偏移
#define	c_ItemNameLen		3		
#define	c_StreamIDOff		4	    //流类别偏移
#define	c_StreamIDLen		3	
#define	c_CalcItemNameOff	27		//计算项目名称偏移
#define	c_CalcItemNameLen	1		
#define	c_SampleNoOff		11		
#define	c_SampleNoEOff		25		//样本号在杯尾包的偏移
#define	c_ResultOff			15		//非计算结果偏移
#define	c_ResultLen			9		//结果长度
#define	c_CalcResultOff		17		//计算结果偏移
#define	c_TrayNumHOff		11		//盘号在杯头包内的偏移
#define	c_CupNumHOff		14		
#define	c_TrayNumEOff		18		
#define	c_CupNumEOff		21		//杯号在杯尾包内的偏移
#define	c_TrayNumLen		2		//盘号长度
#define	c_CupNumLen			2		//杯号长度
#define	c_CupHeaderPackLen	71		//杯头包长	
#define	c_TestResultPackLen 89		//结果包长
#define c_SPCalcuPackLen	33		//计算包长
#define	c_EndCupPackLen		41		//杯结束包长
#define	c_EndTrayPackLen	15		//盘结束包长
#define	c_CupsOfOneTray		40		

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
	SYSTEMTIME SystemTime;
	int	 nItemCount,TrayNum,CupNum;//样本项目号,盘号,杯号
	BYTE  SampleIDLen ;//数据包功能号,样本ID长度
	WORD	wSampleNo;
	bool blogfile;
	int  i,ByteRecieved;
	bool isdataPacket=FALSE;
	char  DataBuffer[200];// 数据缓冲区！
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
	char LSampleID[5];
	Init()
	
	ResetOpPointer()				
	
	ResetRxPointer()																																																																																																															
	

 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXCHAR)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	blogfile=NewLogFile("Syn_ELISE.txt");
	WriteCommChar(XON)
	while (TRUE)
	{
		WaitEvent(EV_RXCHAR)		
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar==STX)
				isdataPacket=TRUE;
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;
			}
			if(RxChar==ENQ||RxChar==EOT)
			{
				break;
			}
			if(((RxChar==ETX)|(RxChar==ETB))&(isdataPacket))
			{
				//DataBuffer[i]=RxChar;
				ByteRecieved=i;
				break;
			}	
		}while(dwReadLength==1);
		i=0;
		if((RxChar==ETX))
		{
			while(DataBuffer[i]!=STX)	i++;
		}
		lpOpPointer=&DataBuffer[i];
		if (blogfile)
		{
			WriteLogData(hLogFile,lpOpPointer,ByteRecieved);
		}

		switch (RxChar) 
		{
		case ENQ:
		//	Sleep(300);
			TransmitCommChar(hComm,ACK);
			break;
		case ETB:
		case ETX:
			//先进行校验的计算，若为正常接受则进行结果处理！
			//目前，由于资料校验和的计算方法与结果包中数据不符！尚未加校验计算
			/*
			    需要到仪器上读出数据包进行分析
			*/
			switch(*(lpOpPointer+2))
			{
			case 'H':
				break;
			case 'P':
				break;
			case 'O':
				nItemCount=0;
				TrayNum=(int)StrToInt(lpOpPointer+c_TrayNumEOff,c_TrayNumLen);				
				CupNum=(int)StrToInt(lpOpPointer+c_CupNumEOff,c_CupNumLen);			
				wSampleNo =(int)StrToInt(lpOpPointer+c_SequenceNoOff,c_SequenceNoLen);
				_itoa(wSampleNo,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				memset(&TempWorkingID[1],0x30,5);
				SampleIDLen=strlen(LSampleID);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';
						
				break;	//继续接收	
			case 'R':
				FillSampleID(nItemCount, wSampleNo)	
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);//填写工作单号
				OutResult[nItemCount].ItemNo=nItemCount+1;
				FillItemName(nItemCount,lpOpPointer+c_ItemNameOff ,c_ItemNameLen)	

				if (OutResult[nItemCount].ItemID[1]==124)//'|'  如果项目名称长度为1则添加字符串尾标志-ASCII 码值为 0
				{
					OutResult[nItemCount].ItemID[1]='\0';			
					FillResult(nItemCount,lpOpPointer+c_ResultOff-2,c_ResultLen)
				}
				else
					FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)

				OutResult[nItemCount].ItemNo=nItemCount+1;
//填日期
				GetSystemTime(&SystemTime); 
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nItemCount++;
				(*lpResultProcessRoutine)(lpDevice->nDevNO, &OutResult[nItemCount-1], 1);
				lpDevice->dwRecordCount+=1;  
				break;
			case 'L':
				break;
			}
		//	Sleep(300);
			TransmitCommChar(hComm,ACK);
			break;
		case EOT:
			//结果入队列
		default:
			TransmitCommChar(hComm,ACK);
			break;
		}
		PurgeComm( hComm, PURGE_RXCLEAR );	
	  
	}
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
	//设定串口监控事件
}

