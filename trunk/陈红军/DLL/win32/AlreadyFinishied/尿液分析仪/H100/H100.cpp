// H100.cpp : Defines the entry point for the DLL application.
//��������˾��Һ������H100

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "LogFile.h"
#include "str.h"
#define c_RxBufferLength	1024 //���建������С		
#define	c_MaxItemNum		18  //������Ŀ��
#define Working_idOff       7
#define ResultOff           7
#define resultLength        8
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
	CommData
	DWORD   i,j;
    int * FSPos;//����е���ָ�����ַ����е�λ��
	int nItemCount,nCheckSum,CK1,CK2,RxCheckSum,iSampleID;//У��ͼ���
	int SumOfResult,SampleIDLen;//�����������еĽ����
	SYSTEMTIME SystemTime;
	bool bLogFile;
	char TempWorkingID[7];
	char LSampleID[10];//ָ��������(�ַ���)��ָ��
	Init()
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	bLogFile=NewLogFile("H100.txt");	
	while (TRUE)
	{


		WaitEvent(EV_RXFLAG)//�ȴ��������ݰ�
		GetInQueueLength(dwReadLength)//��������
		ReadCommBlock(dwReadLength)
		if (dwReadLength<=10)
			continue;
		//����У��
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		j=0;
		for(i=0;i<dwReadLength;i++)
		{
			if (*(lpOpPointer+i)==0x0d)
				j++;
		}
		FSPos=new int[j];
		j=0;
		for(i=0;i<dwReadLength;i++)
		{
			if (*(lpOpPointer+i)==0x0d)
			{
				FSPos[j]=i;
				j++;
			}
		}
		GetLocalTime(&SystemTime);
		iSampleID=StrToInt(lpOpPointer+FSPos[0]+Working_idOff,3);
		_itoa(iSampleID,LSampleID,10);
		TempWorkingID[0]=lpDevice->DevItemType;
		SampleIDLen=strlen(LSampleID);
		memset(&TempWorkingID[1],0x30,5);
		if(SampleIDLen>=6)
		{
			strncpy(&TempWorkingID[1],&LSampleID[SampleIDLen-5],5);
		}
		else
			strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
		TempWorkingID[6]='\0';
		nItemCount=0;
		for(j=1;j<12;j++)
		{
			//�������		
                FillResult(nItemCount,lpOpPointer+FSPos[j] + ResultOff, resultLength)

				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

				OutResult[nItemCount].ItemNo=nItemCount+1;

				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
				nItemCount++;

		}
		for(j=0;j<11;j++)
		{
			OutResult[j].ItemID[0]=0x41+j;
			OutResult[j].ItemID[1]='\0';				
		}

		//������
		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;	
		ResetRxPointer()		
		//��ջ�����
		delete FSPos;

	}
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =0x0c;
	//�趨���ڼ���¼�
}