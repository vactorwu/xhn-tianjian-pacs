//********************************************************************
//  模块: F820.cpp
//  描述: 本模块为东亚公司的Sysmex-F820半自动血球化验仪的联机程序(Win95版)
  
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	128		
#define	c_MaxItemNum		20		
									
#define c_DataCodeOff		3
#define c_SampleIDOff		11
#define c_SampleIDLen		12


#define c_QCIDOff			15
#define c_QCIDLen			2

#define c_ItemNum			38		
#define c_SampleItemNum		18		
#define c_QCItemNum			20		
//#define c_ResultLen			4		
//#define c_QCResultLen		4		



static const int _gItemDataOff[c_ItemNum] =
	{30,35,40,45,50,55,60,65, 
	 70,75,80,85,90,95,100,105,110,116,	 
	 18,22,26,30,34,38,42,46,50,54,58,62,66,70,74,78,82,86,90,94}; 


static const int _gItemDataLen[c_ItemNum] =
	{4,4,4,4,4,4,4,4, 
	 4,4,4,4,4,4,4,4,4,4,	
	 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}; 


static const CHAR * _gItemID[c_ItemNum] =
	{"WBC","RBC","HGB","HCT","MCV","MCH","MCHC","PLT",
	 "W-SCR", "W-MCR","W-LCR","W-SCC", "W-MCC","W-LCC","RDW-CV","PDW","MPV","P-LCR", 
	 "WBC","W-SCR", "W-MCR","W-LCR","W-SCC", "W-MCC","W-LCC","RBC","HGB","HCT","MCV","MCH","MCHC","RDW-C/S","PLT","PDW","MPV","P-LCR","W-SMV","W-LMV"
	 };	


static  const int _gItemPointPos[c_ItemNum] =
	{4, 3, 5, 2, 4, 4, 5, 5, 
	 2, 2, 2, 4, 4, 4, 2, 4, 4, 2,
	 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4 };

//************************************************************************

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,jjj=0, nMaxItemNum;
	BYTE btAssayType;
	char	*p_dndResult1,*p_dndResult2;
	double	f_Result;
	char sRawResult[10],sResult[10];	

    
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


		if (*(lpOpPointer+c_DataCodeOff) =='U')
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
			ResetOpPointer()
			ResetRxPointer()		
			ClearCommRx()		
			WriteCommChar(ACK)
			continue;
//			i =c_SampleItemNum, nMaxItemNum = c_ItemNum;
//			wSampleID=StrToInt(lpOpPointer+c_QCIDOff, c_QCIDLen);	
		}

		for (;i<nMaxItemNum; i++)
		{
			if (*(lpOpPointer+_gItemDataOff[i])=='*')	
			{

				FillErrorCode(nItemCount,c_UnReliable )

				FillResult(nItemCount,"     " ,5)
					
//				FillResult(nItemCount,lpOpPointer+_gItemDataOff[i] ,_gItemDataLen[i])
			}
			else if (*(lpOpPointer+_gItemDataOff[i])==' ')	
			{

//				FillErrorCode(nItemCount,c_UnReliable )
//				strncpy(OutResult[nItemCount].AssayResult ,"     ",5);
//				OutResult[nItemCount].AssayResult[_gItemDataLen[i]+1] ='\0';
				continue;
			}
			else
			{


				memset(sRawResult,0,10);
				memset(sResult,0,10);

				strncpy((LPSTR)sRawResult,(LPSTR)(lpOpPointer+_gItemDataOff[i]) ,_gItemDataLen[i]);

				strncpy((LPSTR)sResult,(LPSTR)sRawResult,_gItemPointPos[i]-1);
				sResult[_gItemPointPos[i]-1]='.';
			if ((_gItemDataLen[i]-_gItemPointPos[i]+1) !=0)
				strncpy((LPSTR)(sResult+_gItemPointPos[i]),(LPSTR)(sRawResult+_gItemPointPos[i]-1),
						_gItemDataLen[i]-_gItemPointPos[i]+1);
				sResult[_gItemDataLen[i]+1]=0;
//				f_Result= atof(sResult);
//				memset(sResult,0,10);
//				sprintf(sResult,"%6f",f_Result);
				if (sResult[_gItemDataLen[i]]!='.')

					for(p_dndResult2=(LPSTR)sResult;*p_dndResult2=='0' &&
						*(p_dndResult2+1)!='.' ;p_dndResult2++);


					for(p_dndResult1=(LPSTR)(sResult+strlen(sResult)-1);*p_dndResult1=='0' ;p_dndResult1--);

					if (*p_dndResult1=='.' )	
						p_dndResult1--;	
				}
				else
				{

					for(p_dndResult2=(LPSTR)sResult;*p_dndResult2=='0' &&
						*(p_dndResult2+1)!='.' ;p_dndResult2++);

					p_dndResult1=(LPSTR)(sResult+strlen(sResult)-1);
					if (*p_dndResult1=='.' )	
						p_dndResult1--;	
				}
//填结果
				FillResult(nItemCount,p_dndResult2,(p_dndResult1-p_dndResult2+1))
//				strncpy(OutResult[nItemCount].AssayResult ,p_dndResult2,int (p_dndResult1-p_dndResult2+1));
//				OutResult[nItemCount].AssayResult[_gItemDataLen[i]+1] ='\0';

			}
//样本号  				
			FillSampleID(nItemCount, wSampleID)	
//项目名
			strcpy(OutResult[nItemCount].ItemID, LPSTR(_gItemID[i]));
				
 			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
			
			FillResultType(nItemCount,  btAssayType)	
			if(btAssayType ==c_TypeSample )
			{
			
//				FillComment(nItemCount ,lpOpPointer+_gItemDataOff[i]+c_ResultLen,1)
			}	
			nItemCount++;
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

