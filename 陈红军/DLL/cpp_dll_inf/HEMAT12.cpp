//********************************************************************
//  模块: HEMAT12.cpp
//  描述: 本模块为意大利SEAE HEMAT 12型血球化验仪的联机程序(Win95版)
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	2500	
#define	c_MaxItemNum		22		
#define	c_ResultLen			4										
#define c_GraphsDataLen		2127	
#define c_SampleIDOff		0		
#define c_SampleIDLen		4		
#define	SOH					01		

static const CHAR * _gItemID[c_MaxItemNum] =
	{"RBC","WBC","Hgb","Hct","MCV","MCH","MCHC","Plt",
	 "MPV", "Pct","RDW","LYN#", "NEU#","MED#","LYN%","NEU%","MED%",
	 "T_Pl/RB","LT_WBC","T_LY/ME","T_ME/NE","N/A" 
	 };	



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,j=0,nOneFieldLen,k=0,LenDlt;
	char sRawResult[10],sResult[10];
	char	*p_dndResult1,*p_dndResult2;

	Init()
	nItemCount =0;
	ClearCommRx()		
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)


	while (TRUE)
	{

		ResetOpPointer()				
		ResetRxPointer()
		do{
			GetInQueueLength(dwReadLength)
			if (dwReadLength==0)
				continue;
			else
			{
				ReadCommBlock(dwReadLength)
				if (lpOpPointer[0]==SOH) 
				{
					WriteCommChar(ACK)
					ResetRxPointer()
					break;
				}
				else if (lpOpPointer[0]==STX)
				{
					break;
				}
				else
					ResetRxPointer()
			}
		}while(TRUE);
		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)

		*(lpOpPointer+dwRxBufferLength)='\0';

	
		GetLocalTime(&SystemTime);	
		nOneFieldLen=4;
		nItemCount =0;
		wSampleID =StrToInt(lpOpPointer+c_GraphsDataLen+c_SampleIDOff,c_SampleIDLen);

		for (i=0;i<c_MaxItemNum; i++)
		{
			if (strncmp(lpOpPointer+c_GraphsDataLen+nOneFieldLen,"0FFF",4)==0)	
			{

				FillResult(nItemCount,"    " ,c_ResultLen)
			}
			else
			{
				j=(int)c_ResultLen-(int)StrToInt(lpOpPointer+c_GraphsDataLen+nOneFieldLen,1);

				memset(sResult,0,10);
				strncpy( sResult,lpOpPointer+c_GraphsDataLen+nOneFieldLen+1 ,j-1);
				sResult[j-1]='.';

				strncpy((LPSTR) sResult+j,lpOpPointer+c_GraphsDataLen+nOneFieldLen+j,(int)c_ResultLen-j);
				sResult[c_ResultLen]=0;

				if (sResult[c_ResultLen-1]!='.')
				{

					for(p_dndResult2=sResult;*p_dndResult2=='0' &&
						*(p_dndResult2+1)!='.' ;p_dndResult2++);

					LenDlt=0;
					if (*p_dndResult2=='.'  && p_dndResult2== sResult)	
					{
						for (k=strlen(sResult);k>=0;k--) 
						{
							*(p_dndResult2+k+1)=*(p_dndResult2+k);
						}
						*p_dndResult2='0';
						LenDlt=1;
					}					

					for(p_dndResult1=(char*)(sResult+strlen(sResult)-1);*p_dndResult1=='0' ;p_dndResult1--);

					if (*p_dndResult1=='.' )	
						p_dndResult1--;	
				}
				else
				{

					for(p_dndResult2=sResult;*p_dndResult2=='0' &&
						*(p_dndResult2+1)!='.' ;p_dndResult2++);

					if (*p_dndResult2=='.')	
						p_dndResult2 --;		

					p_dndResult1=(char*)(sResult+strlen(sResult)-1);
					if (*p_dndResult1=='.' )	
						p_dndResult1--;	
				}
				strncpy(OutResult[nItemCount].AssayResult ,p_dndResult2,(int)(p_dndResult1-p_dndResult2+1));



//				strncpy(OutResult[nItemCount].AssayResult ,,strlen(sResult));
				strncpy(OutResult[nItemCount].AssayResult ,ValStrFormat(sResult),strlen(ValStrFormat(sResult)));

//				OutResult[nItemCount].AssayResult[c_ResultLen+LenDlt] ='\0';
				OutResult[nItemCount].AssayResult[strlen(ValStrFormat(sResult))] ='\0';

			}
				
			FillSampleID(nItemCount, wSampleID)	

			strcpy(OutResult[nItemCount].ItemID, LPSTR(_gItemID[i]));

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		
			nItemCount++;

			nOneFieldLen +=c_ResultLen;
		}				

		if (nItemCount > 0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		ResetOpPointer()
		ResetRxPointer()		
		ClearCommRx()		
		WriteCommChar(ACK)
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

