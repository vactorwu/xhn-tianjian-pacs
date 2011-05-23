//********************************************************************
//  模块: CORN_664.cpp
//  描述: 本模块为美国康宁公司的CIBA-CORNING离子分析仪的联机程序(Win95版)

#include <stdio.h>
//#include <wchar.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	4000		
#define	c_MaxItemNum		4		

//#define c_SampleIDOff			0x38
#define c_SampleIDLen			3
#define	c_LabIDOff			0x50
#define	c_LabIDLen			8

#define c_ResultRecordLen	19
#define c_FirstDataOff		0x45	
#define c_ItemIDOff			0		
#define c_ItemIDLen			4		//
#define c_ResultOff			7		//
#define c_ResultLen			6		//




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nRecordLen=0,ss=0;
	LPSTR lptemp1,ptemp1,ptemp2,ptemp3,ptemp4;
	BYTE	Achar;


	Init()
	nItemCount =0;
	ResetOpPointer()				
	ResetRxPointer()
	ClearCommRx()		
	FillDevName()					

	SetCommWaitMask(EV_RXCHAR)



	while (TRUE)
	{

//		WaitEvent(EV_RXFLAG)
		WaitEvent(EV_RXCHAR)

		GetInQueueLength(dwReadLength)
 
		ReadCommBlock(dwReadLength)

		*(lpOpPointer+dwReadLength)='\0';

		lptemp1=strchrnb(lpOpPointer,STX,1,10);
		if (lpOpPointer[0]!=STX) 
		{
			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memmove( lpOpPointer, lptemp1,dwRxBufferLength );
		} 

		if (strncmp(lpOpPointer+1,"CAL",3)==0 )
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			ResetRxPointer()		
			ResetOpPointer()
			continue;
		}
	
		GetLocalTime(&SystemTime);	
		nItemCount =0;
	
		ptemp1=strchrnb(lpOpPointer,'i',1, 0xb0);
		if (ptemp1==NULL)
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			ResetRxPointer()		
			ResetOpPointer()
			continue;
		}
		ptemp1 ++;
		if (strncmp(ptemp1,"PID",3)!=0)//找不到病人PID项
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			ResetRxPointer()		
			ResetOpPointer()
			continue;
		}
		ptemp1 += 4;		
		wSampleID =StrToInt(ptemp1,c_SampleIDLen);		

 				
		FillSampleID(nItemCount, wSampleID)	

		ptemp2=strchrnb(ptemp1,'m',1, 25)+1;
	 	FillItemName(nItemCount,ptemp2 , 2)

		ptemp3=strchrnb(ptemp2,0x1d,1, 8)+1;
		ptemp4=strchrnb(ptemp3,0x1d,1, 8);
		if (*ptemp3== 0x1d)
		{
			FillResult(nItemCount, "    " ,4)
		}
		else
		{
			FillResult(nItemCount, ptemp3 ,ptemp4-ptemp3)
		}
 
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

	
		ptemp1=strchrnb(ptemp4,0x1c,1, 25)+2;
 				
		FillSampleID(nItemCount, wSampleID)	

	 	FillItemName(nItemCount,ptemp1 , 1)

		ptemp2=strchrnb(ptemp1,0x1d,1, 8)+1;
		ptemp4=strchrnb(ptemp2,0x1d,1, 8);
		if (*ptemp2== 0x1d)
		{
			FillResult(nItemCount, "    " ,4)
		}
		else
		{
			FillResult(nItemCount, ptemp2 ,ptemp4-ptemp2) 
		}
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;
//Cl
		ptemp1=strchrnb(ptemp4,0x1c,1, 25)+2;
  				
		FillSampleID(nItemCount, wSampleID)	

	 	FillItemName(nItemCount,ptemp1 , 2)

		ptemp3=strchrnb(ptemp1,0x1d,1, 8)+1;
		ptemp4=strchrnb(ptemp3,0x1d,1, 8);
		if (*ptemp3== 0x1d)
		{
			FillResult(nItemCount, "    " ,4)		
		}
		else
		{
			FillResult(nItemCount, ptemp3 ,ptemp4-ptemp3)
		}
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;
//tCO2
		ptemp1=strchrnb(ptemp4,0x1c,1, 25)+2;
				
		FillSampleID(nItemCount, wSampleID)	

	 	FillItemName(nItemCount,ptemp1,4)

		ptemp3=strchrnb(ptemp1,0x1d,1, 8)+1;
		ptemp4=strchrnb(ptemp3,0x1d,1, 8);
		if (*ptemp3== 0x1d)
		{
			FillResult(nItemCount, "    " ,4)		
		}
		else
		{
			FillResult(nItemCount, ptemp3 ,ptemp4-ptemp3)
		}
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		PurgeComm( hComm, PURGE_RXCLEAR );
		ResetRxPointer()		
		ResetOpPointer()
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =EOT;
}

