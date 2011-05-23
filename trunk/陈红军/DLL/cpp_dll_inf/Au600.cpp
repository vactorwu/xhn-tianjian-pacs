//********************************************************************
//  模块:  olympus.cpp
//  描述: 本模块为奥林巴斯公司的olympus_AU600化验仪的联机程序(Win95版)
//	函数:
  
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		
#define	c_MaxItemNum		35		 
#define c_PackTypeOff		1
#define c_TranTypeOff		2
//#define c_UnitNumOff		3

 
#define c_SampleNoOff		6		

#define c_SampleNoLen		4

									
#define c_OneDataLen		10
#define c_ItemIDOff			0		
#define c_ItemIDLen			2
#define c_ResultOff			2		
#define c_ResultLen			6
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1		




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nPackType, nTranType, nDataOff,nResultLen;
	BYTE  btCalCheckSum, btRxCheckSum;
	WORD 		wSampleNo;						
	WORD 		wRSampleNo;	
 	BYTE		btResultType;
	LPSTR		lpPointer;

	Init()
	nItemCount =0;
	ResetOpPointer()				
	FillDevName()					
	PurgeComm( hComm, PURGE_RXCLEAR );
	SetCommWaitMask(EV_RXFLAG)
	
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

		i =2;
		btCalCheckSum =lpOpPointer[1];
		do {
		   btCalCheckSum ^=lpOpPointer[i];
		   if (lpOpPointer[i] ==ETX )	//||(lpOpPointer[i] ==ETB)
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

				if (lpOpPointer[c_TranTypeOff+1]==ETX) 	break;

			default:
 
 				//GetSystemTime(&SystemTime);
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
//					wSampleNo = wRSampleNo+6000;
					btResultType =c_TypeEmergency;
					break;
				case	'E':
					wSampleNo =StrToInt(lpOpPointer+c_SampleNoOff+1, c_SampleNoLen-1);
//					wSampleNo = wRSampleNo+4000;
					btResultType =c_TypeEmergency;
					break;

				}

			}
 
			if(btResultType != 0xff)	
				for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX  ;)
				{
				FillSampleID(nItemCount, wSampleNo)	
				FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)	//项目号
			
				FillResult(nItemCount,  lpOpPointer+nDataOff+c_ResultOff,c_ResultLen )

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

				FillResultType(nItemCount, btResultType)	

//				FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	//填状态注解


				FillErrorCode(nItemCount, c_Reliable)	
				nItemCount++;
				nDataOff+=c_OneDataLen;
				}
			break;
		}


			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;

		ResetRxPointer()


		PurgeComm( hComm, PURGE_RXCLEAR );
 		WriteCommChar(ACK)

  }

	return TRUE;

} // end of Comm WatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

