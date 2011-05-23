//********************************************************************
//  本模块为Beckman公司的Array化验仪(血清免疫蛋白)的联机程序(Win95版)
//  描述: 全称：Array Protein System （360/360E） 
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	256	
#define	c_MaxItemNum		1	
#define c_FuncNumOff		1	
#define c_SampleIDLen       15 
#define c_SampleIDOff       7
#define c_ItemIDOff         4
#define c_ItemIDLen         3	
#define c_ResultOff         20
#define c_ResultLen         6	
#define c_IndicatorOff      36	

#define	c_IndicatorLen		2



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,j,nItemCount;
	BYTE  PackFun ;
	BOOL	OddFlag=FALSE;
	char PF_Result[20],TempResult[10];
	char *p_ACK1="\x10\x31";
	char *p_ACK0="\x10\x30";

	Init()
	ResetOpPointer()				


	ResetRxPointer()


 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	WriteCommChar(XON)
	while (TRUE)
	{



		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 
			
		OddFlag=!OddFlag;
		PackFun=*(lpOpPointer+c_FuncNumOff);
		j=atoi(lpOpPointer+c_FuncNumOff);
	switch(j)
	{
		case 20:
			WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	
			OddFlag=FALSE;
			nItemCount =0;
			ResetRxPointer() 	
//			PurgeComm( hComm, PURGE_RXCLEAR );
			continue;
		case 21:
			i =atoi(lpOpPointer+c_SampleIDOff);
			wSampleID =(i==0) ? wSampleID+1 : i;
			GetLocalTime(&SystemTime);		
			if(OddFlag)
			{
				WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	
			}
			else
			{
				WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	
			}
			nItemCount =0;
			ResetRxPointer() 	
//			PurgeComm( hComm, PURGE_RXCLEAR );
			continue;
		case 22:


			FillSampleID(nItemCount, wSampleID)	

			FillItemName(nItemCount,lpOpPointer+c_ItemIDOff ,c_ItemIDLen)	

		if (!strncmp(OutResult[nItemCount].ItemID ,"RF",2))
		{
				switch (*(lpOpPointer+c_IndicatorOff+1))
				{
				case 'O':

					PF_Result[0] ='<';
					break;
				case 'I':
				case 'X':
	
					PF_Result[0] ='>';
					break;
				case 'R':
					PF_Result[0] =' ';
					break;
				case 'A':
					PF_Result[0] =' ';
					break;
				default :
					PF_Result[0] =' ';
				}
				PF_Result[1] ='\0';
				strncpy(TempResult, lpOpPointer+c_ResultOff, c_ResultLen);
				strcat(PF_Result,TempResult );
				strncpy(OutResult[nItemCount].AssayResult,PF_Result , c_ResultLen+1);
				OutResult[nItemCount].AssayResult[c_ResultLen+1] ='\0';
		}
		else
			{

			FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)
		}

		FillComment(nItemCount,lpOpPointer+c_IndicatorOff,c_IndicatorLen )
 

			if (*(lpOpPointer+c_IndicatorOff+1)=='A' )
				FillErrorCode(nItemCount, c_UnReliable)	
			else 
				FillErrorCode(nItemCount, c_Reliable)

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			nItemCount++;



		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;	
		ResetRxPointer()		



		if(OddFlag)
		{
			WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	//
		}
		else
		{
			WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	//
		}

			continue;
		default:
			nItemCount =0;
			ResetRxPointer() 	
			if(OddFlag)
			{
				WriteCommBlock(p_ACK0 ,strlen(p_ACK0))	//SEND DEL_0
			}
			else
			{
				WriteCommBlock(p_ACK1 ,strlen(p_ACK1))	//SEND DEL_1
			}
//			PurgeComm( hComm, PURGE_RXCLEAR );			
			continue;
	}


	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

