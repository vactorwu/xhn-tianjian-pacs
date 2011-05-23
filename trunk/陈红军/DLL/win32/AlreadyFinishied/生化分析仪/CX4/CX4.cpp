// CX4.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength	800		
#define	c_MaxItemNum		100		

#define c_FuncNumOff		8					
#define	c_StreamIDOff		4		
#define c_FuncNumLen		2		

#define c_SampleTypeOff     69
#define c_03TrayOff			45//0x2d		
#define c_03CupOff			48//0x30		
#define c_TrayLen			2			
#define c_CupLen			2			
#define c_11TrayOff			0x23		
#define c_11CupOff			0x26
		
#define c_13TrayOff			0x2d		
#define c_13CupOff			0x30		

#define c_ItemIDOff			51		
#define c_ItemIDLen			4		
#define c_ResultOff			76		
#define c_ResultLen			9		
//03功能的结果
#define c_ResultFlagOff		91		
#define c_DeviceFlagOff		94		
#define c_ResultFlagLen		2		

#define c_SPE11ItemIDOff		44		
#define c_SPE11ItemIDLen		2		
#define c_SPE11ResultOff		50		

#define c_SPE13ItemIDOff		51	
#define c_SPE13ItemIDLen		3		
#define c_SPE13ResultOff		76		

#define	c_CupsOfOneTray			10		
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
	char Sample_ID[6];
	int SampleType,SampleIDLen;
	Init()
	ResetOpPointer()				

	
	ResetRxPointer()

	i=0;
 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	wSampleID=0;
	TranCommChar(XON)

	while (TRUE)
	{
		WaitEvent(EV_RXFLAG)//等待接受数据包

		TranCommChar(XOFF)//中断仪器传送

		GetInQueueLength(dwReadLength)//分析数据
		ReadCommBlock(dwReadLength)
 			
	
		j=atoi(lpOpPointer+c_FuncNumOff);
		switch(j)
		{
			case 1:
				if (strncmp(lpOpPointer+c_StreamIDOff ,"402",3)==0)
				{
					nItemCount =0;
					ResetRxPointer() 	
					SampleType=DecideSamplType(lpOpPointer+c_SampleTypeOff);
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
			case 5:
				if (strncmp(lpOpPointer+c_StreamIDOff ,"402",3)==0)
				{
					if(nItemCount>0)
					{
						//向系统队列添加结果数据
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}
					nItemCount=0;	
					ResetRxPointer()		
					TranCommChar(XON)
				}
				else
				{
					nItemCount=0;	
					ResetRxPointer()		
					TranCommChar(XON)
				}
				break;
			case 3:
				GetLocalTime(&SystemTime);	
				
				//工作单号由盘号*每盘数量+在盘中的位置
				wSampleID =((int)StrToInt(lpOpPointer+ c_03TrayOff, c_TrayLen )-1)
					*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_03CupOff ,c_CupLen );
				IntToStr(Sample_ID,wSampleID);
				
				OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
				//strncpy(&OutResult[nItemCount].WorkingID[1],&Sample_ID[0],5);
				//向工作单号中添加'0'
				SampleIDLen=strlen(Sample_ID);
				memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
				strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
				OutResult[nItemCount].WorkingID[6]='\0';
				//打印序号
				OutResult[nItemCount].ItemNo=nItemCount+1;
				FillSampleID(nItemCount, wSampleID)	
				//填项目名称
				if((*(lpOpPointer+c_ItemIDOff+2)==0x20))
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen-2)	
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-2]);
				}
				else
				if((*(lpOpPointer+c_ItemIDOff+3)==0x20))
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen-1)
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-1]);
				}
				else
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen]);
				}
	//填状态注解
	//填日期
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nResultLen=c_ResultLen;
				DeleZero(lpOpPointer, nResultLen)			
				FillResult(nItemCount,lpPointer,nResultLen)
				if(OutResult[nItemCount].AssayResult[nResultLen]=='.')
				{
					OutResult[nItemCount].AssayResult[nResultLen]='\0';
				}
				nItemCount++;
				ResetRxPointer()		
				TranCommChar(XON)
				break;
		case 11:	
				if (strncmp(lpOpPointer+c_StreamIDOff ,"402",3)==0 )
				{
					GetLocalTime(&SystemTime);	
					//填样本号
					wSampleID =((int)StrToInt(lpOpPointer+ c_11TrayOff, c_TrayLen)-1)
						*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_11CupOff ,c_CupLen );
					IntToStr(Sample_ID,wSampleID);
					
					OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
					//strncpy(&OutResult[nItemCount].WorkingID[1],&Sample_ID[0],5);
					//向工作单号中添加'0'
					SampleIDLen=strlen(Sample_ID);
					memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
					strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
					OutResult[nItemCount].WorkingID[6]='\0';
					FillSampleID(nItemCount, wSampleID)	
					//填项目名称
					FillItemName(nItemCount,lpOpPointer+c_SPE11ItemIDOff ,c_SPE11ItemIDLen)	
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_SPE11ItemIDLen]);
					//填结果
					FillResult(nItemCount,lpOpPointer+c_SPE11ResultOff,c_ResultLen)
					//填日期
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nItemCount++;
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
				wSampleID =((int)StrToInt(lpOpPointer+ c_13TrayOff, c_TrayLen )-1)
					*c_CupsOfOneTray+(int)StrToInt(lpOpPointer+c_13CupOff ,c_CupLen );
				IntToStr(Sample_ID,wSampleID);
				IntToStr(Sample_ID,wSampleID);
				
				OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
				//strncpy(&OutResult[nItemCount].WorkingID[1],&Sample_ID[0],5);
				//向工作单号中添加'0'
				SampleIDLen=strlen(Sample_ID);
				memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
				strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&Sample_ID[0],SampleIDLen);
				OutResult[nItemCount].WorkingID[6]='\0';
			//	OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
			//	OutResult[nItemCount].ItemNo=nItemCount+1;
				FillSampleID(nItemCount, wSampleID)	
	//填项目名称
				if((*(lpOpPointer+c_ItemIDOff+2)==0x20))
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen-2)
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-2]);		
				}
				else
				if((*(lpOpPointer+c_ItemIDOff+3)==0x20))
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen-1)
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen-1]);
				}
				else
				{
					FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)
					FillSampleType(SampleType,nItemCount,&OutResult[nItemCount].ItemID[c_ItemIDLen]);
				}
	//填状态注解
	//填日期
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nResultLen=c_ResultLen;
				DeleZero(lpOpPointer, nResultLen)			
				FillResult(nItemCount,lpPointer,nResultLen)
				nItemCount++;
				break;

			case 81:
			default:
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

