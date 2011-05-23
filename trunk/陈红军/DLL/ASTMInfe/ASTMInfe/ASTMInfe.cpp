//-----------------------------------------------------------------------//
//   ASTMInfe.cpp : Defines the entry point for the DLL application.     //
//																		 //
//   ������				ʱ	��				��	ע                       //
//  --------          -----------          --------                      //
//   ��  ��            2001-02-08           ��	��                       //
//																		 //
//	 ˵������������ASTM E1394-91��ASTM E1381-94��׼�Ľӿ���������        //
//                                                                       //
//-----------------------------------------------------------------------//

#include	<stdio.h>
#include	<stdlib.h>
#include	"stdafx.h"
#include	"Const.h"
#include	"Device.h"
#include	"DevMacro.h"
#include	"str.h"
#include	"LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�����ⲿ�������� LogFile.cpp
extern		HANDLE  hLogFile;

//�Զ����ȫ�ֱ���(�ָ�������)
char        Field_delimiter;
char        component_delimiter;
char        repeat_delimiter;
char        escape_character;

#define		c_RxBufferLength	512	
#define		c_MaxItemNum		60

//--------------------------------����ں���-----------------------------------//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//-----------------------------------------------------------------------------//

//-------------------------------���ڼ�غ���----------------------------------//
DWORD WINAPI CommMonitor( LPSTR lpData)
{
	char		TempWorkingID[c_MaxWorkingIDLength+1];
	char		TempWID[c_MaxWorkingIDLength+1];
	char        ItemID[6];
	char		ResultValue[20];
	char		DataBuffer[247];	 //������ݻ�����
	char		RecordBuffer[900];
	SYSTEMTIME	SystemTime;	
	int			nItemCount;		
	int			i;
	int         StrIDLen;
	int         FrameLen;            //��֡�ĳ���
	int         TotalLen;            //֡�ܳ���
	LPSTR		lpResult;
	LPSTR		lpSampleID;
	LPSTR		lpItemID;
	bool		bLogFile=FALSE;
	bool		isdataPacket=FALSE;  //�Ƿ�Ϊ���ݰ���ʶ	
	bool		bCheckSum;			 //У��ͱ�ʾ
	bool		bEventChar;
	
	CommData
	Init()
	nItemCount=0;			   //��Ŀ������ʼ��Ϊ��
	TotalLen = 0;
	ResetOpPointer()	
	bEventChar = FALSE;
	if(NewLogFile("ASTMInfe.txt"))
		bLogFile=TRUE;	
 	FillDevName()					
 	SetCommWaitMask(EV_RXCHAR) 
	
	while (TRUE)
	{		
		beg_label:
		if(bEventChar==FALSE) 	WaitEvent(EV_RXCHAR)//�ȴ��������ݰ�EV_RXCHAR,EV_RXFLAG-�ַ��¼�ָ���ַ�
		else					WaitEvent(EV_RXFLAG);			
		i=0;		 
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);			
			if(RxChar==ENQ)//���ַ�W����ENQ
			{
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)			
				bEventChar = TRUE;
				SetCommWaitMask(EV_RXFLAG)//�ı�Ϊָ�����ַ��¼�
				goto beg_label;
			}						
			if(RxChar==STX)			
				isdataPacket = TRUE;				
			if((RxChar==LF)&&(isdataPacket==TRUE)&&(bEventChar==TRUE))
			{			
				DataBuffer[i]=RxChar;
				i++;
				break;
			}
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;							
			}		
		}while(dwReadLength==1);
		
		isdataPacket = FALSE;
		dwRxBufferLength = i; 		
		lpOpPointer=&DataBuffer[0]; //ȡһ֡����

		if (bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		}

		//�������ݰ�		
		FrameLen = dwRxBufferLength;
		switch(*(lpOpPointer + FrameLen - 5))//�ж�֡���ͣ��м�֡�����֡
			{
			//�м�֡
			case ETB:				
				strncpy(&RecordBuffer[TotalLen],&lpOpPointer[0],FrameLen);
				TotalLen = TotalLen + FrameLen;
				//Ӧ��				
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
			//���֡
			case ETX:									
				strncpy(&RecordBuffer[TotalLen],&lpOpPointer[0],FrameLen);
				TotalLen = 0;
				//����У���
				bCheckSum = TRUE;
				if(bCheckSum == FALSE) 
				{
					//�����ٴη���
					PurgeComm(hComm, PURGE_RXCLEAR);	
					WriteCommChar(NAK)
					break;
				}
				//ȡ�����ݲ���
				SubString(&RecordBuffer[0] , lpOpPointer ,2, FrameLen - 5);
				//��������
				switch(*(lpOpPointer))
				{				
				case 'H': //ͷ֡
					Field_delimiter = *(lpOpPointer+1);
					component_delimiter = *(lpOpPointer+3);
					repeat_delimiter = *(lpOpPointer+2);
					escape_character = *(lpOpPointer+4);
					break;
				case 'P': //������Ϣ֡
					//ȡ��������
					/*
					lpSampleID = &TempWorkingID[0];
					GetResultStr(lpOpPointer, lpSampleID ,Sequence_Number);
					StrIDLen=strlen(lpSampleID);
					//����������䡮0����ʹ���������ȱ�����6���ַ�
					memset(&TempWorkingID[1],0x30,5);
					strncpy(&TempWorkingID[6-StrIDLen],&lpSampleID[0],StrIDLen);
					TempWorkingID[0]=lpDevice->DevItemType;				
		    		TempWorkingID[6]='\0';	
					*/
					break;
				case 'O': //��������֡
					//ȡ��������
					lpSampleID = &TempWID[0];
					GetSampleStr(lpOpPointer, lpSampleID ,4,1);//specimen_no
					StrIDLen=strlen(lpSampleID);
					//����������䡮0����ʹ���������ȱ�����6���ַ�
					memset(&TempWorkingID[1],0x30,5);
					strncpy(&TempWorkingID[6-StrIDLen],&lpSampleID[0],StrIDLen);
					TempWorkingID[0]=lpDevice->DevItemType;				
		    		TempWorkingID[6]='\0';	
					break;
				case 'R': //�������֡		
					//ȡ��ĿID
					lpItemID = &ItemID[0];
					GetItemStr(lpOpPointer,lpItemID,Universal_Test_ID,5);
					StrIDLen=strlen(lpItemID);
					//����ĿID
					strncpy(&OutResult[nItemCount].ItemID[0],&lpItemID[0],StrIDLen);
					OutResult[nItemCount].ItemID[StrIDLen] = '\0';
					OutResult[nItemCount].ItemNo=nItemCount+1;				
					//ȡ���
					lpResult = &ResultValue[0];
					GetResultStr(lpOpPointer,lpResult,Measurement_Value);
					StrIDLen = strlen(lpResult);
					if(StrIDLen==0) break;
					//����
					FillResult(nItemCount, lpResult , StrIDLen) 
					GetSystemTime(&SystemTime);
					//��ʱ��
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)					
					//�������
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					nItemCount = nItemCount + 1;					
					break;
				case 'L': //����֡
					//�н��ʱ,д����
					if(nItemCount>0)
					{
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}			
					nItemCount = 0;
					bEventChar = FALSE;
					SetCommWaitMask(EV_RXCHAR)//�ı�Ϊָ�����ַ��¼�
					break;
				}							
				//Ӧ��
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
				RecordBuffer[0] = '\0';				
			}				
	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;	
} 
//-----------------------------------------------------------------------------//


//-------------------------------�趨�ַ��¼�----------------------------------//
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar = LF;////���ַ�Z����LF
	//�趨���ڼ���¼�,���ڷ���ASTM�ײ��׼�ַ��¼�����ΪLF
}
//-----------------------------------------------------------------------------//