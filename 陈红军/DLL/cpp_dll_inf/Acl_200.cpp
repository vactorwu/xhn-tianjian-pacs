//********************************************************************
//  模块: clt-acl.cpp
//  描述: 本模块为库尔特(Coulter)公司的acl系列分析仪的联机程序(Win95版)
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	100		  
#define	c_MaxItemNum		9		  

#define	c_StartSymbolLength	1		 
#define	c_ProtocolHeadLength 16		  
#define	c_MsgHeadLength		52
#define	c_DataMsgLength		28
#define	c_BatchMsgLength	17
#define c_MsgLengthOff		11
#define c_MsgLengthLen		4
#define c_NormalPTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_NormalFIBOff	c_StartSymbolLength+c_ProtocolHeadLength+6
#define c_NormalAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define	c_NormalPTFIBAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength+12	
#define c_NormalTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_NormalTTAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength+6
#define c_TestPTOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestFIBOff	c_StartSymbolLength+c_ProtocolHeadLength+13
#define c_TestAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestTTOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestIDOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_MsgIDOff		c_StartSymbolLength+c_ProtocolHeadLength+24
#define c_FormatOff		c_StartSymbolLength+c_ProtocolHeadLength+4
#define c_NormalPTLen	4
#define c_NormalFIBLen	4
#define c_NormalAPTTLen	4
#define c_NormalTTLen	4
#define c_TestPTLen		4
#define c_TestFIBLen	4
#define c_TestAPTTLen	4
#define c_TestTTLen		4
#define c_TestIDLen		2
#define c_MsgIDLen		2
#define	c_RxCheckSumLen 2
#define	c_TestPERCENOff	c_StartSymbolLength+c_ProtocolHeadLength+5
#define	c_TestPERCENLen	3
#define	c_TestR_INROff	c_StartSymbolLength+c_ProtocolHeadLength+9
#define	c_TestR_INRLen	4
#define	c_TestAPTT_ROff	c_StartSymbolLength+c_ProtocolHeadLength+4
#define	c_TestAPTT_RLen	4
#define	c_TestTT_ROff	c_StartSymbolLength+c_ProtocolHeadLength+4
#define	c_TestTT_RLen	4

#define c_PT_FIB		3
#define c_APTT			6

#define c_PT_FIB_APTT	27



#define c_batch4IDOff   63  
#define c_batch4IDLen   4



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData	



		
	SYSTEMTIME SystemTime;
	int	i, nOneMsgLength, nRxCheckSum, nCalCheckSum, nItemCount;
	unsigned char  lpszNormalPT[c_NormalPTLen+1], lpszNormalFIB[c_NormalFIBLen+1],
		lpszNormalAPTT[c_NormalAPTTLen+1], lpszNormalTT[c_NormalTTLen+1];
	BYTE btTestID, btMsgID,PT_FIB_APTT_FLAG=0, TT_APTT_FLAG=0;
	BOOL bUnitIsmgdl =TRUE;
	div_t CheckSumDiv;
	
	unsigned int batchNo;



	Init()

	memset(LPSTR(lpszNormalPT), 0, c_NormalPTLen+1);
	memset(LPSTR(lpszNormalFIB), 0, c_NormalFIBLen+1);
	memset(LPSTR(lpszNormalAPTT), 0, c_NormalAPTTLen+1);
	memset(LPSTR(lpszNormalTT), 0, c_NormalTTLen+1);

	ResetOpPointer()		
 		

	FillDevName()			
 
	SetCommWaitMask(EV_RXFLAG)
 
	while (TRUE)
	{
		
		nOneMsgLength=0;

		ResetRxPointer()		


		GetInQueueLength(dwReadLength)
 
		if (dwReadLength ==0)
		WaitEvent(EV_RXFLAG)
 

		FetchCommBlock(c_StartSymbolLength +c_ProtocolHeadLength)
		 
 


		nOneMsgLength =HStrToInt(lpOpPointer+c_MsgLengthOff, c_MsgLengthLen);
		FetchCommBlock(nOneMsgLength +c_RxCheckSumLen)
	


		nRxCheckSum =HStrToInt(lpOpPointer+c_StartSymbolLength
					+c_ProtocolHeadLength +nOneMsgLength, 
					c_RxCheckSumLen);  

		for(nCalCheckSum=0, i =1; i<int(dwRxBufferLength-c_RxCheckSumLen); i++)
			nCalCheckSum +=RxBuffer[i];
		do{
			CheckSumDiv =div(nCalCheckSum, 256);
			nCalCheckSum =CheckSumDiv.quot +CheckSumDiv.rem;
		}while(nCalCheckSum>256);
		nCalCheckSum =256 -nCalCheckSum;
		if (nCalCheckSum !=nRxCheckSum)
		{ 
			WriteCommChar('!');
			WriteCommBlock("????", 4);
			continue;
		}

	
		switch (nOneMsgLength)	
		{
			case	c_MsgHeadLength:  

				btTestID =(BYTE)HStrToInt(lpOpPointer+c_TestIDOff, c_TestIDLen); 

if (lpOpPointer[c_FormatOff]=='0' || lpOpPointer[c_FormatOff]=='1' || lpOpPointer[c_FormatOff]=='8' || lpOpPointer[c_FormatOff]=='9')
	bUnitIsmgdl	=TRUE;			
else
	bUnitIsmgdl	=FALSE;
				GetLocalTime(&SystemTime);		
				nItemCount =0;
			
				batchNo =StrToInt(lpOpPointer+c_batch4IDOff, c_batch4IDLen)-1;
 				break;
 			
			case	c_DataMsgLength:
				btMsgID =StrToInt(lpOpPointer+c_MsgIDOff, c_MsgIDLen);
				switch (btMsgID)
				{
					case  50:	
						break;
					case  60:	 
						switch (btTestID)	
						{
							case  c_PT_FIB:
								strncpy(LPSTR(lpszNormalPT), lpOpPointer+c_NormalPTOff+1, c_NormalPTLen);	
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--;
								strncpy(LPSTR(lpszNormalFIB), lpOpPointer+c_NormalFIBOff+1, i);	 
								lpszNormalFIB[i] ='\0';
								break;
							case  c_APTT:
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalAPTTOff+1, c_NormalAPTTLen);
								break;
							case  c_TT:
								strncpy(LPSTR(lpszNormalTT), lpOpPointer+c_NormalTTOff+1, c_NormalTTLen);
								break;
							case  c_PT_FIB_APTT:
strncpy(LPSTR(lpszNormalPT), lpOpPointer+c_NormalPTOff+1, c_NormalPTLen);	
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; 
strncpy(LPSTR(lpszNormalFIB), lpOpPointer+c_NormalFIBOff+1, i);	 //Error
								lpszNormalFIB[i] ='\0';
if (bUnitIsmgdl)	
strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalPTFIBAPTTOff, c_NormalAPTTLen);				 				
else
strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalPTFIBAPTTOff+1 , c_NormalAPTTLen);	
								break;
								break;
							case  c_TT_APTT:
								strncpy(LPSTR(lpszNormalTT), lpOpPointer+c_NormalTTOff+1, c_NormalTTLen);
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalTTAPTTOff+1, c_NormalAPTTLen);
								break;
 						}
						break;

						default:
						if (btMsgID >18)
							break;

											
						wSampleID =(WORD)(btMsgID+batchNo);
						
						switch (btTestID)	//switch (btTestID)--B
						{
							case  c_PT_FIB:
								FillItemName(nItemCount, "NP_PT", 5)
 								FillSampleID(nItemCount, wSampleID)	
								FillResult(nItemCount,LPSTR(lpszNormalPT) ,c_NormalPTLen )
  		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;

								FillItemName(nItemCount, "PT", 2)
 								FillSampleID(nItemCount, wSampleID)	
							    FillResult(nItemCount, lpOpPointer+c_TestPTOff, c_TestPTLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 
								nItemCount++;

								FillItemName(nItemCount, "PERCEN", 6)
 								FillSampleID(nItemCount, wSampleID)	
								FillResult(nItemCount, lpOpPointer+c_TestPERCENOff,c_TestPERCENLen) 
if (*(lpOpPointer+c_TestPERCENOff)=='*' || *(lpOpPointer+c_TestPERCENOff)=='-')
{
	FillErrorCode(nItemCount, c_UnReliable )	
//	FillComment(nItemCount,"OVERFLOW" ,8 )		
}
else 
{
	if(*(lpOpPointer+c_TestPERCENOff-1)!='*' && *(lpOpPointer+c_TestPERCENOff-1)!=' ') //有错误代码
	{
//		FillComment(nItemCount,lpOpPointer+c_TestPERCENOff-1 ,1)		
		FillErrorCode(nItemCount, c_UnReliable )	
	}
	
//	if ( *(lpOpPointer+c_TestPERCENOff-1)=='*')  FillComment(nItemCount,"OUTRANGE" ,8 )	
}								

		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 
								nItemCount++;

								FillItemName(nItemCount, "R_INR", 5)
 								FillSampleID(nItemCount, wSampleID)	//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestR_INROff, c_TestR_INRLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)		 
								nItemCount++;
 								
								FillItemName(nItemCount, "NP_FIB", 6)
								FillSampleID(nItemCount, wSampleID)	//样本号
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; //FIB单位为mg/dl或//FIB单位为g/l
 								FillResult(nItemCount,LPSTR(lpszNormalFIB), i ) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "FIB", 3)
								FillSampleID(nItemCount, wSampleID)	//样本号
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; //FIB单位为mg/dl或//FIB单位为g/l
 								FillResult(nItemCount,lpOpPointer+c_TestFIBOff+1 ,i ) 
if (*(lpOpPointer+c_TestFIBOff+1)=='*' || *(lpOpPointer+c_TestFIBOff+1)=='-')
{
	FillErrorCode(nItemCount, c_UnReliable )	
//	FillComment(nItemCount,"OVERFLOW" ,8 )		
}
else 
{
	if(*(lpOpPointer+c_TestFIBOff)!='*' && *(lpOpPointer+c_TestFIBOff)!=' ') 
	{
//		FillComment(nItemCount,lpOpPointer+c_TestFIBOff ,1)		
		FillErrorCode(nItemCount, c_UnReliable )	
	}
	
//	if ( *(lpOpPointer+c_TestFIBOff)=='*')  FillComment(nItemCount,"OUTRANGE" ,8 )	
}
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								break;
							case  c_APTT:
								FillItemName(nItemCount, "NP_APTT", 7)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,LPSTR(lpszNormalAPTT), c_NormalAPTTLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "APTT", 4)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen)
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								
								FillItemName(nItemCount, "APTT_R", 6)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;  								
								break;

							case  c_TT:
								FillItemName(nItemCount, "NP_TT", 5)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,LPSTR(lpszNormalTT) , c_NormalTTLen ) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "TT", 2)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestTTOff, c_TestTTLen  ) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								

								FillItemName(nItemCount, "TT_R", 4)
								FillSampleID(nItemCount, wSampleID)//样本号
								FillResult(nItemCount,lpOpPointer+c_TestTT_ROff,c_TestTT_RLen)
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								break;

							case  c_PT_FIB_APTT:
if(PT_FIB_APTT_FLAG==0)
{
								FillItemName(nItemCount, "NP_PT", 5)
 								FillSampleID(nItemCount, wSampleID)	//样本号
 								FillResult(nItemCount,LPSTR(lpszNormalPT), c_NormalPTLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "PT", 2)
 								FillSampleID(nItemCount, wSampleID)	//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestPTOff, c_TestPTLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)		 
								nItemCount++;

								FillItemName(nItemCount, "PERCEN", 6)
 								FillSampleID(nItemCount, wSampleID)	//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestPERCENOff  , c_TestPERCENLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "R_INR", 5)
 								FillSampleID(nItemCount, wSampleID)	//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestR_INROff, c_TestR_INRLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
 								
								FillItemName(nItemCount, "NP_FIB", 6)
								FillSampleID(nItemCount, wSampleID)	//样本号
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; 
 								FillResult(nItemCount,LPSTR(lpszNormalFIB),i) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)		 
								nItemCount++;

								FillItemName(nItemCount, "FIB", 3)
								FillSampleID(nItemCount, wSampleID)	//样本号
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; 
 								FillResult(nItemCount,lpOpPointer+c_TestFIBOff+1 ,i) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
PT_FIB_APTT_FLAG=1;
}
else
{
//APTT RESULT	
								FillItemName(nItemCount, "NP_APTT", 7)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,LPSTR(lpszNormalAPTT), c_NormalAPTTLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "APTT", 4)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;

								FillItemName(nItemCount, "APTT_R", 6)
								FillSampleID(nItemCount, wSampleID)//样本号
 								FillResult(nItemCount,lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen) 
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++; 
PT_FIB_APTT_FLAG=0;
}
 								break;
		 
							case  c_TT_APTT:
								if (TT_APTT_FLAG ==0)
								{
									//TT Result
									FillItemName(nItemCount, "NP_TT", 5)
									FillSampleID(nItemCount, wSampleID)//样本号
 									FillResult(nItemCount,LPSTR(lpszNormalTT) , c_NormalTTLen)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;

									FillItemName(nItemCount, "TT", 2)
									FillSampleID(nItemCount, wSampleID)//样本号
 									FillResult(nItemCount,lpOpPointer+c_TestTTOff, c_TestTTLen) 
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									

									FillItemName(nItemCount, "TT_R", 4)
									FillSampleID(nItemCount, wSampleID)//样本号
									FillResult(nItemCount,lpOpPointer+c_TestTT_ROff,c_TestTT_RLen)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									TT_APTT_FLAG =1;
								}
								else
								{
									FillItemName(nItemCount, "NP_APTT", 7)
									FillSampleID(nItemCount, wSampleID)//样本号
 									FillResult(nItemCount,LPSTR(lpszNormalAPTT), c_NormalAPTTLen) 
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;

									FillItemName(nItemCount, "APTT", 4)
									FillSampleID(nItemCount, wSampleID)//样本号
 									FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									
									FillItemName(nItemCount, "APTT_R", 6)
									FillSampleID(nItemCount, wSampleID)//样本号
 									FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									TT_APTT_FLAG =0;
								}
 								break;
						}//end of switch (btTestID)--B

						if (nItemCount>0)
						{
							(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
							lpDevice->dwRecordCount+=nItemCount;
						}
						nItemCount =0;
				}//end of switch (btMsgID)
				break;
			case	c_BatchMsgLength:
				break;
		}

		ClearCommRx()
		PurgeComm( hComm, PURGE_TXCLEAR );
 
			WriteCommChar('!')
 
			WriteCommBlock(lpOpPointer+1, 4);
	
 	
}//end of while(TRUE)

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =':';
}

 