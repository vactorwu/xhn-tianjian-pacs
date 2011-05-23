//********************************************************************
//  模块:  MIDI_JUN.cpp(ver format)
//  描述: 本模块为德国宝林曼公司的miditron_JUNIOR型 尿化验仪的联机程序(Win95版)

 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
 

#define c_RxBufferLength	512		 
#define	c_MaxItemNum		11		 

#define c_SampleIDOff		0x18	 
#define c_SampleIDLen		3		 
#define c_ResultMinLen		0x0c
#define c_ResultMaxLen		0x12
#define c_TestIDLen			3

#define c_PackIDOff			0x38	 

#define c_SG_ItemOff		0x39	 
#define c_SG_ResultOff		0x3d	 

#define c_PH_ItemOff		0x4d	 
#define c_PH_ResultOff		0x51	 

#define c_LEU_ItemOff		0x61	 
#define c_LEU_ResultOff		0x65	  

#define c_NIT_ItemOff		0x7b	 
#define c_NIT_ResultOff		0x7f	 

#define c_PRO_ItemOff		0x95	 
#define c_PRO_ResultOff		0x99	 

#define c_GLU_ItemOff		0xaf	 
#define c_GLU_ResultOff		0xb3	 

#define c_KET_ItemOff		0xc9	 
#define c_KET_ResultOff		0xcd	 

#define c_UBG_ItemOff		0xe3	 
#define c_UBG_ResultOff		0xe7	 

#define c_BIL_ItemOff		0xfd	 
#define c_BIL_ResultOff		0x101	 

#define c_ERY_ItemOff		0x117	  
#define c_ERY_ResultOff		0x11b	  



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
 	SYSTEMTIME SystemTime;
	int	 i,nItemCount,nResultLen,nResultLen1;

	LPSTR lpPointer,lptemp1;


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
		else	
			continue; 
 
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
		}



 
		if ( *(lpOpPointer+c_PackIDOff)=='I')
		{
			continue;
		}
 	
 		
		nItemCount =0;
		GetLocalTime(&SystemTime); 
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
 
//1--SG		
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_SG_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_SG_ResultOff ,c_ResultMinLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//2--PH
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PH_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_PH_ResultOff,c_ResultMinLen )	
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//3--LEU
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_LEU_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_LEU_ResultOff,c_ResultMaxLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//4--NIT
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_NIT_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_NIT_ResultOff,c_ResultMaxLen)
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//5--PRO
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_PRO_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_PRO_ResultOff,c_ResultMaxLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//6--GLU
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_GLU_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_GLU_ResultOff,c_ResultMaxLen )
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//7--KET
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_KET_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_KET_ResultOff,c_ResultMaxLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//8--UBG
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_UBG_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_UBG_ResultOff,c_ResultMaxLen )
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//9--BIL
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_BIL_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_BIL_ResultOff,c_ResultMaxLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;
//10--ERY
		FillSampleID(nItemCount, wSampleID)		
 		FillItemName(nItemCount, lpOpPointer+c_ERY_ItemOff, c_TestIDLen)
		FillResult(nItemCount,lpOpPointer+c_ERY_ResultOff,c_ResultMaxLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 		nItemCount++;

 
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
 
 

//	 	 PurgeComm( hComm, PURGE_RXCLEAR );	 

 		
}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =ETX;
}

