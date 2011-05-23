// RL7060.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "str.h"
#include "logfile.h"
#include "device.h"
#include "devmacro.h"
#include "const.h"

#define c_RxBufferLength	300		
#define	c_MaxItemNum		40		

#define c_PacketFuncOff		1		
#define c_TransModeAndSampleTypeOff 	2		
 
 
#define c_SampleNoOff		4		
#define c_SampleNoLen		3		
#define c_SampleIDOff		10		
#define c_SampleIDLen		13		
 
#define c_ItemCountOff		38		
#define c_ItemCountLen		2		
#define c_DataOff			40		

#define c_OneDataLen		9		
#define c_ItemIDOff			0		
#define c_ItemIDLen			2		
#define c_ResultOff			2		
#define c_ResultLen			6		
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1		
#define c_RxCheckSumLen		1	

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
	char SampleID[5];
 	SYSTEMTIME SystemTime;
	int	 nItemCount,nDataOff,nResultLen,SampleIDLen;
	bool bLogFile;
	BYTE  btResultType, btRxCheckSum;
	DWORD wSampleID1;
	LPSTR lpPointer,lptemp1;
	const char *p_MOR  ="\x2>\x3=";		
	const char *p_REP  ="\x2?\x3<";		
 
 	bLogFile=NewLogFile("RL7060.txt");

	Init()
		
	nItemCount =0;
	ResetOpPointer()				
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG )
 
	while (TRUE)
	{

		ResetRxPointer()
		ResetOpPointer()
 		GetInQueueLength(dwReadLength)
 		WaitEvent(EV_RXFLAG)  
		GetInQueueLength(dwReadLength)
  		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	continue; 
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
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}
		if ((*(lpOpPointer+c_PacketFuncOff)!='1') &&
			(*(lpOpPointer+c_PacketFuncOff)!='2') &&
			(*(lpOpPointer+c_PacketFuncOff)!=':'))
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			WriteCommBlock(p_MOR ,strlen(p_MOR))	
			continue;
		}

		nItemCount = 0;
 		GetLocalTime(&SystemTime);

		wSampleID =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
		wSampleID1 =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);

		TempWorkingID[0]=lpDevice->DevItemType;
		_itoa(wSampleID,SampleID,10);
		SampleIDLen=strlen(SampleID);
		memset(&TempWorkingID[1],0x30,5);
		strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
		TempWorkingID[6]='\0';
	//	WriteLogData(hLogFile,TempWorkingID,7);
		btResultType=*(lpOpPointer+ c_TransModeAndSampleTypeOff);
		if (btResultType=='A' || btResultType=='a' || btResultType=='B' ||
			btResultType=='b' || btResultType=='S' || btResultType=='s' ||
			btResultType=='T' || btResultType=='t' )
				btResultType =c_TypeSample;
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
		{
			btResultType =c_TypeStat ;
			TempWorkingID[1]='S';
		}
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
				btResultType =c_TypeQC ;
			
		if (btResultType ==c_TypeSample || btResultType ==c_TypeStat ||
				btResultType ==	 c_TypeQC)	
		{	
			for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX ;)//for===A
			{

				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				if(*(lpOpPointer+nDataOff+c_ItemIDOff)==0x20)
				{
					FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff+1, c_ItemIDLen-1)//添项目
				}
				else
				{
					FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)//添项目
				}
				lpPointer=lpOpPointer+nDataOff+c_ResultOff;
				DeleSpace(c_ResultLen )

				FillResult(nItemCount, lpPointer ,nResultLen ) //添结果

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

				FillResultType(nItemCount, btResultType)

 				FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)
				OutResult[nItemCount].ItemNo=nItemCount+1;
					 
 				FillErrorCode(nItemCount, c_Reliable)	
 				nItemCount++;   
					
				nDataOff+=c_OneDataLen;  

			}	
		}
 



		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;

 
	 	WriteCommBlock(p_MOR ,strlen(p_MOR))	
		PurgeComm( hComm, PURGE_RXCLEAR );	
 
 		
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
}

