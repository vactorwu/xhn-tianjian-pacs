// Vitros250.cpp : Defines the entry point for the DLL application.
//
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "device.h"
#include "const.h"
#include "devmacro.h"
#include "str.h"
#include "Logfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define c_RxBufferLength	512	
#define	c_MaxItemNum		60	
#define c_SampleIDOff		29		
#define c_SampleIDLen		15	
#define c_SampleTypeOff       44
#define c_ResultOff			9		
#define c_ResultLen			8		
	
#define c_ItemIDOff			5		
#define c_ItemIDLen			4		
		
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
	//校验核的计算方法，求每个字符的算术和，取256模
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
	char    DataBuffer[200];
	CommData
	char  Ackchar=0x2B;//'+'
	char  Nakchar=0x2D;//'-'
	char  Cancelchar=0x3F;//'?'
 	SYSTEMTIME SystemTime;
	bool isStat=FALSE;//是否为急诊样本!
	bool bLogFile=FALSE;
	int	 nItemCount,nResultLen,AnyTimes=0;
	int SampleID,i;
	char LSampleID[5];
	char SampleType,SampleIDLen;
	LPSTR lpPointer;
	bool isdataPacket;
	//接收数据的Log文件
	Init()
	nItemCount =0;
	ResetOpPointer()
	if(NewLogFile("Vitros250.txt"))
		bLogFile=TRUE;
	EscapeCommFunction(hComm,SETRTS);
 	FillDevName()					
 	SetCommWaitMask(EV_RXCHAR ) 
	while (TRUE)
	{

	 	WaitEvent(EV_RXCHAR)  
		i=0;
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar == 0x21)
				isdataPacket = TRUE;
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;
			}
			if((RxChar==LF)& (isdataPacket == TRUE))
			{
				EscapeCommFunction(hComm,CLRRTS);
				DataBuffer[i]=RxChar;
				i++;
				break;
			}	
				
		}while(dwReadLength==1);
		isdataPacket=FALSE;
		dwRxBufferLength = i; 
		lpOpPointer=&DataBuffer[0];
	
		//计算校验和
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		if(lpOpPointer[0] == 0x21)
		{
			switch(*(lpOpPointer+4))//数据包类型
			{
			case 'a'://头纪录 header record
				//给工作单号赋值
				SampleID=StrToInt(lpOpPointer+c_SampleIDOff,c_SampleIDOff);
				_itoa(SampleID,LSampleID,10);
				if((*(lpOpPointer+64))=='T')//急诊
				{
					TempWorkingID[0]=lpDevice->DevItemType;
					TempWorkingID[1]='S';
				//	strncpy(&TempWorkingID[2],LSampleID,5);
				
				}
				else//常规
				{
					TempWorkingID[0]=lpDevice->DevItemType;
					TempWorkingID[1]=0x30;
				//	strncpy(&TempWorkingID[1],LSampleID,5);
				}
				//向工作单中填充‘0’，使工作单长度保持在6个字符
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[2],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
			    TempWorkingID[6]='\0';
				//
				SampleType=*(lpOpPointer+c_SampleTypeOff);
				//应答数据头记录
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'c'://病人描述纪录patient description record
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'd'://医生描述纪录doctor description record
		//		ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'e'://其他纪录 miscellaneous result
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'f'://测试结果纪录 test result
				/*仪器报告项目处理
				从数组中定位出当前报告项目的顺序值,顺序值为静态数组中的顺序号，
				反映为结果中的打印序号（Print_order）*/
				lpPointer=lpOpPointer+c_ResultOff;
				if(!((*lpPointer=='?')&(*lpPointer=='*')))
				{
				
					strncpy(OutResult[nItemCount].ItemID,lpOpPointer+c_ItemIDOff,c_ItemIDLen);
					switch(SampleType)
					{
					case  '1'://样本为血清
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='\0';
							}
							else
							 OutResult[nItemCount].ItemID[3]='\0';
						}
						OutResult[nItemCount].ItemID[4]='\0';
						break;
					case  '2'://样本为脑脊液
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='C';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='C';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='C';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
		
					case  '3'://样本为尿液

						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='U';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='U';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='U';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
					}
					OutResult[nItemCount].ItemNo=nItemCount+1;

					OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//将当前结果记录的仪器结果数指针指向仪器结果数变量.
					
					DeleSpace(c_ResultLen)
					FillResult(nItemCount, lpPointer ,nResultLen ) 
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					GetSystemTime(&SystemTime);  
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 					nItemCount++;   
				}
			//	ResetRxPointer()
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'g'://导出测试结果纪录Derived test result
				lpPointer=lpOpPointer+c_ResultOff;
				if((*lpPointer!='?')&(*lpPointer!='*'))
				{
				
					strncpy(OutResult[nItemCount].ItemID,lpOpPointer+c_ItemIDOff,c_ItemIDLen);
					switch(SampleType)
					{
					case  '1'://样本为血清
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='\0';
							}
							else
							 OutResult[nItemCount].ItemID[3]='\0';
						}
						OutResult[nItemCount].ItemID[4]='\0';
						break;
					case  '2'://样本为脑脊液
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='C';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='C';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='C';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
		
					case  '3'://样本为尿液

						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='U';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='U';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='U';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
					}
					OutResult[nItemCount].ItemNo=nItemCount+1;
					OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//将当前结果记录的仪器结果数指针指向仪器结果数变量.
					DeleSpace(c_ResultLen+1)
					FillResult(nItemCount, lpPointer ,nResultLen ) 
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					GetSystemTime(&SystemTime);
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 					nItemCount++;   
				}
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'h'://结束纪录Trailer
				if (nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
		//		ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				nItemCount=0;
				break;
			}
		}
		else
		{
			MessageBox(NULL,"DataError","HAha",MB_OK);
		//	ResetRxPointer() 
			EscapeCommFunction(hComm,SETRTS);
		}
	

	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;
}
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;//Line feed
	// 设置通信事件字符,当串口接收到该字符并移入缓冲区后,产生一个事件通知串口监控线程
	//此字符根据仪器不同而改变. 此注释  陈红军于2000.10.25 
}

