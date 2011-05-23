// AU600.cpp : Defines the entry point for the DLL application.
//

#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "device.h"
#include "const.h"
#include "devmacro.h"
#include "str.h"
#include "Logfile.h"

#define c_RxBufferLength	512		
#define	c_MaxItemNum		35		 
#define c_PackTypeOff		1
#define c_TranTypeOff		2
 
#define c_SampleNoOff		12
#define c_SampleNoLen		4

#define c_OneDataLen		10

// c_dataoff 'Value is not exactly defined chj 2000.7.17
#define c_DataOff           21
#define c_ItemIDOff			0		
#define c_ItemIDLen			2
#define c_ResultOff			2		
#define c_ResultLen			6
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1	
extern  HANDLE hLogFile;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

  


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	char LSampleID[5];;//指向样本号(字符串)的指针
    //创建并打开文件
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nPackType, nTranType, nDataOff;
	BYTE  btCalCheckSum, btRxCheckSum;
	WORD 		wSampleNo,SampleIDLen;						
	BYTE		btResultType;
	char TempWorkingID[7];
	bool bLogFile;
	Init()
	nItemCount =0;
	ResetOpPointer()				
	FillDevName()					
	PurgeComm(hComm, PURGE_RXCLEAR );
	SetCommWaitMask(EV_RXFLAG)
	bLogFile=NewLogFile("Au600.txt");//创建LOG文件
	while (TRUE)	
	{

		ResetRxPointer()	
		do
		{
			GetInQueueLength(dwReadLength)
			if(dwReadLength==0 && dwRxBufferLength==0)
			{
				WaitEvent(EV_RXFLAG)  
				GetInQueueLength(dwReadLength)
			}
 
			ReadCommBlock(dwReadLength)	
			if ( dwRxBufferLength>=2)
			{
				if( (*(lpRxPointer-2)== ETX))
				break;
			}
				
		}while(TRUE);				
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		//校验
	
		i =2;
		btCalCheckSum =lpOpPointer[1];
		do {
		   btCalCheckSum ^=lpOpPointer[i];
		   if (lpOpPointer[i] ==ETX )	
			   break;
		   i++;
			}while(TRUE);
		if (btCalCheckSum!=0)
		{
			btRxCheckSum =lpOpPointer[i+1];
			if (btRxCheckSum!=btCalCheckSum)
			{
				PurgeComm( hComm, PURGE_RXCLEAR );		
				WriteCommChar(NAK)
				continue;
			}
			
		}

 
		nPackType = lpOpPointer[c_PackTypeOff];
		btResultType =0xff;
		nItemCount =0;
		switch (nPackType)	
		{
		case	'R':
			break;
		case	'D':
		case	'd':
			nTranType =lpOpPointer[c_TranTypeOff];
			switch (nTranType)	
			{				  
			case	'B': 
				break;
			case	'E':

				if (lpOpPointer[5]==ETX) 	break;

			default:
 
 				GetLocalTime(&SystemTime);
 				switch (*(lpOpPointer+c_SampleNoOff))	
				{
				case	'0'://常规样本0001--9999
				case	'1':
				case	'2':
				case	'3':
				case	'4':
				case	'5':
				case	'6':
				case	'7':
				case	'8':
				case	'9':
					wSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
					btResultType =c_TypeSample;
					break;
				case	'P':
					wSampleNo =StrToInt(lpOpPointer+c_SampleNoOff+1, c_SampleNoLen-1);
					btResultType =c_TypeEmergency;
					break;
				case	'E':
					wSampleNo =StrToInt(lpOpPointer+c_SampleNoOff+1, c_SampleNoLen-1);
					btResultType =c_TypeEmergency;
					break;

				}
				if(btResultType != 0xff)		
				_itoa(wSampleNo,LSampleID,10);		
				TempWorkingID[0]=lpDevice->DevItemType;
				SampleIDLen=strlen(LSampleID);
			 	memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
				TempWorkingID[6]='\0';				
				for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX  ;)
				{
					//合成工作单号，并将合成的工作单号写入结果数组OutResult
			        strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			
					OutResult[nItemCount].ItemNo=StrToInt(lpOpPointer+nDataOff+c_ItemIDOff,2);//项目号
					FillSampleID(nItemCount, wSampleNo)	
					FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)	//项目名称				    
					//将当前结果记录的仪器结果数指针指向仪器结果数变量.
                    OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);

					FillResult(nItemCount,  lpOpPointer+nDataOff+c_ResultOff,c_ResultLen )

					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

					FillResultType(nItemCount, btResultType)	

					FillErrorCode(nItemCount, c_Reliable)	
					nItemCount++;
					nDataOff+=c_OneDataLen;
				}
		
		
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;

			}
 
		
			break;
		}

		ResetRxPointer()
		PurgeComm( hComm, PURGE_RXCLEAR );
 		WriteCommChar(ACK)

  }
  if(bLogFile)
	  CloseLogFile(hLogFile);
	return TRUE;

} 

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

