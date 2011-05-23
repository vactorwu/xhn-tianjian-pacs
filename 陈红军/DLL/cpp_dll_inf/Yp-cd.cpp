//********************************************************************
//  模块: YPCD1600.cpp
//  描述: 本模块为雅培公司的Cell-DYN化验仪的联机程序(Win95版)
//	函数:
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512	  
#define	c_MaxItemNum		18	  
#define c_PackIDOff			3	
#define c_PackIDLen			3	
//#define c_SampleIDOff		7	
//#define c_SampleIDLen		3	
#define c_SampleIDOff		12	
#define c_SampleIDLen		9	
#define c_AssayDateOff		26	
#define c_ResultLen			5	
#define c_RxCheckSumLen		2	
#define c_ResultPack		0	
#define c_ResultPackLen		212	
#define c_GraphPack			1	
#define c_GraphPackLen		310	

#define c_ItemNum			18	


static const int _gItemDataOff[c_ItemNum] =
	{45, 51, 57, 63, 69, 75, 81, 87, 93, 99, 105,111,117,123,129,135,141,147};


static const CHAR * _gItemID[c_ItemNum] =
	{"WBC","LYM","*MID","GRAN","RBC","HGB","HCT","MCV",
	  "MCH","MCHC", "RDW","PLT","MPV", "PCT","PDW","LYM%","MID%","GRAN%"};


static  const int _gItemResultDivFactor[c_ItemNum] =
	{10, 10, 10, 10, 100,1, 1000, 1, 10, 1, 10, 1, 10, 10, 10,10,10,10};


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData


	SYSTEMTIME SystemTime;

	int	 i,j,nItemCount, nOnePackLen, nPackType;
	BYTE  btCalCheckSum, btRxCheckSum, btErrorCode;
	LPSTR	hRXBuffer=(LPSTR)RxBuffer;
	DWORD	hReadLen;
	char	*p_dndResult1,*p_dndResult="                    ";
	double dndResult; 
	Init()
 
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	memset((LPSTR)lpOpPointer , 0, c_RxBufferLength);
	ResetRxPointer()		

		GetInQueueLength(dwReadLength)


	while (TRUE)
	{

		ResetOpPointer()

		WaitEvent(EV_RXFLAG)  

		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)	


		for (i=0;*lpOpPointer!=STX;lpOpPointer++,i++); 
		memmove(hRXBuffer , lpOpPointer ,  dwRxBufferLength-i );
		hReadLen= dwRxBufferLength-i ;
		ResetOpPointer()
		
		if (RxBuffer[2]==SPACE)			
		{
			nPackType =c_ResultPack;  
			nOnePackLen  =c_ResultPackLen;	
		}
		else
		{
			nPackType =c_GraphPack;	
			nOnePackLen  =c_GraphPackLen;	
		 }



		nItemCount =0;

		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);		
		GetLocalTime(&SystemTime);

		switch(nPackType)
		{

			case  c_ResultPack:
				for (i=0;i<c_MaxItemNum; i++)
				{
					switch (*(lpOpPointer+_gItemDataOff[i]))	
					{
						case '-':  
//							dResult =0;
//							btErrorCode =c_ErrorInvalid;
//							break;
						case '>':  
//							dResult =StrToInt(lpOpPointer+_gItemDataOff[i]+1, c_ResultLen)/(double)_gItemResultDivFactor[i];
//							btErrorCode =c_ErrorOverflow;

//							FillErrorCode(nItemCount,c_UnReliable )
							FillResult(nItemCount,lpOpPointer+_gItemDataOff[i]+1, c_ResultLen-1 )
							break;
						default:   

							dndResult =StrToInt(lpOpPointer+_gItemDataOff[i], c_ResultLen)/(double)_gItemResultDivFactor[i];
							sprintf(p_dndResult,"%f",dndResult);
							p_dndResult1 =p_dndResult+strlen(p_dndResult)-1;
							if (strchr(p_dndResult,'.')!=NULL)
								for(;*p_dndResult1=='0' ;p_dndResult1--);
							if (*p_dndResult1=='.')	p_dndResult1--;

							FillResult(nItemCount,p_dndResult , (int)(p_dndResult1-p_dndResult)+1)


					}
					FillSampleID(nItemCount, wSampleID)	

					strcpy(OutResult[nItemCount].ItemID, LPSTR(_gItemID[i]));	
				
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					FillResultType(nItemCount, c_TypeRoutine)	
					FillErrorCode(nItemCount, c_Reliable )
					nItemCount++;
				}	

				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;	
				break;

			case  c_GraphPack:
				break;
		}					


		
//		PurgeComm( hComm, PURGE_RXCLEAR );
		WriteCommChar(ACK)
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

