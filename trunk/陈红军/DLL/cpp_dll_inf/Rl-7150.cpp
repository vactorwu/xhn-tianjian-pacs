//********************************************************************
//  模块: RL7150.cpp
//  描述: 本模块为日立公司的7150化验仪的联机程序(Win95版)


#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
  

#define c_RxBufferLength	300		 
#define	c_MaxItemNum		23		  
#define c_FuncNumOff		1		
#define c_FuncNumLen		2		
#define c_SampleInfoOff		3		
#define c_SampleIDOff		4		
#define c_SampleIDLen		4		
#define c_ControlIDOff		6		
#define c_ControlIDLen		2		
#define c_StatIDOff			5		
#define c_StatIDLen			3		
#define c_DataOff			25		
#define c_OneDataLen		9		
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
	int	 i, nItemCount, nFuncNum, nOnePackLen, nDataOff,nResultLen;
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	DWORD dwReadLength1;
	LPSTR lpPointer;

	Init()
	nItemCount =0;
	ResetOpPointer()				

	FillDevName()					

	SetCommWaitMask(EV_RXFLAG )
	nItemCount=0;
	while (TRUE)
	{

		ResetRxPointer()		

		do {
				GetInQueueLength(dwReadLength)
				if (dwReadLength==0 && dwRxBufferLength==0)
				{

					WaitEvent(EV_RXFLAG  )  

					GetInQueueLength(dwReadLength)
				}
 				if(dwReadLength!=0 )
				{
					ReadCommBlock(dwReadLength)

					if (lpOpPointer[0]!=STX) 
					{
						memcpy( lpOpPointer, lpRxPointer-dwRxBufferLength+1,dwRxBufferLength-1  );
						lpRxPointer--;
						dwRxBufferLength--;
					}
					if ( dwRxBufferLength>=2)
					{
						if( (*(lpRxPointer-2)== ETX)  )
						break;
					}
				}
		}while(TRUE);




		nFuncNum =StrToInt(lpOpPointer+c_FuncNumOff, c_FuncNumLen);
		nItemCount =0;
		switch (nFuncNum)
		{				  
			case	2:
			case	3:
			case	52:
			case	53:
				 
				GetLocalTime(&SystemTime);
				switch (*(lpOpPointer+c_SampleInfoOff))
				{


				case	'N':
						wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
						btResultType =c_TypeSample;
						break;
					case	'C':
						wSampleID =StrToInt(lpOpPointer+c_ControlIDOff, c_ControlIDLen);
						btResultType =c_TypeQC;
						break;
					case	'E':
						wSampleID =StrToInt(lpOpPointer+c_StatIDOff, c_StatIDLen);
						wSampleID +=1000;
						btResultType =c_TypeEmergency;
						break;
				}
				if (*(lpOpPointer+c_SampleInfoOff)=='N' || 
					*(lpOpPointer+c_SampleInfoOff)=='C' ||
					*(lpOpPointer+c_SampleInfoOff)=='E')
					for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX && *(lpOpPointer+nDataOff)!=ETB;)
					{
						FillSampleID(nItemCount, wSampleID)	
						FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)	
						DeleZero(lpOpPointer,c_ResultLen)

						FillResult(nItemCount, lpPointer ,nResultLen ) 
						FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
						FillResultType(nItemCount, btResultType)
						FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	

						FillErrorCode(nItemCount, c_Reliable)					
 						nItemCount++;   
						nDataOff+=c_OneDataLen; 
					}
				break;
		}
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

 
 

	PurgeComm( hComm, PURGE_RXCLEAR );	
		if (nFuncNum>50)	
		{
		 	WriteCommChar(ACK)
		}
 		
}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{

	pComDcb->EvtChar =STX;
}

