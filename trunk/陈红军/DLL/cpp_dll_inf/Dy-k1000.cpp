//********************************************************************
//  模块: dy-k1000.cpp
//  描述: 本模块为东亚公司的Sysmex-k1000化验仪的联机程序(Win95版)
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
  
#define c_RxBufferLength	150		
#define	c_MaxItemNum		23
									
#define c_SampleDataLength	119		
#define c_QCDataLength		97		

#define c_SampleDistCodeOff	3
#define c_SampleIDOff		11
#define c_SampleIDLen		12

#define c_ItemNum		36		
#define c_SampleItemNum	18		
#define c_QCItemNum		18		
#define c_ResultLen		4		
 

static const int _gItemDataOff[c_ItemNum] =
	{30,35,40,45,50,55,60,65, 
	 70,75,80,85,90,95,100,105,110,115,	
	 18,22,26,30,34,38,42,46,50,54,58,62,66,70,74,78,82,86}; 
static const CHAR * _gItemID[c_ItemNum] =
	{"WBC","RBC","HGB","HCT","MCV","MCH","MCHC","PLT",
	 "LYMPH%", "MXD%","NEUT%","LYMPH#", "MXD#","NEUT#","RDW-CV","PDW","MPV","P-LCR", 
	 "WBC","LYMPH%", "MXD%","NEUT%","LYMPH#", "MXD#","NEUT#","RBC","HGB","HCT","MCV","MCH","MCHC","RDW-CV","PLT","PDW","MPV","P-LCR"
	 };	
             static  const int _gItemResultDivFactor[c_ItemNum] =
	{10, 100, 1, 1000, 10, 10, 1, 1, 
	 1000, 1000, 1000, 10, 10, 10, 1000, 10, 10, 1000,
	 10, 1000, 1000, 1000, 10, 10, 10, 100, 1, 1000, 10, 10, 1, 1000, 1, 10	, 10, 1000};


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,jjj=0, nMaxItemNum;
	BYTE btAssayType;
	char	*p_dndResult="     ";
	double dndResult;

    
	Init()
	nItemCount =0;
	ResetOpPointer()				
	ResetRxPointer()
	ClearCommRx()		
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
 
	
	while (TRUE)
	{

		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
 
		ReadCommBlock(dwReadLength)

*(lpOpPointer+dwReadLength)='\0';
 

		if (*(lpOpPointer+c_SampleDistCodeOff) =='U')
			btAssayType =c_TypeSample;	
		else
			btAssayType =c_TypeQC;	

	
		GetLocalTime(&SystemTime);	
		nItemCount =0;

	
		if (btAssayType ==c_TypeSample)		
		{
			i =0, nMaxItemNum =c_SampleItemNum;
	
			wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);		
		}
		else	
		{	
			i =c_SampleItemNum, nMaxItemNum = c_ItemNum;
			wSampleID=9999;
		}
		for (;i<nMaxItemNum; i++)
		{
			if (*(lpOpPointer+_gItemDataOff[i])=='*')	
			{

				FillErrorCode(nItemCount,c_UnReliable )
				FillResult(nItemCount,lpOpPointer+_gItemDataOff[i] , c_ResultLen)
			}
			else
			{

				dndResult =StrToInt(lpOpPointer+_gItemDataOff[i], c_ResultLen)/(double)_gItemResultDivFactor[i];
				sprintf(p_dndResult,"%5f",dndResult);
				FillResult(nItemCount,p_dndResult , c_ResultLen+1)
			}
  				
				FillSampleID(nItemCount, wSampleID)	

				strcpy(OutResult[nItemCount].ItemID, LPSTR(_gItemID[i]));
				
 				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			
				FillResultType(nItemCount,  btAssayType)	
			if(btAssayType ==c_TypeSample )
			
				FillComment(nItemCount ,lpOpPointer+_gItemDataOff[i]+c_ResultLen,1)
				
				nItemCount++;
		}				

		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;

		ResetRxPointer()		
		ResetOpPointer()
		ClearCommRx()		
		WriteCommChar(ACK)
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

