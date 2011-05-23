// AEROSET.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"
#define c_RxBufferLength	1024 //���建������С		
#define	c_MaxItemNum		100  //������Ŀ��
#define c_SampleIDOff       7
#define c_SamlpeIDLen       20
#define c_DataOff           48
#define c_OneDataLen        13
#define c_ResultOff         4
#define c_ResultLen         6
extern HANDLE hLogFile;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DWORD WINAPI CommMonitor( LPSTR lpData)
{
    //��ش��ڽ������ݣ���������ݸ�ʽ���������������������
	int		nItemCount,SampleIDLen,nDataOff,nResultLen,iSampleID;
	bool	bLogFile;
	char    TempWorkingID[7];
	LPSTR   lpPointer;
	//Ӧ���
	char * p_Ack="\x2\x6\x3";
	char * p_Nak="\x2\x16\x3";
	char * p_IntSessionTXT="\x2I\x20\x3";
	char * p_ReleaseMaster="\x2M\x20\x20\x20\x20\x20\x3";
	char  PackageType;
	char SampleID[10];
	SYSTEMTIME SystemTime;
	CommData
    //ͨ������1'��1''��2��3��4��5
	
	Init()
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	if(NewLogFile("AEROSET.txt"))
	{
		bLogFile=TRUE;
	}

	while (TRUE)
	{


		WaitEvent(EV_RXFLAG)//�ȴ��������ݰ�
		GetInQueueLength(dwReadLength)//��������
		ReadCommBlock(dwReadLength)
		PackageType=*(lpOpPointer+1);//���ݰ�������
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}
		//У����յ������ݣ�����ȷ�������������Ӧ��
		/*
			checksum  calculation
		*/

		/*��������ݿ��п��ܰ���������Ϣ��ID��Ϣ��������ݵ��顣*/
		PackageType=*(lpOpPointer+1);
		switch(PackageType)
		{
		case 'Q':
			WriteCommBlock(p_Ack,3)//�϶�Ӧ��
			//WriteCommChar(ACK)
			break;

		case 'R':
			//�������
			WriteCommBlock(p_Ack,3)//�϶�Ӧ��
			iSampleID=StrToInt(lpOpPointer+c_SampleIDOff,c_SamlpeIDLen);
			itoa(iSampleID,&SampleID[0],10);
			SampleIDLen=strlen(SampleID);
			TempWorkingID[0]=lpDevice->DevItemType;
			if(SampleIDLen>=6)
			{
				strncpy(&TempWorkingID[1],&SampleID[SampleIDLen-5],5);
			}
			else
				strncpy(&TempWorkingID[6-SampleIDLen],SampleID,SampleIDLen);
			TempWorkingID[6]='\0';
			GetLocalTime(&SystemTime);
			nItemCount=0;
			for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETB ;)
			{
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
				OutResult[nItemCount].ItemNo=nItemCount+1;
				lpPointer=lpOpPointer+nDataOff;
				DeleSpace(4)
				strncpy(OutResult[nItemCount].ItemID,lpPointer,nResultLen);
				OutResult[nItemCount].ItemID[nResultLen]='\0';
				lpPointer=lpOpPointer+nDataOff+c_ResultOff;
				DeleSpace(c_ResultLen )
				FillResult(nItemCount, lpPointer ,nResultLen ) 
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nDataOff = nDataOff + c_OneDataLen;
				nItemCount++;
			}
			if(nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
		
			//WriteCommChar(ACK)
		///	WriteCommBlock(p_Ack,3)//�϶�Ӧ��
			break;
		case ACK:
		case NAK:
			break;
       
		}
	

		ResetRxPointer()		
	

	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
	//�趨���ڼ���¼�
}