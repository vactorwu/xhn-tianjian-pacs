//********************************************************************
//  模块: MA4210.cpp
//  描述: 本模块为尿8联化验仪的联机程序(Win95版)	 
// 
 
#include <stdio.h>     
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	180		 
#define	c_MaxItemNum		8		 

#define c_SampleIDOff		10		 
#define c_SampleIDLen		2
#define c_ItemNameLen		3		 

#define c_NIT_ItemOff		18		 
#define c_NIT_ResultOff		22		 
#define c_NIT_ResultLen		2		 

#define c_PH_ItemOff		35		 
#define c_PH_ResultOff		43		  
#define c_PH_ResultLen		3		 

#define c_GLU_ItemOff		18		 
#define c_GLU_ResultOff		22		  
#define c_GLU_ResultLen		7		 

#define c_PRO_ItemOff		18		 
#define c_PRO_ResultOff		22		  
#define c_PRO_ResultLen		7		 

#define c_BLD_ItemOff		18		 
#define c_BLD_ResultOff		22		  
#define c_BLD_ResultLen		7		 

#define c_KET_ItemOff		18		 
#define c_KET_ResultOff		22		 
#define c_KET_ResultLen		7		 

#define c_BIL_ItemOff		18		 
#define c_BIL_ResultOff		22		 
#define c_BIL_ResultLen		7		 

#define c_URO_ItemOff		18		 
#define c_URO_ResultOff		22		  
#define c_URO_ResultLen		7		 


 

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
 	SYSTEMTIME SystemTime;
	int	 i,nItemCount,nResultLen;
	LPSTR	lptemp1;
 
	Init()
 
	nItemCount =0;
	ResetOpPointer()				 
 	FillDevName()					 
 	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	 
	while (TRUE)
	{
 
		ResetRxPointer()		 
  		WaitEvent(EV_RXFLAG)  
  		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
		}

 
 
		nItemCount =0;
		GetLocalTime(&SystemTime); 
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen); 
 
//1--NIT
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_NIT_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_NIT_ResultOff,c_NIT_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//2--PH
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PH_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_PH_ResultOff,c_PH_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//3--GLU
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_GLU_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_GLU_ResultOff,c_GLU_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//4--PRO
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PRO_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_PRO_ResultOff,c_PRO_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//5--BLD
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_BLD_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_BLD_ResultOff,c_BLD_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//6--KET
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_KET_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_KET_ResultOff,c_KET_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//7--BIL
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_BIL_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_BIL_ResultOff,c_BIL_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//8--URO
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_URO_ItemOff, c_ItemNameLen)
		FillResult(nItemCount, lpOpPointer+c_URO_ResultOff,c_URO_ResultLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
 
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
 
 

	 	 PurgeComm( hComm, PURGE_RXCLEAR );	 
 		
}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}



