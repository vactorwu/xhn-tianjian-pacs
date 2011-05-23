//********************************************************************
//  模块:   RL_747.cpp
//  描述: 本模块为日本日立公司的747化验仪的联机程序(Win95版)
  

#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
 

#define c_RxBufferLength	512	
#define	c_MaxItemNum		60		
#define c_PacketFuncOff		1		
#define c_TransModeOff		3		
#define c_SampleTypeOff		2		

#define c_SampleNoOff		8		
#define c_SampleNoLen		5		
#define c_SampleIDOff		14		
#define c_SampleIDLen		13		
 

#define c_DataOff			29		
#define c_OneDataLen		9		
#define c_ItemIDOff			0		
#define c_ItemIDLen			2		
#define c_ResultOff			2		
#define c_ResultLen			6		
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1		

#define c_RxCheckSumLen		2		



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

 	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nDataOff,nResultLen,AnyTimes=0;
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	DWORD wSampleID1;
	LPSTR lpPointer,lptemp1;
	char *p_MOR  ="\x2>\x3\x33\x45\r";		
	char *p_REP  ="\x2?\x3\x33\x46\r";		
	char *p_SingleMax="\x2=D \x33\x41\x31\r";	
 	char TempRx[c_RxBufferLength];	
	char LastFuncChar;	

	Init()
 
	nItemCount =0;
	ResetOpPointer()				
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG )

//	WriteCommBlock(p_SingleMax ,strlen(p_SingleMax)) 
 
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
			//memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
			memmove( lpOpPointer, lptemp1,dwRxBufferLength );
		}


	switch (*(lpOpPointer+c_PacketFuncOff)) 
	{	 
		case	'>':
			AnyTimes++;
			if (AnyTimes%2)	
			{
				PurgeComm( hComm, PURGE_RXCLEAR );
				WriteCommBlock(p_SingleMax ,strlen(p_SingleMax))	
			}
			else	
			{
				PurgeComm( hComm, PURGE_RXCLEAR );
				WriteCommBlock(p_MOR ,strlen(p_MOR))	
			}
			continue;
//			break;
		case	'1':
 		case	'2':
 		case	':':
			AnyTimes=0;
			break;	
		
		default :	
			PurgeComm( hComm, PURGE_RXCLEAR );
			WriteCommBlock(p_MOR,strlen(p_MOR))
			continue;
	}
		nItemCount =0;
		switch (*(lpOpPointer+c_TransModeOff))	
		{				  
			case	'A': 
 
			case	'B': 
  
				GetLocalTime(&SystemTime);

				wSampleID =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
				wSampleID1 =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
				switch (*(lpOpPointer+c_SampleTypeOff ))
				{
					case	'A':
						btResultType =c_TypeSample;
						break;
					case	'I':
						btResultType =c_TypeQC;
						break;
					case	'G':
						btResultType =c_TypeStat;
						break;
					case	'B':
						btResultType = c_TypeUrine;
						break;
 					case	'C':
						btResultType =c_TypeAutoRerun ;
						break;					
				}
				if (*(lpOpPointer+ c_SampleTypeOff)=='A' ||
					*(lpOpPointer+ c_SampleTypeOff)=='B' ||
					*(lpOpPointer+ c_SampleTypeOff)=='C' ||
					*(lpOpPointer+ c_SampleTypeOff)=='G' ||
					*(lpOpPointer+ c_SampleTypeOff)=='I' )	
				{	
					for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX ;)
					{

						FillSampleID(nItemCount, wSampleID)		

//						FillSampleID(nItemCount, wSampleID1)	

 						FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)

						lpPointer=lpOpPointer+nDataOff+c_ResultOff;
						DeleSpace(c_ResultLen )

//						for(nResultLen =c_ResultLen ; *lpPointer==SPACE; nResultLen--, lpPointer++); 

						FillResult(nItemCount, lpPointer ,nResultLen ) 

						FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)

						FillResultType(nItemCount, btResultType)

 						FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
						 
 						FillErrorCode(nItemCount, c_Reliable)	
 						nItemCount++;   
					
						nDataOff+=c_OneDataLen;  

					}	
				}
				break;
		}	

	if (nItemCount>0)
	{
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;
	}

 	memmove( TempRx,lpOpPointer,dwRxBufferLength);	
	LastFuncChar=*(lpOpPointer+c_PacketFuncOff);	
 

		

	 	 PurgeComm( hComm, PURGE_RXCLEAR );	
		 WriteCommBlock(p_MOR ,strlen(p_MOR))	

 		
}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar =CR;
}

