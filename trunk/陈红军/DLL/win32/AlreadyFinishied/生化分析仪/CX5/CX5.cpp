// CX5.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength	800		
#define	c_MaxItemNum		200		

#define c_FuncNumOff		8			
#define	c_StreamIDOff		4		
#define c_FuncNumLen		2	
#define c_SampleType        53		
#define c_03SampleIdOff     47//检验结果样本号偏移0x38
#define c_03SampleLen       9
#define c_03TrayOff			41//0x2d		
#define c_03CupOff			44//0x30		
#define c_TrayLen			2			 
#define c_CupLen			2		
#define c_11TrayOff			32//0x23c_11TrayOff		
#define c_11CupOff			35//0x26		
#define c_13TrayOff         32
#define c_13CupOff          35 
#define c_03ResultOff		81  
#define c_03ItemIDOff		59//0x3a	
#define c_13ItemIDOff	    59
#define c_ItemIDLen			4		
#define c_ResultOff			81//76		
#define c_ResultLen			9		

#define c_ResultFlagOff		91		
#define c_DeviceFlagOff		94		
#define c_ResultFlagLen		2		

#define c_SPE11ItemIDOff		73//44		
#define c_SPE11ItemIDLen		2		
#define c_SPE11ResultOff		50		

#define c_SPE13ItemIDOff		51		
#define c_SPE13ItemIDLen		3		
#define c_SPE13ResultOff		76		

#define	c_CupsOfOneTray			7	//每架上的杯数
#define	c_MultiConst			100			


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

void FillSampleType(int SampleType,int nItemCount,LPSTR ItemID);

int DecideSamplType(LPSTR  lpSampleType);


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

	SYSTEMTIME SystemTime;
	int	 i=0,j,nItemCount,nResultLen,nPackLen=0,nDataOff=0;
	LPSTR lpPointer;
	char Sample_ID[5];//样本号
	char    DataBuffer[2000];
	bool isdataPacket;
	int SampleType,SampleIDLen;
	Init()

	ResetOpPointer()				

	i=0;
 	nItemCount =0;
	FillDevName()					
//	SetCommWaitMask(EV_RXFLAG)
	SetCommWaitMask(EV_RXCHAR)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	wSampleID=0;
	TranCommChar(ACK)
	while (TRUE)
	{

	//	WaitEvent(EV_RXFLAG)//等待接受数据包

	//	TranCommChar(XOFF)//中断仪器传送
	//	GetInQueueLength(dwReadLength)//分析数据
	//	ReadCommBlock(dwReadLength)
 	
	 	WaitEvent(EV_RXCHAR)  
		i=0;
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar == '[')
				isdataPacket = TRUE;
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;
			}
			if((RxChar==LF)& (isdataPacket == TRUE))
			{
				TranCommChar(XOFF)//中断仪器传送
				DataBuffer[i]=RxChar;
				i++;
				break;
			}			
		}while(dwReadLength==1);
		isdataPacket=FALSE;
		dwRxBufferLength = i; 
		lpOpPointer=&DataBuffer[0];
		
		j=atoi(lpOpPointer+c_FuncNumOff);
		switch(j)
		{
			case 1://头记录
				if (strncmp(lpOpPointer+c_StreamIDOff ,"702",3)==0)
				{
					nItemCount =0;
					ResetRxPointer() 	
					SampleType=DecideSamplType(lpOpPointer+c_SampleType);
					TranCommChar(XON)
					continue;
				}
				else	
				{
					nItemCount =0;
					ResetRxPointer() 	
					TranCommChar(XON)			
					continue;
				}
			case 3://结果记录
				if(strncmp(lpOpPointer+c_StreamIDOff ,"702",3)==0)//结果包
				{
					GetLocalTime(&SystemTime);	
					wSampleID =(int)StrToInt(lpOpPointer+ c_03SampleIdOff, c_03SampleLen );
					//处理工作单号
					OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
					IntToStr(Sample_ID,wSampleID);
					OutResult[nItemCount].ItemNo=nItemCount+1;
				//	strncpy(&OutResult[nItemCount].WorkingID[1],&Sample_ID[0],5);
	//向工作单中填充‘0’，使工作单长度保持在6个字符
					SampleIDLen=strlen(Sample_ID);
					memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
					strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
					OutResult[nItemCount].WorkingID[6]='\0';
					FillSampleID(nItemCount, wSampleID)	
					if((*(lpOpPointer+c_03ItemIDOff+2)==0x20))
					{
						FillItemName(nItemCount,lpOpPointer+c_03ItemIDOff ,c_ItemIDLen-2)
						FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-2]);
					}
					else
						if((*(lpOpPointer+c_03ItemIDOff+3)==0x20))
						{
							FillItemName(nItemCount,lpOpPointer+c_03ItemIDOff ,c_ItemIDLen-1)
							FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-1]);
						}
						else
						{
							FillItemName(nItemCount,lpOpPointer+c_03ItemIDOff ,c_ItemIDLen)	
							FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen]);
						}

					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nResultLen=c_ResultLen;
					lpPointer=lpOpPointer+c_03ResultOff;
					DeleSpace(c_ResultLen)
					//填3包结果
					FillResult(nItemCount, lpPointer ,nResultLen )
					nItemCount++;
					ResetRxPointer() 	
					TranCommChar(XON)
					break;
				}
				else
				{
					nItemCount=0;
					ResetRxPointer() 	
					TranCommChar(XON)
					break;
				
				}
			case 5:
				if(strncmp(lpOpPointer+c_StreamIDOff ,"702",3)==0)//结果包结束
				{
					if(nItemCount>0)
					{
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}
					nItemCount=0;	
					ResetRxPointer()		
					TranCommChar(XON)				
				}
				else
				{
					nItemCount =0;
					ResetRxPointer() 	
					TranCommChar(XON)			

				}
				break;

			case 11://结果包
				GetLocalTime(&SystemTime);		
				if (strncmp(lpOpPointer+c_StreamIDOff ,"702",3)==0 )
				{
	//填样本号
					wSampleID =((int)StrToInt(lpOpPointer+ c_11TrayOff, c_TrayLen)-1)
						*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_11CupOff ,c_CupLen );
					OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
					IntToStr(Sample_ID,wSampleID);
	//向工作单中填充‘0’，使工作单长度保持在6个字符
					SampleIDLen=strlen(Sample_ID);
					memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
					strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
					OutResult[nItemCount].WorkingID[6]='\0';
				//	strncpy(&OutResult[nItemCount].WorkingID[1],&Sample_ID[0],5);
					OutResult[nItemCount].ItemNo=nItemCount+1;
					FillSampleID(nItemCount, wSampleID)	
	//填项目名称
					FillItemName(nItemCount,lpOpPointer+c_SPE11ItemIDOff ,c_SPE11ItemIDLen)	
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_SPE11ItemIDLen]);
	//填结果
					FillResult(nItemCount,lpOpPointer+c_SPE11ResultOff,c_ResultLen)
	//填日期
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nItemCount++;
					ResetRxPointer() 	
					TranCommChar(XON)	
					break;
				}
				else	
				{
					nItemCount =0;
					ResetRxPointer() 	
					TranCommChar(XON)			
					continue;
				}
			case 13:
				GetLocalTime(&SystemTime);	
				//工作单号
				wSampleID =((int)StrToInt(lpOpPointer+ c_13TrayOff, c_TrayLen )-1)
					*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_13CupOff ,c_CupLen );
				OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
				IntToStr(Sample_ID,wSampleID);
				SampleIDLen=strlen(Sample_ID);
				memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
				strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
				OutResult[nItemCount].WorkingID[6]='\0';
				FillSampleID(nItemCount, wSampleID)	
				OutResult[nItemCount].ItemNo=nItemCount+1;
				//项目名称
				if((*(lpOpPointer+c_13ItemIDOff+2)==0x20))
					{
						FillItemName(nItemCount,lpOpPointer+c_13ItemIDOff ,c_ItemIDLen-2)	
						FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-2]);
					}
					else
						if((*(lpOpPointer+c_13ItemIDOff+3)==0x20))
						{
							FillItemName(nItemCount,lpOpPointer+c_13ItemIDOff ,c_ItemIDLen-1)
							FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-1]);
						}
						else
						{
							FillItemName(nItemCount,lpOpPointer+c_13ItemIDOff ,c_ItemIDLen)
							FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen]);
						}
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nResultLen=c_ResultLen;
				lpPointer=lpOpPointer+c_SPE13ResultOff;
				DeleSpace(c_ResultLen)			
				FillResult(nItemCount,lpPointer,nResultLen)
				nItemCount++;
				break;
			default:
				nItemCount =0;
				ResetRxPointer() 	
				TranCommChar(XON)			
				continue;
		}

	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

void FillSampleType(int SampleType,int nItemCount,LPSTR ItemID)
{
	switch(SampleType)
	{
	case 1:
		strncpy(ItemID,"_SE",3);
		(*(ItemID+3))='\0';
		break;
	case 2:
		strncpy(ItemID,"_SF",3);
		(*(ItemID+3))='\0';
		break;
	case 3:
		strncpy(ItemID,"_UR",3);
		(*(ItemID+3))='\0';
		break;
	case 4:
		strncpy(ItemID,"_TU",3);
		(*(ItemID+3))='\0';
		break;
	case 5:
		strncpy(ItemID,"_PL",3);
		(*(ItemID+3))='\0';
		break;
	}
}

int DecideSamplType(LPSTR  lpSampleType)
{

		switch(*(lpSampleType))
		{
		case 'S':
			if(*(lpSampleType+1)=='E')
			{
				return 1;
			}
			else
			{
				return 2;
			}		
		case  'U':
		
				return 3;
		
		case 'T':
			
				return 4;
			
		case 'P':
				return 5;				
		
		}
		return -1;
}