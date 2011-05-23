//********************************************************************
//  模块: AVL988_3.cpp
//  描述: 本模块为瑞士公司的离子分析仪的联机程序(Win95版)
//	函数:
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	800	  
#define	c_MaxItemNum		3		 
#define	c_PackTypeOff		0x37		
#define c_SampleIDOff		0x6f		
#define c_SampleIDLen		4			

#define c_K_ResultOff			0x93		
#define	c_Na_ResultOff			0x82		
#define	c_Cl_ResultOff			0xa4		

#define c_ResultLen				6		




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nRecordLen=0,ss=0;
	LPSTR lptemp1;
	BYTE	Achar;

	Init()
	nItemCount =0;
	ResetOpPointer()				
	ResetRxPointer()
	ClearCommRx()		
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)

	WriteCommChar(ACK)

	while (TRUE)
	{


		do
		{
			ResetOpPointer()				
			ResetRxPointer()
			GetInQueueLength(dwReadLength)
			if (dwReadLength == 0 )
			{
				WriteCommChar(ACK)
				continue;
			}
			else
			{
				if ( dwReadLength >1 )
				{
					WriteCommChar(ACK)
					break;//pack
				}
				else//char
				{
					ReadCommBlock(dwReadLength)
					if (*(lpOpPointer)==0x07 )
					{
						WriteCommChar(ACK)
						continue;
					}
					else
					{
						Sleep(1);
					}
				}
			}
		}while (TRUE);

		
		
		
		WaitEvent(EV_RXFLAG)

		GetInQueueLength(dwReadLength)
 
		ReadCommBlock(dwReadLength)

		*(lpOpPointer+dwReadLength)='\0';

		lptemp1=strchrnb(lpOpPointer,0x07,1,10);
		if (lpOpPointer[0]!=0x07) 
		{
			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memmove( lpOpPointer, lptemp1,dwRxBufferLength );
		} 
	
		GetLocalTime(&SystemTime);
			
		wSampleID =(int)StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
		nItemCount =0;	

		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"K" ,1)
		FillResult(nItemCount,lpOpPointer+c_K_ResultOff,c_ResultLen )
			
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"Na" ,2)
		FillResult(nItemCount,lpOpPointer+c_Na_ResultOff,c_ResultLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;
		FillSampleID(nItemCount, wSampleID)	
		FillItemName(nItemCount,"Cl" ,2)
		FillResult(nItemCount,lpOpPointer+c_Cl_ResultOff,c_ResultLen )
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		

				

		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		ResetRxPointer()		
		ResetOpPointer()
		WriteCommChar(ACK)
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =EOT;
}

