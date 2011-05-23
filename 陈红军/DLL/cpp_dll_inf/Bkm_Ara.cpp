//********************************************************************
//  本模块为Beckman公司的Array化验仪(血清免疫蛋白)的联机程序(Win95版)
//  描述: 全称：Array Protein System （360/360E） 
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	1024	
#define	c_MaxItemNum		10		
#define c_FuncNumOff		2		




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,nItemCount,nPackLen=0;
	BYTE  PackFun ;
	BOOL	OddFlag=FALSE;
	LPSTR lpPointer,lptemp1,lptemp2,hRXBuffer=(LPSTR)RxBuffer;
	char *p_ACK1="\x10\x31";
	char *p_ACK0="\x10\x30";

	Init()
	ResetOpPointer()				

	
	ResetRxPointer()


 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	while (TRUE)
	{



		WaitEvent(EV_RXFLAG)


		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 

 

		lptemp1=strbchrnb(lpRxPointer,'[',1,(int)dwReadLength+5);
		PackFun= *(lptemp1+c_FuncNumOff);
		{
			case '0':	
			case 'D':
				ResetRxPointer()		
				ResetOpPointer()
				continue;
			case '1':	
				if (lptemp1!=lpOpPointer)	
				{
					for (i=0;*hRXBuffer!='[';hRXBuffer++,i++); 
					memmove(lpOpPointer ,hRXBuffer , (int)(lpRxPointer-hRXBuffer));
					lpRxPointer=lpRxPointer-i;
					hRXBuffer=(LPSTR)RxBuffer;
				}
				continue;
			case '5':	

				if ((*lpOpPointer)!='[')
				{
					for (i=0;*hRXBuffer!='[';hRXBuffer++,i++); 
					memmove(lpOpPointer ,hRXBuffer , (int)(lpRxPointer-hRXBuffer));
					lpRxPointer=lpRxPointer-i;
					hRXBuffer=(LPSTR)RxBuffer;
				continue;
			case '9':	
				
				break;			

			default :	

				continue;

		}


		wSampleID =(int)StrToInt(lpOpPointer+7,5);
		GetLocalTime(&SystemTime);
				

		lpPointer=strchrnb(lpOpPointer+1,'[',2,70);				
		while(*(lpPointer+2)=='2')	
		{

			FillSampleID(nItemCount, wSampleID)	

			lptemp1=strchrnb(lpPointer,',',1,4)+1;
			lptemp2=strchrnb(lptemp1,',',1,5);
			FillItemName(nItemCount, lptemp1,(lptemp2-lptemp1))	

			lptemp1=strchrnb(lptemp2+1,',',2,20)+1;
			for (;*lptemp1==' ';lptemp1++);
			lptemp2=strchrnb(lptemp1,',',1,10);
			FillResult(nItemCount,lptemp1,(int)(lptemp2-lptemp1))

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour)

			lpPointer=strchrnb(lptemp2+1,'[',1,40);				
			nItemCount++;
		}	


		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		
		nItemCount=0;	
//		nPackLen=0;
		ResetRxPointer()		
		ResetOpPointer()
		hRXBuffer=(LPSTR)RxBuffer;

	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar ='[';
}
