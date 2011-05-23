// Syn_ELISE.cpp : Defines the entry point for the DLL application.
//������������˾��Synchron EL_ISE ELECTROLYTE  System ��̬���ӿ�

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"
#define c_RxBufferLength	800		//���ݴ���������
#define	c_MaxItemNum		8		//һ����ദ�����Ŀ��

#define c_SequenceNoOff     8
#define c_SequenceNoLen     9

#define	c_TrayNumEOff		18		
#define	c_CupNumEOff		21		//�����ڱ�β���ڵ�ƫ��
#define	c_TrayNumLen		2		//�̺ų���
#define	c_CupNumLen			2		//���ų���
#define c_RecordTypeOff     2       //��¼����ƫ��
#define c_FuncNumOff		9		//�����ڶ��ֽ�ƫ��
#define	c_ItemNameOff		11		//�Ǽ�����Ŀ��ƫ��
#define	c_ItemNameLen		3		
#define	c_StreamIDOff		4	    //�����ƫ��
#define	c_StreamIDLen		3	
#define	c_CalcItemNameOff	27		//������Ŀ����ƫ��
#define	c_CalcItemNameLen	1		
#define	c_SampleNoOff		11		
#define	c_SampleNoEOff		25		//�������ڱ�β����ƫ��
#define	c_ResultOff			15		//�Ǽ�����ƫ��
#define	c_ResultLen			9		//�������
#define	c_CalcResultOff		17		//������ƫ��
#define	c_TrayNumHOff		11		//�̺��ڱ�ͷ���ڵ�ƫ��
#define	c_CupNumHOff		14		
#define	c_TrayNumEOff		18		
#define	c_CupNumEOff		21		//�����ڱ�β���ڵ�ƫ��
#define	c_TrayNumLen		2		//�̺ų���
#define	c_CupNumLen			2		//���ų���
#define	c_CupHeaderPackLen	71		//��ͷ����	
#define	c_TestResultPackLen 89		//�������
#define c_SPCalcuPackLen	33		//�������
#define	c_EndCupPackLen		41		//����������
#define	c_EndTrayPackLen	15		//�̽�������
#define	c_CupsOfOneTray		40		

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
	SYSTEMTIME SystemTime;
	int	 nItemCount,TrayNum,CupNum;//������Ŀ��,�̺�,����
	BYTE  SampleIDLen ;//���ݰ����ܺ�,����ID����
	WORD	wSampleNo;
	bool blogfile;
	int  i,ByteRecieved;
	bool isdataPacket=FALSE;
	char  DataBuffer[200];// ���ݻ�������
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
	char LSampleID[5];
	Init()
	
	ResetOpPointer()				
	
	ResetRxPointer()																																																																																																															
	

 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXCHAR)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	blogfile=NewLogFile("Syn_ELISE.txt");
	WriteCommChar(XON)
	while (TRUE)
	{
		WaitEvent(EV_RXCHAR)		
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar==STX)
				isdataPacket=TRUE;
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;
			}
			if(RxChar==ENQ||RxChar==EOT)
			{
				break;
			}
			if(((RxChar==ETX)|(RxChar==ETB))&(isdataPacket))
			{
				//DataBuffer[i]=RxChar;
				ByteRecieved=i;
				break;
			}	
		}while(dwReadLength==1);
		i=0;
		if((RxChar==ETX))
		{
			while(DataBuffer[i]!=STX)	i++;
		}
		lpOpPointer=&DataBuffer[i];
		if (blogfile)
		{
			WriteLogData(hLogFile,lpOpPointer,ByteRecieved);
		}

		switch (RxChar) 
		{
		case ENQ:
		//	Sleep(300);
			TransmitCommChar(hComm,ACK);
			break;
		case ETB:
		case ETX:
			//�Ƚ���У��ļ��㣬��Ϊ������������н������
			//Ŀǰ����������У��͵ļ��㷽�������������ݲ�������δ��У�����
			/*
			    ��Ҫ�������϶������ݰ����з���
			*/
			switch(*(lpOpPointer+2))
			{
			case 'H':
				break;
			case 'P':
				break;
			case 'O':
				nItemCount=0;
				TrayNum=(int)StrToInt(lpOpPointer+c_TrayNumEOff,c_TrayNumLen);				
				CupNum=(int)StrToInt(lpOpPointer+c_CupNumEOff,c_CupNumLen);			
				wSampleNo =(int)StrToInt(lpOpPointer+c_SequenceNoOff,c_SequenceNoLen);
				_itoa(wSampleNo,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				memset(&TempWorkingID[1],0x30,5);
				SampleIDLen=strlen(LSampleID);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';
						
				break;	//��������	
			case 'R':
				FillSampleID(nItemCount, wSampleNo)	
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);//��д��������
				OutResult[nItemCount].ItemNo=nItemCount+1;
				FillItemName(nItemCount,lpOpPointer+c_ItemNameOff ,c_ItemNameLen)	

				if (OutResult[nItemCount].ItemID[1]==124)//'|'  �����Ŀ���Ƴ���Ϊ1������ַ���β��־-ASCII ��ֵΪ 0
				{
					OutResult[nItemCount].ItemID[1]='\0';			
					FillResult(nItemCount,lpOpPointer+c_ResultOff-2,c_ResultLen)
				}
				else
					FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)

				OutResult[nItemCount].ItemNo=nItemCount+1;
//������
				GetSystemTime(&SystemTime); 
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				nItemCount++;
				(*lpResultProcessRoutine)(lpDevice->nDevNO, &OutResult[nItemCount-1], 1);
				lpDevice->dwRecordCount+=1;  
				break;
			case 'L':
				break;
			}
		//	Sleep(300);
			TransmitCommChar(hComm,ACK);
			break;
		case EOT:
			//��������
		default:
			TransmitCommChar(hComm,ACK);
			break;
		}
		PurgeComm( hComm, PURGE_RXCLEAR );	
	  
	}
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
	//�趨���ڼ���¼�
}

