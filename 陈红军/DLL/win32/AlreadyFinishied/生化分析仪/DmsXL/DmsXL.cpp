// DmsXL.cpp : Defines the entry point for the DLL application.
//Dimension Clinical Chemistry SYSTEM
//美国德林公司 Dimension XL 生化分析仪

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"

#define c_RxBufferLength	1024 //定义缓冲区大小		
#define	c_MaxItemNum		100  //定义项目数
           
extern HANDLE  hLogFile;
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
	//计算机发送的消息。
	//result acceptance message
	char ResultAccept[9] ={2,'M',28,'A',28, 28,'E','2', 3};
	char ResultReject[10]={2,'M',28,'R',28,'1',28 ,'2','4',3};
	//NO request message 
	char NoRequestMsg[6] ={2,'N',28,'6','A',3};
	// Sample Request Message.'D'
	// Wait Message 
	char WaitMsg[6]={2,'W',28,'7','3',3};

	SYSTEMTIME SystemTime;
	char TempWorkingID[7];
	char LSampleID[10];//指向样本号(字符串)的指针
	/*
	仪器发送的消息类型：
	1.Poll Message               'P'
	2.Request acceptance message 'M'
	3.Query Message				 'I'
	4.Result Message			 'R'
	5.Calibration Result Message 'C'
	*/
	bool bLogFile =FALSE;
	LPSTR  lptemp1;
	Init()
	ResetOpPointer()				

	if(NewLogFile("DmsXl.txt"))
		bLogFile=TRUE;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	
	while (TRUE)
	{

		WaitEvent(EV_RXFLAG)//等待接受数据包
		GetInQueueLength(dwReadLength)//分析数据
		ReadCommBlock(dwReadLength)
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}
	
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		//上面函数用来定位包起始位在接收缓冲区中的位置.???
		if (lpOpPointer[0]!=STX) 
		{	
			if(lptemp1!=NULL)
			{
				dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
				memmove( lpOpPointer, lptemp1,dwRxBufferLength );			
			}
		}
		//calculate checksum.
		nCheckSum =0;//lpOpPointer[0];
		i=1;
		do 
		{
			if (lpOpPointer[i+2] ==ETX )
			  break;
			nCheckSum +=lpOpPointer[i];
			nCheckSum=nCheckSum%256;	   
		   i++;
		}while(TRUE);

		if (nCheckSum!=0)
		{
			CK1=lpOpPointer[i]-0x30;
			if(CK1>9)
				CK1 -= 7;
		    CK2 = lpOpPointer[i+1]-0x30;
			if(CK2>9)
				CK2 -= 7;
			RxCheckSum = CK1 * 16;
		    RxCheckSum += CK2;
			if (RxCheckSum!=nCheckSum)
			{
				PurgeComm( hComm, PURGE_RXCLEAR );		
				WriteCommChar(NAK)
				continue;
			}
			
		}
		switch(*(lpOpPointer+1))//包类型
		{
		case 'P'://POLL Message
			WriteCommChar(ACK)
			Sleep(500);
			if(*(lpOpPointer+9)=='1')//first poll
			{
				if(*(lpOpPointer+11)=='1')//request
				{
					WriteCommBlock(NoRequestMsg,6)
				}
				else
				{
				}
			}
			else//first poll
			{
				if(*(lpOpPointer+11)=='1')//request
				{
					WriteCommBlock(NoRequestMsg,6)
				}
				else
				{
				}
			}
			
			break;

		case 'R'://Result Message  结果处理
			WriteCommChar(ACK)
			j=0;
			for(i=0;i<dwReadLength;i++)
			{
				if (*(lpOpPointer+i)==28)
					j++;
			}
			FSPos=new int[j];
			j=0;
			for(i=0;i<dwReadLength;i++)
			{
				if (*(lpOpPointer+i)==28)
				{
					FSPos[j]=i;
					j++;
				}
			}
			SumOfResult=(j-11)/4;
			if(SumOfResult<1)
				SumOfResult=1;
			if(j>15)//结果数组中具有一个以上结果。
			{
				GetLocalTime(&SystemTime);
				iSampleID=StrToInt(lpOpPointer+FSPos[2]+1,FSPos[3]-FSPos[2]-1);
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
			 //	memset(&TempWorkingID[1],0x30,5);
			//	strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
				TempWorkingID[6]='\0';	
				for(nItemCount=0;nItemCount<SumOfResult;nItemCount++)
				{
					FillItemName(nItemCount, lpOpPointer+FSPos[11 + 4 * nItemCount-1]+1, FSPos[12 + 4 * nItemCount-1]- FSPos[11 + 4 * nItemCount-1] -1)

                    FillResult(nItemCount,lpOpPointer+FSPos[12 + 4 * nItemCount-1] + 1, FSPos[13 + 4 * nItemCount-1] - FSPos[12 + 4 * nItemCount-1] -1)

					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

					OutResult[nItemCount].ItemNo=nItemCount+1;

					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);

				}
				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				delete FSPos;
			}
		
			nItemCount=0;
			WriteCommChar(ACK)
			break;
		case 'M':
		case 'I':
		case 'C':
			WriteCommChar(ACK)
			break;
		}
		
	
		ResetRxPointer()
		ResetOpPointer()
		

	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
	//设定串口监控事件
}