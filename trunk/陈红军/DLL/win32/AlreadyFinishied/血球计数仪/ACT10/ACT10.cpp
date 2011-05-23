// ACT10.cpp : Defines the entry point for the DLL application.
//��������ع�˾��ACT10Ѫ�������

#include "stdafx.h"
#include "str.h"
#include "logfile.h"
#include "device.h"
#include "devmacro.h"
#include "const.h"
#define c_RxBufferLength   256
#define c_MaxItemNum        17
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
	char  DataBuffer[200];// ���ݻ�������
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
 	SYSTEMTIME SystemTime;
	bool bLogFile=FALSE;
	int	 nItemCount;
	int  i;
	int  BufferStart ;
	int  ByteRecieved ;   //���յ�����Ч�ַ���
	int  DelimiterStart ; //�ֽ��λ��
	int  DelimiterEnd ;   //�·ֽ��λ��
	int SearchRange;      //�ڻ������в��ҷֽ����������Χ
	bool isdataPacket=FALSE;
	CommData
	Init()
	ResetOpPointer()
	FillDevName()
	if (NewLogFile("ACT10.txt"))
		bLogFile = TRUE;
	SetCommWaitMask(EV_RXCHAR ) 
	while (TRUE)
	{						
		do{					
			dwReadLength=0;
			i=0;
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
					break;
				if(((RxChar==ETX)|(RxChar==ETB))&(isdataPacket))
				{
					//DataBuffer[i]=RxChar;
					ByteRecieved=i;
					break;
				}					
			}while(dwReadLength==1);
			if(bLogFile)
				WriteLogData(hLogFile,&DataBuffer[0],ByteRecieved);
			switch (RxChar) 
			{
			case ENQ:
				Sleep(300);
				TransmitCommChar(hComm,ACK);
				break;
			case ETB:
			case ETX:
				//�Ƚ���У��ļ��㣬��Ϊ������������н������
				//Ŀǰ����������У��͵ļ��㷽�������������ݲ�������δ��У�����
				/*
			    	��Ҫ�������϶������ݰ����з���
				*/
				switch(DataBuffer[2])
				{
				case 'H':
					break;
				case 'P':
					break;
				case 'O':
				//	wSampleID=StrToInt(&DataBuffer[6],6);
					TempWorkingID[0]=lpDevice->DevItemType;
					strncpy(&TempWorkingID[1],&DataBuffer[7],5);
					TempWorkingID[6]='\0';
				//	itoa(wSampleID,&TempWorkingID[1],10);
				//	GetLocalTime(&SystemTime);
					nItemCount=0;
					break;
				case 'R':
					BufferStart=0;
					SearchRange=ByteRecieved;
					//��λ������
					DelimiterStart = FindCharInStr(&DataBuffer[BufferStart],'|',SearchRange,1);
					if(DelimiterStart != -1)
					{
						SearchRange=SearchRange-(DelimiterStart+1);
						BufferStart=BufferStart+DelimiterStart+1;
					}
					else
					{
						TransmitCommChar(hComm,NAK);
						continue;
					}
					OutResult[nItemCount].ItemNo=atoi(&DataBuffer[DelimiterStart+1]);
					//��λ��Ŀ����
					DelimiterStart = FindCharInStr(&DataBuffer[BufferStart],'|',SearchRange,1);
					if(DelimiterStart != -1)
					{
						SearchRange=SearchRange-(DelimiterStart+1);
						BufferStart=BufferStart+DelimiterStart+1;
					}
					else
					{
						TransmitCommChar(hComm,NAK);
						continue;
					}
					DelimiterEnd = FindCharInStr(&DataBuffer[BufferStart],'|',SearchRange,1);
			
					FillItemName(nItemCount, &DataBuffer[BufferStart+3],DelimiterEnd-3 )

					//��λ���ֵ
					BufferStart=BufferStart+DelimiterEnd;

					DelimiterEnd = FindCharInStr(&DataBuffer[BufferStart+1],'|',SearchRange,1);
					if(DelimiterStart != -1)
					{
						SearchRange=SearchRange-(DelimiterStart+1);
						BufferStart=BufferStart+DelimiterStart+1;
					}
					else
					{
						TransmitCommChar(hComm,NAK);
						continue;
					}
				
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
					FillDate(nItemCount, SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					strncpy(OutResult[nItemCount].AssayResult, &DataBuffer[BufferStart-2],DelimiterEnd +1);	
					OutResult[nItemCount].AssayResult[DelimiterEnd] ='\0';
					ReplaceAllCharInStr(OutResult[nItemCount].AssayResult,'!',' ',DelimiterEnd+1);
					ReplaceAllCharInStr(OutResult[nItemCount].AssayResult,'|',' ',DelimiterEnd+1);
					ReplaceAllCharInStr(OutResult[nItemCount].AssayResult,'*',' ',DelimiterEnd+1);
					ReplaceAllCharInStr(OutResult[nItemCount].AssayResult,'L',' ',DelimiterEnd+1);
					ReplaceAllCharInStr(OutResult[nItemCount].AssayResult,'H',' ',DelimiterEnd+1);
					nItemCount++;

					break;
				case 'L':
					if(nItemCount>0)
					{
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}
					break;
				}
				Sleep(300);
				TransmitCommChar(hComm,ACK);
				break;
			case EOT:
				//��������
			default:
				TransmitCommChar(hComm,ACK);
				break;
			}
			isdataPacket=FALSE;
		} while(TRUE);
		
	}
	return TRUE;
}
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar=ETX;
}