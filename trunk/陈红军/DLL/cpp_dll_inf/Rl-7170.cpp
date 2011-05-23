//********************************************************************
//  模块: rl-7170.cpp
//  描述: 本模块为日立公司的7170化验仪的联机程序(Win95版)
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
#include <profile.h>
  

#define c_RxBufferLength        512	    		
#define c_MaxItemNum            46             
#define c_PackTypeOff 			1               
#define c_PackTypeLen           1               
#define c_SampleIDOff         	4               
#define c_SampleIDLen           5               
#define c_ItemNumOff         	41              
#define c_ItemNumLen           	3               
#define c_FirstItemIDOff        44              
#define c_ItemIDLen             3		        
#define c_FirstResultOff        47              
#define c_ResultLen             6		        
#define c_OneItemLen            10              
#define c_AssayYearOff          35              
#define c_AssayMonthOff         31              
#define c_AssayDayOff           33              
#define c_AssayHourOff          37              

#define c_ErrorCodeOff		9		
#define c_ErrorCodeLen		1		
#define c_ResultOff			3	
#define c_PacketFuncOff		1		
#define c_TransModeAndSampleTypeOff		3		




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount ,nResultLen;
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	LPSTR lpPointer;
	char PackFR[2];
	const char *p_MOR  ="\x2>\x3=";		
	const char *p_REP  ="\x2?\x3<";		


	Init()
	FillDevName()                       
	SetCommWaitMask(EV_RXFLAG)
	ResetOpPointer()
	lpRxPointer =(LPSTR)RxBuffer;		
		

	while (TRUE)
	{

		ResetRxPointer()		
		do
		{
			GetInQueueLength(dwReadLength)
			if(dwReadLength==0 &&dwRxBufferLength==0)
			{
				WaitEvent(EV_RXFLAG)  
				GetInQueueLength(dwReadLength)
			}
			ReadCommBlock(dwReadLength)	
			if ( dwRxBufferLength>=2)
			{
				if( (*(lpRxPointer-1)== ETX) )//|| (*(lpRxPointer-2)==ETB) )
				break;
			}
				
		}while(TRUE);				

		*(lpOpPointer+dwRxBufferLength)='\0'; 

		

		PackFR[0]=*(lpOpPointer+c_PacketFuncOff);
		PackFR[1]=0;

		switch (PackFR[0])	
		{
		case	'1':	
		case	'2':
		case	':':

		GetLocalTime(&SystemTime);
		wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		nItemCount =StrToInt(lpOpPointer+c_ItemNumOff, c_ItemNumLen);
		btResultType=*(lpOpPointer+ c_TransModeAndSampleTypeOff);
		if (btResultType=='A' || btResultType=='a' || btResultType=='B' ||
			btResultType=='b' || btResultType=='S' || btResultType=='s' ||
			btResultType=='T' || btResultType=='t' )
			btResultType =c_TypeSample;
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
			btResultType =c_TypeStat ;
		if (btResultType=='G' || btResultType=='g' || 
			btResultType=='H' || btResultType=='h')
			btResultType =c_TypeQC ;
		for(i =0; i<nItemCount; i++)
		{
			
			FillSampleID(i, wSampleID)     

			FillItemName(i, lpOpPointer+i*c_OneItemLen+c_FirstItemIDOff, c_ItemIDLen) //项目号

			lpPointer=lpOpPointer+ i*c_OneItemLen+c_FirstItemIDOff+c_ResultOff;
			DeleSpace(c_ResultLen )

			FillResult(i, lpPointer ,nResultLen )
		 
			FillDate(i, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

			FillResultType( i, btResultType)

 			FillComment( i, lpOpPointer+ i*c_OneItemLen+c_FirstItemIDOff+c_ErrorCodeOff, c_ErrorCodeLen)	
					 
 			FillErrorCode( i, c_Reliable)			
		}
	
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}


		PurgeComm( hComm, PURGE_RXCLEAR );	
		WriteCommBlock(p_MOR ,strlen(p_MOR))	
	
		break;
		default:		

			PurgeComm( hComm, PURGE_RXCLEAR );
			WriteCommBlock(p_MOR ,strlen(p_MOR))
		}

		 
 
	}
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

