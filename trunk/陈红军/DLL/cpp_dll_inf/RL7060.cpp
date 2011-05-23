//********************************************************************
//  模块:   RL7060.cpp
//  描述: 本模块为日本日立公司的7060化验仪的联机程序(Win95版)

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

 
#define c_RxBufferLength	300		
#define	c_MaxItemNum		20		

#define c_PacketFuncOff		1		
#define c_TransModeAndSampleTypeOff		3		
 
 
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


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

 	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nDataOff,nResultLen;
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	DWORD wSampleID1;
	LPSTR lpPointer,lptemp1;
	const char *p_MOR  ="\x2>\x3=";		
	const char *p_REP  ="\x2?\x3<";		
 
 	

	Init()
		

	nItemCount =0;
	ResetOpPointer()				
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG )
 
	
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
 

	if ((*(lpOpPointer+c_PacketFuncOff)!='1') &&
		(*(lpOpPointer+c_PacketFuncOff)!='2') &&
		(*(lpOpPointer+c_PacketFuncOff)!=':'))
	{
		PurgeComm( hComm, PURGE_RXCLEAR );
		WriteCommBlock(p_MOR ,strlen(p_MOR))	
		continue;
	}

		nItemCount =0;
 		GetLocalTime(&SystemTime);

		wSampleID =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
		wSampleID1 =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		btResultType=*(lpOpPointer+ c_TransModeAndSampleTypeOff);
		if (btResultType=='A' || btResultType=='a' || btResultType=='B' ||
			btResultType=='b' || btResultType=='S' || btResultType=='s' ||
			btResultType=='T' || btResultType=='t' )
				btResultType =c_TypeSample;
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
				btResultType =c_TypeStat ;
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
				btResultType =c_TypeQC ;
			
		if (btResultType ==c_TypeSample || btResultType ==c_TypeStat ||
				btResultType ==	 c_TypeQC)	
		{	
			for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX ;)//for===A
			{


			FillSampleID(nItemCount, wSampleID1)	

 				FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)

				lpPointer=lpOpPointer+nDataOff+c_ResultOff;
				DeleSpace(c_ResultLen )

				FillResult(nItemCount, lpPointer ,nResultLen ) 

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)

				FillResultType(nItemCount, btResultType)

 				FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
					 
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

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =ETX;
}

