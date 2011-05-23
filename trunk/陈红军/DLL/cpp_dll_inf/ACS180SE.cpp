//********************************************************************
//  模块: ACS180SE.cpp
//  描述: 本模块为美国康仁公司的ACS180SE化学发光免疫分析仪联机程序(Win95版)
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
#include <string.h>

#define c_RxBufferLength	256		
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
	int	 i, nItemCount, nOnePackLen  ,nDataLen,nItemIDLen,nSampleIDLen;
	BYTE sample_flag, btResultType, btCalCheckSum, btRxCheckSum;
	BYTE btRecordType;
 	char *lptempOpPointer, *ptemp1,*ptemp2,*ptemp3;	
 

	Init()




	nItemCount =0;

	ResetOpPointer()				



	FillDevName()					


	

	SetCommWaitMask(EV_RXCHAR )


 
	WriteCommChar(ACK)
	while (TRUE)
	{
		
		nOnePackLen =0;
 		ResetOpPointer()
		ResetRxPointer()

		do
		{
			GetInQueueLength(dwReadLength)


			if (dwReadLength ==0 )	//&& dwRxBufferLength==0)
			{
				WaitEvent(EV_RXCHAR )   

				
				Sleep(250);	
				GetInQueueLength(dwReadLength)
			}
			
			 if (dwReadLength!=0)  
			 {
 
				ReadCommBlock(dwReadLength)

				lpOpPointer[dwRxBufferLength ]='\0';
				if(strchrnb(lpOpPointer+dwRxBufferLength-dwReadLength,ENQ,1,dwReadLength)!=NULL)
				{
						ResetRxPointer()
						ResetOpPointer()
						WriteCommChar(ACK)	
	   					continue;
				}

				ptemp3=lpOpPointer+strlen(lpOpPointer)-1;
// 				if (strbchrnb(ptemp3,LF,1,3)!=NULL)
				if (*ptemp3==LF)
				{
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
					
					nOnePackLen=strlen( lptempOpPointer);
					break;	

				}
			 }


		}while(TRUE);	


 	 
 
btRecordType=lptempOpPointer[2];

switch(btRecordType)	//switch---A
{
	case 'H':		
	
		sample_flag=*(strchrnb(lptempOpPointer,CR,1,MaxSeekRange )-sample_flagOff);	
 		btResultType = c_TypeSample; 
		if (sample_flag=='Q') btResultType = c_TypeQC; 
		break;
	case 'O':		

		ptemp1 =strchrnb(lptempOpPointer,'^',1, SeekRange);
		ptemp2 =strchrnb(ptemp1 ,'A',1,8);
		if(ptemp2==NULL)
			ptemp2 =strchrnb(ptemp1 ,'B',1,8)+1;
		else
			ptemp2 ++;
		ptemp3 =strchrnb(ptemp2 ,'|',1,8);
 		nSampleIDLen=ptemp3-ptemp2;
		wSampleID=StrToInt(ptemp2 , nSampleIDLen);	

		GetLocalTime(&SystemTime);
		nItemCount=0;
		break;
	case 'R':	
	
		ptemp1=strchrnb(lptempOpPointer,'^',1, SeekRange);
		for (;*ptemp1=='^';ptemp1++);
		ptemp2=strchrnb(ptemp1,'|',1, SeekRange);	
		nItemIDLen =ptemp2 - ptemp1 ;

		FillItemName(nItemCount,ptemp1,nItemIDLen)	
		FillSampleID(nItemCount, wSampleID)	
		ptemp1=strchrnb(lptempOpPointer,'|',3, SeekRange*2)+1;
		ptemp2=strchrnb(ptemp1,'|',1, SeekRange);
		FillResult(nItemCount,ptemp1, ptemp2-ptemp1 )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

		
		



	 

//		FillItemName(nItemCount,ptemp1+3 ,nItemIDLen-3)	

//		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)


	
 
//		if (strncmp(ptemp2+1,".....",5)==0 || strncmp(ptemp2+1,"+++++",5)==0
//			|| strncmp(ptemp2+1,"-----",5)==0 || strncmp(ptemp2+1,"     ",5)==0)
//		{

//			FillErrorCode(nItemCount,c_UnReliable )			
//			nDataLen=5;	

//		}
//		else 
//		{
//			FillErrorCode(nItemCount, c_Reliable )	


//			ptemp4=strchrnb(ptemp2+1,'!',1,SeekRange);	
//
//			if( ptemp4 !=NULL )	
//			{

//				ptemp1=strchrnb(ptemp4+1 ,Delimiter ,1, SeekRange);
//				nDataLen=ptemp1-(ptemp4+1);
// 				FillComment(nItemCount, ptemp4+1 , 1)	//nDataLen)

//				nDataLen=ptemp4-(ptemp2+1);	
//			}
//
//			else	
//			{
//				ptemp1=strchrnb(ptemp2+1 ,Delimiter ,1, SeekRange);
//				nDataLen=ptemp1-(ptemp2+1);
//			}
//		}


//
//		FillResult(nItemCount,ptemp2+1, nDataLen )

 
	
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
								\
//	TxChar =btTxChar;										\
//	WriteFile(hComm, &TxChar, 1, &dwWrittenLength, NULL);
		ResetRxPointer()
		ResetOpPointer()
 		continue;	
	
}//WHILE--END

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
//	pComDcb->EvtChar =LF;
}

