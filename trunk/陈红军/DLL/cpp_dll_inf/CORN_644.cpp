//********************************************************************
//  ģ��: CORN_644.cpp
//  ����: ��ģ��Ϊ����������˾��CIBA-CORNING���ӷ����ǵ���������(Win95��)

#include <stdio.h>
//#include <wchar.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	512		//���ջ�������
#define	c_MaxItemNum		3		//һ�δ������Ŀ��

#define c_SampleIDOff		0x0c
#define c_SampleIDLen		2

#define c_FunctionIDOff		0x15

#define c_ResultNaOff		0x28	//Na
#define c_ResultKOff		0x39	//K
#define c_ResultClOff		0x4c	//Cl

#define c_ResultLen			5		//



DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount,nRecordLen=0,ss=0;
	LPSTR lptemp1,ptemp2,ptemp3,ptemp4;
	BYTE	Achar;
	char *ptemp1;

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

		lptemp1=strchrnb(lpOpPointer,STX,1,10);
		if (lpOpPointer[0]!=STX) 
		{
			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memmove( lpOpPointer, lptemp1,dwRxBufferLength );
		} 


		ptemp1 =strchrnb(lpOpPointer,'S',1, 24);		
		if (ptemp1==NULL )
		{
			PurgeComm( hComm, PURGE_RXCLEAR );
			ResetRxPointer()		
			ResetOpPointer()
			continue;
		}
	
		
		GetLocalTime(&SystemTime);	
		nItemCount =0;

		wSampleID =StrToInt(ptemp1+c_SampleIDOff, c_SampleIDLen);

		FillSampleID(nItemCount, wSampleID)	//������
		FillItemName(nItemCount,"Na" , 2)	//��Ŀ��
		FillResult(nItemCount, ptemp1+c_ResultNaOff ,c_ResultLen ) 
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;
	
		FillSampleID(nItemCount, wSampleID)	//������
		FillItemName(nItemCount,"K " , 1)	//��Ŀ��
		FillResult(nItemCount, ptemp1+c_ResultKOff ,c_ResultLen ) 
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		FillSampleID(nItemCount, wSampleID)	//������
		FillItemName(nItemCount,"Cl" , 2)	//��Ŀ��
		FillResult(nItemCount, ptemp1+c_ResultClOff ,c_ResultLen ) 
 		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		nItemCount++;

		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}

		PurgeComm( hComm, PURGE_RXCLEAR );
		ResetRxPointer()		
		ResetOpPointer()
	}

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

