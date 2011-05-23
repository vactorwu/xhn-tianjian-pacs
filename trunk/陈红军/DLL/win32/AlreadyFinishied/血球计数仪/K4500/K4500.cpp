// K4500.cpp : Defines the entry point for the DLL application.
//
//********************************************************************
//  模块: k4500.cpp
//  描述: 本模块为东亚公司的Sysmex-k4500血球化验仪的联机程序(Win95版),数据格式采用K1000

#include "stdafx.h"
#include "const.h"
#include "device.h"
#include "str.h"
#include "logfile.h"
#include "devmacro.h"

#define c_RxBufferLength	512		
#define	c_MaxItemNum		20		
									
#define c_DataCodeOff		3
#define c_SampleIDOff		20
#define c_SampleIDLen		3


#define c_QCIDOff			15
#define c_QCIDLen			1

#define c_ItemNum			38		
#define c_SampleItemNum		18		
#define c_QCItemNum			20		
	 
extern HANDLE hLogFile;
 
static const int _gItemDataOff[c_ItemNum] =//项目数据偏移
	{30,35,40,45,50,55,60,65,70,75,
	 80,85,90,95,100,105,110,115};
	
 
static const int _gItemDataLen[c_ItemNum] =//项目数据长度,不包括FLAG字符.
	{4, 4, 4, 4, 4, 4, 4, 4, 
	 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

 
static const CHAR * _gItemID[c_ItemNum] =//项目ID
	{"WBC","RBC","HGB","HCT","MCV","MCH","MCHC","PLT",
	 "W-SCR","W-MCR","W-LCR","W-SCC","W-MCC","W-LCC",
	"RDW-CV","PDW","MPV","P-LCR"};	 
 

 
static  const int _gItemPointPos[c_ItemNum] =//小数点位置,因为数据长度均为4,小数点位置在5的项目为无小数.
	{4, 3, 5, 2, 4, 4, 5, 5, 
	 2, 2, 2, 4, 4, 4, 2, 4, 4, 2 };

 
 
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,jjj=0, nMaxItemNum;
	BYTE btAssayType,SampleIDLen;
	char	*p_dndResult1,*p_dndResult2;
	double  f_Result;
	bool bLogFile;
	char sRawResult[10],sResult[10];	
	char TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
    char SampleID[5];
	LPSTR  lptemp1;
	Init()
	bLogFile=NewLogFile("K4500.txt");
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
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}
 		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		//上面函数用来定位包起始位在接收缓冲区中的位置.???
		if (lpOpPointer[0]!=STX) 
		{	
			if((lptemp1!=NULL) &( lptemp1[0]==STX) )
			{
				dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
				memmove( lpOpPointer, lptemp1,dwRxBufferLength );			
			}
		}
		if(lpOpPointer[0]==STX)
		{
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
			{	//指控数据不处理。
				ResetOpPointer()
				ResetRxPointer()		 
				ClearCommRx()		 
				WriteCommChar(ACK)
				continue;
			}
			TempWorkingID[0]=lpDevice->DevItemType;
			_itoa(wSampleID,SampleID,10);
			SampleIDLen=strlen(SampleID);
			memset(&TempWorkingID[1],0x30,5);
			strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
			TempWorkingID[6]='\0';						
			for (;i<nMaxItemNum; i++)//18项结果
			{
				if (*(lpOpPointer+_gItemDataOff[i])=='*')	 
				{
 
					FillErrorCode(nItemCount,c_UnReliable )
 
					FillResult(nItemCount,"     " ,5)
					OutResult[nItemCount].ItemNo=nItemCount+1;
				}
				else
				{
 
 
					memset(sRawResult,0,10);
					strncpy( sRawResult,lpOpPointer+_gItemDataOff[i] ,_gItemDataLen[i]);
 
					strncpy( sResult,sRawResult,_gItemPointPos[i]-1); 
					sResult[_gItemPointPos[i]-1]='.'; 
					strncpy((LPSTR)sResult+_gItemPointPos[i],(LPSTR)sRawResult+_gItemPointPos[i]-1,
							_gItemDataLen[i]-_gItemPointPos[i]+1); 
					sResult[_gItemDataLen[i]+1]=0; 
 					f_Result= atof(sResult);
 					memset(sResult,0,10);
 					sprintf(sResult,"%6f",f_Result);
					if (sResult[_gItemDataLen[i]]!='.') 
					{
 
						for(p_dndResult2=sResult;*p_dndResult2=='0' &&
							*(p_dndResult2+1)!='.' ;p_dndResult2++);

 
						for(p_dndResult1=sResult+strlen(sResult)-1;*p_dndResult1=='0' ;p_dndResult1--);
 
						if (*p_dndResult1=='.' )	
							p_dndResult1--;	
					}
					else 
					{
 
						for(p_dndResult2=sResult;*p_dndResult2=='0' &&
							*(p_dndResult2+1)!='.' ;p_dndResult2++);
 
						p_dndResult1=sResult+strlen(sResult)-1;
						if (*p_dndResult1=='.' )	
							p_dndResult1--;	
					}
 
					strncpy(OutResult[nItemCount].AssayResult ,p_dndResult2,p_dndResult1-p_dndResult2+1);
			
					OutResult[nItemCount].AssayResult[int (p_dndResult1-p_dndResult2+1)] ='\0';
					OutResult[nItemCount].ItemNo=nItemCount+1;

				}
 					
				FillSampleID(nItemCount, wSampleID)	
 
				strcpy(OutResult[nItemCount].ItemID, LPSTR(_gItemID[i]));
				
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
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
		else
		{
			
			ResetOpPointer()
			ResetRxPointer()		 
			ClearCommRx()		 
			WriteCommChar(NAK)
		}
	}
	CloseLogFile(hLogFile);
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

