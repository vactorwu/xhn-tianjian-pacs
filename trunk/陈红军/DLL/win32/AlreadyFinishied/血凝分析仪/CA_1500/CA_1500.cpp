// CA_1500.cpp : Defines the entry point for the DLL application.
//������Ϊ�ձ�����ҽ�õ�����ʽ����� Ѫ��������CA-1500�����ݲɼ���������

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"

#define c_RxBufferLength	1024 //���建������С		
#define	c_MaxItemNum		80  //������Ŀ��
#define c_RackNumOff        20
#define c_RackNumLen        6
#define c_TubePosNumOff     26  //CA1000 Ϊ 24
#define c_TubePosNumLen     2
#define c_SampleIDoff       28  //CA1000 Ϊ 26
#define c_SampleLen         15  //CA1000 Ϊ 13
#define c_DataOff           59  //CA1000 Ϊ 51
#define c_OneDataLen        9   
#define c_ItemIDOff         0
#define c_ItemIDLen         3
#define c_ResultOff         3
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
	int nItemCount,SampleIDLen,nDataOff;
//	int TubeNum,RackNum;
	bool bLogFile=FALSE;
	int itemNum;
	char PacketType;
	SYSTEMTIME SystemTime;
	char TempWorkingID[7];
	char LSampleID[5];//ָ��������(�ַ���)��ָ��
	CommData
	Init()
	ResetOpPointer()				
	FillDevName()
	if(NewLogFile("CA_1500.txt"))
	{
		bLogFile=TRUE;
	}
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );

	while (TRUE)
	{
		WaitEvent(EV_RXFLAG)//�ȴ��������ݰ�
			
		GetInQueueLength(dwReadLength)//��������
		ReadCommBlock(dwReadLength)
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwReadLength);
		}

		PacketType=*(lpOpPointer+1);
		switch(PacketType)
		{
			case 'R'://Order inquiry
				break;
			case 'D'://Analysis Data
				nItemCount=0;
				GetLocalTime(&SystemTime);
				wSampleID=StrToInt(lpOpPointer+c_SampleIDoff, c_SampleLen);
				_itoa(wSampleID,LSampleID,10);		
				TempWorkingID[0]=lpDevice->DevItemType;
				SampleIDLen=strlen(LSampleID);
			 	memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],LSampleID,SampleIDLen);
				TempWorkingID[6]='\0';	
				nDataOff =c_DataOff;
				for(; *(lpOpPointer+nDataOff)!=ETX  ;)
				{
					itemNum=StrToInt(lpOpPointer+nDataOff,2);//������Ŀ��
					strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
					OutResult[nItemCount].ItemNo=nItemCount+1;//��Ŀ��
					FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)
					// ������Ŀ��ֵ��ͬ���������ʽ��ͬ��Time,
					switch(*(lpOpPointer+nDataOff+2))
					{
					case  '1':
						strncpy(OutResult[nItemCount].AssayResult,lpOpPointer+nDataOff+c_ResultOff,4);
						OutResult[nItemCount].AssayResult[4]='.';
						OutResult[nItemCount].AssayResult[5]=*(lpOpPointer+nDataOff+c_ResultOff+4);
						OutResult[nItemCount].AssayResult[6]='\0';
						break;
					case '2':
						if(itemNum!=6)
						{
							strncpy(OutResult[nItemCount].AssayResult,lpOpPointer+nDataOff+c_ResultOff+1,3);
							OutResult[nItemCount].AssayResult[3]='.';
							OutResult[nItemCount].AssayResult[4]=*(lpOpPointer+nDataOff+c_ResultOff+4);
							OutResult[nItemCount].AssayResult[5]='\0';
						}
						else
						{
							//����жϽ���ĵ�λ������λ��ͬ�������ͬ��
							strncpy(OutResult[nItemCount].AssayResult,lpOpPointer+nDataOff+c_ResultOff+1,2);
							OutResult[nItemCount].AssayResult[2]='.';
							OutResult[nItemCount].AssayResult[3]=*(lpOpPointer+nDataOff+c_ResultOff+3);
							OutResult[nItemCount].AssayResult[4]=*(lpOpPointer+nDataOff+c_ResultOff+4);
							OutResult[nItemCount].AssayResult[5]='\0';
						}
						break;
					case '3':
						strncpy(OutResult[nItemCount].AssayResult,lpOpPointer+nDataOff+c_ResultOff+1,2);
						OutResult[nItemCount].AssayResult[2]='.';
						OutResult[nItemCount].AssayResult[3]=*(lpOpPointer+nDataOff+c_ResultOff+3);
						OutResult[nItemCount].AssayResult[4]=*(lpOpPointer+nDataOff+c_ResultOff+4);
						OutResult[nItemCount].AssayResult[5]='\0';
						break;
					case '4':
						strncpy(OutResult[nItemCount].AssayResult,lpOpPointer+nDataOff+c_ResultOff+1,2);
						OutResult[nItemCount].AssayResult[2]='.';
						OutResult[nItemCount].AssayResult[3]=*(lpOpPointer+nDataOff+c_ResultOff+3);
						OutResult[nItemCount].AssayResult[4]=*(lpOpPointer+nDataOff+c_ResultOff+4);
						OutResult[nItemCount].AssayResult[5]='\0';
						break;
					case '5':
					case '6':
					case '7':
						nItemCount--;
						break;
						}
					nDataOff =c_OneDataLen+nDataOff;
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nItemCount++;
				}
				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				
				break;
		}
		WriteCommChar(ACK)
		//�������
		nItemCount=0;	
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