//********************************************************************
//  模块: BMHE2010.cpp
//  描述: BM/Hitachi(保灵曼/日立)Elecsys 2010电化学发光自动标记免疫分析仪联机程序(Win95版)
 
//


#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
#include <string.h>

#define c_RxBufferLength	200	
#define	c_MaxItemNum		20		
#define c_RxCheckSumLen		2		
#define SeekRange           10		
#define MaxSeekRange        256		
#define Delimiter	'|'				
#define	sample_flagOff		18		





DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData

	SYSTEMTIME SystemTime;
	int	 nItemCount,nDataLen,nItemIDLen,nSampleIDLen;
	BYTE btRecordType;
 	char *lptempOpPointer, *ptemp1,*ptemp2,*ptemp4;	
 

	Init()
	PurgeComm( hComm, PURGE_RXCLEAR );
	nItemCount =0;
	WriteCommChar(ENQ)
	FillDevName()					

//	SetCommWaitMask(EV_RXCHAR )
	SetCommWaitMask(EV_RXFLAG)
	while (TRUE)
	{
		

 		ResetOpPointer()
		ResetRxPointer()
		do
		{

			GetInQueueLength(dwReadLength)
			if (dwReadLength ==0 )	
				continue;

			ReadCommBlock(dwReadLength)

			if(strchrnb(lpOpPointer+dwRxBufferLength-dwReadLength,ENQ,1,dwReadLength)!=NULL )
			{
		 		PurgeComm( hComm, PURGE_RXCLEAR );
				ResetRxPointer()
				ResetOpPointer()
				WriteCommChar(ACK)	
			}
			if ( dwReadLength >=3)	
				break;
		}while(TRUE);
			
		WaitEvent(EV_RXFLAG )  
		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
		lpOpPointer[dwRxBufferLength ]='\0';

	
		lptempOpPointer=strchrnb(lpOpPointer,STX,1,SeekRange);
		if (lptempOpPointer==NULL)  
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			ResetRxPointer()
			ResetOpPointer()
			WriteCommChar(NAK)
			continue;
		}


 




btRecordType=lptempOpPointer[2];

switch(btRecordType)	
{
	case 'H':	


		break;
	case 'O':	

		ptemp1 =strchrnb(lptempOpPointer,Delimiter,2, SeekRange*2)+1;
		ptemp2 =strchrnb(ptemp1,Delimiter,1, SeekRange);

 		nSampleIDLen=ptemp2-ptemp1;
		wSampleID=StrToInt(ptemp1 , nSampleIDLen);		
		

		GetLocalTime(&SystemTime);
		nItemCount=0;
		break;
	case 'R':	

	ptemp1=strchrnb(lptempOpPointer,'!',3, SeekRange*2)+1;
	ptemp2=strchrnb(lptempOpPointer,Delimiter,3, SeekRange*2);	
	nItemIDLen =ptemp2 - ptemp1 ;



		 
		FillSampleID(nItemCount, wSampleID)	

		FillItemName(nItemCount,ptemp1,nItemIDLen)	

		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

//		FillResultType(nItemCount, btResultType)


//			FillErrorCode(nItemCount,c_UnReliable )			


			ptemp4=strchrnb(ptemp1,Delimiter,1,SeekRange)+1;	
			ptemp1=strchrnb(ptemp4 ,'!' ,1, SeekRange);
			if (ptemp1==NULL) 
				ptemp1=strchrnb(ptemp4,Delimiter,1, SeekRange);
			nDataLen=ptemp1-ptemp4;


		FillResult(nItemCount,ptemp4, nDataLen )

	nItemCount++;	
 		break;
	case 'L':	
	
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		break;
	default:	
 ;
}//end case--A

 		PurgeComm( hComm, PURGE_RXCLEAR );
		WriteCommChar(ACK)

 		continue;	
	
}//WHILE--END

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

