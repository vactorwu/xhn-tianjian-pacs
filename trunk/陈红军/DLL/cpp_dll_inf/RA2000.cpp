//********************************************************************
 
//  模块:   RA2000.cpp	
//  描述: 本模块为美国TECHNICON仪器公司的RA2000生化仪的联机程序(Win95版)

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
 

#define c_RxBufferLength	800		
#define	c_MaxItemNum		30		
#define c_PacketFuncOff		2		
#define c_MTOff				1		

#define c_SampleIDOff		4		
#define c_SampleIDLen		7		
 
#define	c_TrayNoOff			13		
#define	c_TrayNoLen			4		
#define c_CupNoOff			18		
#define c_CupNoLen			2		

#define c_DataOff			40		
#define c_OneDataLen		14		
#define c_ItemIDOff			0		
#define c_ItemIDLen			3		
#define c_ResultOff			4		
#define c_ResultLen			5		
#define c_ErrorCodeOff		9		
#define c_ErrorCodeLen		5		



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

 	SYSTEMTIME SystemTime;
	int	 nItemCount,nDataOff;
	int		kk=0,NAK_Flag=0;
	LPSTR lptemp1;


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

		switch (*(lpOpPointer+c_PacketFuncOff)) 
		{	 
			case 'R':
			GetLocalTime(&SystemTime);


		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);

			nDataOff =c_DataOff;
			nItemCount=0;
			for (;*(lpOpPointer+nDataOff)!=CR;)
			{

				FillSampleID(nItemCount, wSampleID)		

	 			FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)

				FillResult(nItemCount, lpOpPointer+nDataOff+c_ResultOff , c_ResultLen) 
 
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

//				FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
						 
//				FillErrorCode(nItemCount, c_Reliable)	
					
				nDataOff += c_OneDataLen; 
				nItemCount ++;
			}	

			if (nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
 

			PurgeComm( hComm, PURGE_RXCLEAR );	

				break;	
			case 'S':
				break;	
			default :	
				PurgeComm( hComm, PURGE_RXCLEAR );	

		}//END SWITCH



	}//END WHILE-----A

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =ETX;
}



