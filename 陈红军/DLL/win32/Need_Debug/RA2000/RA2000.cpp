
//********************************************************************
 
//  模块:   RA2000.cpp	
//  描述: 本模块为美国TECHNICON仪器公司的RA2000生化仪的联机程序(Win95版)


#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"

#define c_RxBufferLength	800		
#define	c_MaxItemNum		50		
#define c_PacketFuncOff		2		
#define c_MTOff				1		

#define c_SampleIDOff		4		
#define c_SampleIDLen		7		
 
#define	c_TrayNoOff			13		
#define	c_TrayNoLen			4		
#define c_CupNoOff			18		
#define c_CupNoLen			2		
                               //数据说明书
#define c_DataOff			40 //56		
#define c_OneDataLen		14 //9		
#define c_ItemIDOff			0		
#define c_ItemIDLen			3		
#define c_ResultOff			4		
#define c_ResultLen			5		
#define c_ErrorCodeOff		9		
#define c_ErrorCodeLen		5		
 
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
	CommData
	char TempWorkingID[7];
	char LSampleID[6];
    bool bLogFile;
	char  charMsgToggle;
 	SYSTEMTIME SystemTime;
	int	 nItemCount,nDataOff,SampleIDLen;
	int		kk=0,NAK_Flag=0;
//	LPSTR lptemp1;

	if(NewLogFile("RA2000.txt"))
	{
		bLogFile=TRUE;
	}

	Init()
 
	nItemCount =0;
	ResetOpPointer()				
 	ResetRxPointer()
	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG )
	while (TRUE)//------A
	{

		ResetRxPointer()		

		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)

		ReadCommBlock(dwReadLength)
		//calculation checksum
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}
		charMsgToggle=*(lpOpPointer+1);
		switch (*(lpOpPointer+c_PacketFuncOff)) 
		{	 
			case 'R':
			GetLocalTime(&SystemTime);


	    	wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
			_itoa(wSampleID,LSampleID,10);		
			TempWorkingID[0]=lpDevice->DevItemType;
			SampleIDLen=strlen(LSampleID);
			memset(&TempWorkingID[1],0x30,5);
			strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
			TempWorkingID[6]='\0';		
			nDataOff =c_DataOff;
			nItemCount=0;
			for (;*(lpOpPointer+nDataOff)!=CR;)
			{

				FillSampleID(nItemCount, wSampleID)		

	 			FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)

				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);

				OutResult[nItemCount].ItemNo=nItemCount+1;

				FillResult(nItemCount, lpOpPointer+nDataOff+c_ResultOff , c_ResultLen) 
 
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		
				nDataOff += c_OneDataLen; 
				nItemCount ++;
			}	

			if (nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
 
			WriteCommChar(charMsgToggle)
			PurgeComm( hComm, PURGE_RXCLEAR );	
				break;	
			case 'S':
				
			default :	
				WriteCommChar(charMsgToggle)
				PurgeComm( hComm, PURGE_RXCLEAR );	

		}//END SWITCH



	}//END WHILE-----A
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =ETX;
}

