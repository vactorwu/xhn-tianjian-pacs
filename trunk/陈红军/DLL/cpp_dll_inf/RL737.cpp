//********************************************************************
 
//  模块:   RL737.cpp	
//  描述: 本模块为日本日立公司的MODEL_737化验仪的联机程序(Win95版)

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
 

#define c_RxBufferLength	600		
#define	c_MaxItemNum		35		
#define c_PacketFuncOff		1		

//#define c_SampleNoOff		2		
//#define c_SampleNoLen		10		
#define c_SampleIDOff		2		
#define c_SampleIDLen		10		
 
#define	c_TotalItemsOff		186		
#define	c_TotalItemsLen		2		
#define c_DataOff			188		
#define c_OneDataLen		12		
#define c_ItemIDOff			0		
#define c_ItemIDLen			2		
#define c_ResultOff			3		
#define c_ResultLen			5		
#define c_ErrorCodeOff		10		
#define c_ErrorCodeLen		1		

#define c_RxCheckSumLen		2		
#define	c_DelayTimeMaxCounts	3000	


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

 	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nDataOff,nResultLen,DelayTimeCounts=18000;
	int TotalItems;
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	DWORD wSampleID1;
	LPSTR lpPointer,lptemp1;

	Init()
 
	nItemCount =0;
	ResetOpPointer()				
 	ResetRxPointer()
	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG )
	int kk=0;

	WriteCommChar(DC1)	
	Sleep(100);
	while (TRUE)
	{

		ResetRxPointer()		
		GetInQueueLength(dwReadLength)

		if (DelayTimeCounts>=c_DelayTimeMaxCounts && dwReadLength==0 )
		{
			DelayTimeCounts=0;



			do{			
				Sleep(1);
				kk ++;
				if (kk>=3000)
				{
					kk=0;
					WriteCommChar(DC1)
				}
				GetInQueueLength(dwReadLength)
			} while(dwReadLength==0);
		}



		ReadCommBlock(dwReadLength)
		lptemp1=strchrnb(lpOpPointer,STX,1,dwReadLength);
		if (lptemp1==NULL)
		{
			WriteCommChar(DC1)	
			DelayTimeCounts=18000;	
			continue;
		}

		WaitEvent(EV_RXFLAG)


		GetInQueueLength(dwReadLength)

		ReadCommBlock(dwReadLength)
		switch (*(lpOpPointer+c_PacketFuncOff)) 
		{	 
			case '1':
			case '2':
			case 'E':
				break;	
			default :	
				PurgeComm( hComm, PURGE_RXCLEAR );	
				WriteCommChar(DC3)
				continue;
		}

		GetLocalTime(&SystemTime);

//		wSampleID =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		TotalItems =StrToInt(lpOpPointer+c_TotalItemsOff, c_TotalItemsLen);
		nDataOff =c_DataOff;
		for (nItemCount=0;nItemCount<TotalItems;nItemCount ++)
		{

			FillSampleID(nItemCount, wSampleID)		

//			FillSampleID(nItemCount, wSampleID1)	

 			FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)

			lpPointer=lpOpPointer+nDataOff+c_ResultOff;
			DeleSpace(c_ResultLen )

//			for(nResultLen =c_ResultLen ; *lpPointer==SPACE; nResultLen--, lpPointer++); 

			FillResult(nItemCount, lpPointer ,nResultLen ) 

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

// 			FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
					 
 			FillErrorCode(nItemCount, c_Reliable)	
					
			nDataOff+=c_OneDataLen;  
		}	


	if (nItemCount>0)
	{
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;
	}
 

	PurgeComm( hComm, PURGE_RXCLEAR );	
	WriteCommChar(DC3)	


	DelayTimeCounts=0;

	do{			
		Sleep(1);
		GetInQueueLength(dwReadLength)
		DelayTimeCounts ++;
	} while(dwReadLength==0 && DelayTimeCounts<c_DelayTimeMaxCounts );

	if (DelayTimeCounts>=c_DelayTimeMaxCounts)
	{
		WriteCommChar(DC1)	
	}

}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =ETX;
}

