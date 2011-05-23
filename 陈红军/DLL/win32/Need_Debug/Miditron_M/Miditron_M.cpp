// Miditron_M.cpp : Defines the entry point for the DLL application.
//
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "stdafx.h"
#include "device.h"
#include "const.h"
#include "devmacro.h"
#include "str.h"
#include "Logfile.h"


#define c_RxBufferLength	255		 
#define	c_MaxItemNum		11		 
#define c_PacketFuncOff		1		 
#define c_ResultMaxLen		0x11	 

#define c_SampleIDOff		0x10	 
#define c_SampleIDLen		4		 

#define c_SG_ItemOff		0x24	 
#define c_SG_ItemLen		2		 
#define c_SG_ResultOff		0x26	 

#define c_PH_ItemOff		0x31	 
#define c_PH_ItemLen		2		 
#define c_PH_ResultOff		0x33	 

#define c_LEU_ItemOff		0x3c	 
#define c_LEU_ItemLen		3		 
#define c_LEU_ResultOff		0x3f	 

#define c_NIT_ItemOff		0x50	 
#define c_NIT_ItemLen		3		 
#define c_NIT_ResultOff		0x53	 
#define c_PRO_ItemOff		0x5c	 
#define c_PRO_ItemLen		3		 
#define c_PRO_ResultOff		0x5f	 
#define c_GLU_ItemOff		0x70	 
#define c_GLU_ItemLen		3		 
#define c_GLU_ResultOff		0x73	 

#define c_KET_ItemOff		0x84	 
#define c_KET_ItemLen		3		 
#define c_KET_ResultOff		0x87	 

#define c_UBG_ItemOff		0x98	 
#define c_UBG_ItemLen		3		 
#define c_UBG_ResultOff		0x9b	 

#define c_BIL_ItemOff		0xac	 
#define c_BIL_ItemLen		3		 
#define c_BIL_ResultOff		0xaf	 

#define c_ERY_ItemOff		0xc0	 
#define c_ERY_ItemLen		3		 
#define c_ERY_ResultOff		0xc3	 

 
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
	CommData
 	SYSTEMTIME SystemTime;

	int	 nItemCount,nResultLen,nResultLen1;

	LPSTR lpPointer,lptemp1;
	char *p_MOR  ="\x2>\x3\x20\x20\r\n";		 

 
	Init()
 
	nItemCount =0;
	ResetOpPointer()				 
 	FillDevName()					 
 	SetCommWaitMask(EV_RXFLAG)

	while (TRUE)
	{
 
		ResetRxPointer()		 
 		GetInQueueLength(dwReadLength)
 		WaitEvent(EV_RXFLAG)  
  		GetInQueueLength(dwReadLength)
  		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	continue; 
 
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
		}

		switch (*(lpOpPointer+c_PacketFuncOff)) 
		{	 
			case	'<':
				PurgeComm( hComm, PURGE_RXCLEAR );
				WriteCommBlock(p_MOR ,strlen(p_MOR))	
				continue;
			case	';':
				break;	
		
			default :	
				PurgeComm( hComm, PURGE_RXCLEAR );
				WriteCommBlock(p_MOR,strlen(p_MOR))
				continue;
		}

		nItemCount =0;
		GetLocalTime(&SystemTime);
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);

//1--SG		
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_SG_ItemOff, c_SG_ItemLen)
		lpPointer=lpOpPointer+c_SG_ResultOff;

		DeleSpace(c_ResultMaxLen)

		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	

		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//2--PH
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PH_ItemOff, c_PH_ItemLen)
		lpPointer=lpOpPointer+c_PH_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//3--LEU
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_LEU_ItemOff, c_LEU_ItemLen)
		lpPointer=lpOpPointer+c_LEU_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//4--NIT
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_NIT_ItemOff, c_NIT_ItemLen)
		lpPointer=lpOpPointer+c_NIT_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//5--PRO
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PRO_ItemOff, c_PRO_ItemLen)
		lpPointer=lpOpPointer+c_PRO_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//6--GLU
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_GLU_ItemOff, c_GLU_ItemLen)
		lpPointer=lpOpPointer+c_GLU_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//7--KET
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_KET_ItemOff, c_KET_ItemLen)
		lpPointer=lpOpPointer+c_KET_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//8--UBG
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_UBG_ItemOff, c_UBG_ItemLen)
		lpPointer=lpOpPointer+c_UBG_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//9--BIL
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_BIL_ItemOff, c_BIL_ItemLen)
		lpPointer=lpOpPointer+c_BIL_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//10--ERY
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_ERY_ItemOff, c_ERY_ItemLen)
		lpPointer=lpOpPointer+c_ERY_ResultOff;
		DeleSpace(c_ResultMaxLen)
		lptemp1=lpPointer;
		for (nResultLen1=0 ;*lptemp1!=' ';lptemp1++,nResultLen1++) ;
		FillResult(nItemCount,(LPSTR)(lptemp1-nResultLen1) , nResultLen1)	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;


		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
 


	 	 PurgeComm( hComm, PURGE_RXCLEAR );	
		 WriteCommBlock(p_MOR ,strlen(p_MOR))	

 		
}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =CR;
}

