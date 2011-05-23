//-----------------------------------------------------------------------//
//   ASTMInfe.cpp : Defines the entry point for the DLL application.     //
//																		 //
//   开发者				时	间				备	注                       //
//  --------          -----------          --------                      //
//   余  浩            2001-02-08           开	发                       //
//																		 //
//	 说明：开发符合ASTM E1394-91和ASTM E1381-94标准的接口驱动程序        //
//                                                                       //
//-----------------------------------------------------------------------//

#include	<stdio.h>
#include	<stdlib.h>
#include	"stdafx.h"
#include	"Const.h"
#include	"Device.h"
#include	"DevMacro.h"
#include	"str.h"
#include	"LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//引用外部变量来自 LogFile.cpp
extern		HANDLE  hLogFile;

//自定义的全局变量(分隔符定义)
char        Field_delimiter;
char        component_delimiter;
char        repeat_delimiter;
char        escape_character;

#define		c_RxBufferLength	512	
#define		c_MaxItemNum		60

//--------------------------------主入口函数-----------------------------------//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//-----------------------------------------------------------------------------//

//-------------------------------串口监控函数----------------------------------//
DWORD WINAPI CommMonitor( LPSTR lpData)
{
	char		TempWorkingID[c_MaxWorkingIDLength+1];
	char		TempWID[c_MaxWorkingIDLength+1];
	char        ItemID[6];
	char		ResultValue[20];
	char		DataBuffer[247];	 //结果数据缓冲区
	char		RecordBuffer[900];
	SYSTEMTIME	SystemTime;	
	int			nItemCount;		
	int			i;
	int         StrIDLen;
	int         FrameLen;            //单帧的长度
	int         TotalLen;            //帧总长度
	LPSTR		lpResult;
	LPSTR		lpSampleID;
	LPSTR		lpItemID;
	bool		bLogFile=FALSE;
	bool		isdataPacket=FALSE;  //是否为数据包标识	
	bool		bCheckSum;			 //校验和标示
	bool		bEventChar;
	
	CommData
	Init()
	nItemCount=0;			   //项目个数初始化为零
	TotalLen = 0;
	ResetOpPointer()	
	bEventChar = FALSE;
	if(NewLogFile("ASTMInfe.txt"))
		bLogFile=TRUE;	
 	FillDevName()					
 	SetCommWaitMask(EV_RXCHAR) 
	
	while (TRUE)
	{		
		beg_label:
		if(bEventChar==FALSE) 	WaitEvent(EV_RXCHAR)//等待接受数据包EV_RXCHAR,EV_RXFLAG-字符事件指定字符
		else					WaitEvent(EV_RXFLAG);			
		i=0;		 
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);			
			if(RxChar==ENQ)//用字符W代替ENQ
			{
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)			
				bEventChar = TRUE;
				SetCommWaitMask(EV_RXFLAG)//改变为指定的字符事件
				goto beg_label;
			}						
			if(RxChar==STX)			
				isdataPacket = TRUE;				
			if((RxChar==LF)&&(isdataPacket==TRUE)&&(bEventChar==TRUE))
			{			
				DataBuffer[i]=RxChar;
				i++;
				break;
			}
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;							
			}		
		}while(dwReadLength==1);
		
		isdataPacket = FALSE;
		dwRxBufferLength = i; 		
		lpOpPointer=&DataBuffer[0]; //取一帧数据

		if (bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		}

		//分析数据包		
		FrameLen = dwRxBufferLength;
		switch(*(lpOpPointer + FrameLen - 5))//判断帧类型，中间帧或最后帧
			{
			//中间帧
			case ETB:				
				strncpy(&RecordBuffer[TotalLen],&lpOpPointer[0],FrameLen);
				TotalLen = TotalLen + FrameLen;
				//应答				
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
			//最后帧
			case ETX:									
				strncpy(&RecordBuffer[TotalLen],&lpOpPointer[0],FrameLen);
				TotalLen = 0;
				//计算校验和
				bCheckSum = TRUE;
				if(bCheckSum == FALSE) 
				{
					//请求再次发送
					PurgeComm(hComm, PURGE_RXCLEAR);	
					WriteCommChar(NAK)
					break;
				}
				//取出数据部分
				SubString(&RecordBuffer[0] , lpOpPointer ,2, FrameLen - 5);
				//分析数据
				switch(*(lpOpPointer))
				{				
				case 'H': //头帧
					Field_delimiter = *(lpOpPointer+1);
					component_delimiter = *(lpOpPointer+3);
					repeat_delimiter = *(lpOpPointer+2);
					escape_character = *(lpOpPointer+4);
					break;
				case 'P': //病人信息帧
					//取工作单号
					/*
					lpSampleID = &TempWorkingID[0];
					GetResultStr(lpOpPointer, lpSampleID ,Sequence_Number);
					StrIDLen=strlen(lpSampleID);
					//向工作单中填充‘0’，使工作单长度保持在6个字符
					memset(&TempWorkingID[1],0x30,5);
					strncpy(&TempWorkingID[6-StrIDLen],&lpSampleID[0],StrIDLen);
					TempWorkingID[0]=lpDevice->DevItemType;				
		    		TempWorkingID[6]='\0';	
					*/
					break;
				case 'O': //试验申请帧
					//取工作单号
					lpSampleID = &TempWID[0];
					GetSampleStr(lpOpPointer, lpSampleID ,4,1);//specimen_no
					StrIDLen=strlen(lpSampleID);
					//向工作单中填充‘0’，使工作单长度保持在6个字符
					memset(&TempWorkingID[1],0x30,5);
					strncpy(&TempWorkingID[6-StrIDLen],&lpSampleID[0],StrIDLen);
					TempWorkingID[0]=lpDevice->DevItemType;				
		    		TempWorkingID[6]='\0';	
					break;
				case 'R': //结果数据帧		
					//取项目ID
					lpItemID = &ItemID[0];
					GetItemStr(lpOpPointer,lpItemID,Universal_Test_ID,5);
					StrIDLen=strlen(lpItemID);
					//填项目ID
					strncpy(&OutResult[nItemCount].ItemID[0],&lpItemID[0],StrIDLen);
					OutResult[nItemCount].ItemID[StrIDLen] = '\0';
					OutResult[nItemCount].ItemNo=nItemCount+1;				
					//取结果
					lpResult = &ResultValue[0];
					GetResultStr(lpOpPointer,lpResult,Measurement_Value);
					StrIDLen = strlen(lpResult);
					if(StrIDLen==0) break;
					//填结果
					FillResult(nItemCount, lpResult , StrIDLen) 
					GetSystemTime(&SystemTime);
					//填时间
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)					
					//填工作单号
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					nItemCount = nItemCount + 1;					
					break;
				case 'L': //结束帧
					//有结果时,写入结果
					if(nItemCount>0)
					{
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}			
					nItemCount = 0;
					bEventChar = FALSE;
					SetCommWaitMask(EV_RXCHAR)//改变为指定的字符事件
					break;
				}							
				//应答
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
				RecordBuffer[0] = '\0';				
			}				
	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;	
} 
//-----------------------------------------------------------------------------//


//-------------------------------设定字符事件----------------------------------//
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar = LF;////用字符Z代替LF
	//设定串口监控事件,对于符合ASTM底层标准字符事件可设为LF
}
//-----------------------------------------------------------------------------//