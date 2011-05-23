// Vitros250.cpp : Defines the entry point for the DLL application.
//
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "device.h"
#include "const.h"
#include "devmacro.h"
#include "str.h"
#include "Logfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define c_RxBufferLength	512	
#define	c_MaxItemNum		60	
#define c_SampleIDOff		29		
#define c_SampleIDLen		15	
#define c_SampleTypeOff       44
#define c_ResultOff			9		
#define c_ResultLen			8		
	
#define c_ItemIDOff			5		
#define c_ItemIDLen			4		
		
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
	//У��˵ļ��㷽������ÿ���ַ��������ͣ�ȡ256ģ
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
	char    DataBuffer[200];
	CommData
	char  Ackchar=0x2B;//'+'
	char  Nakchar=0x2D;//'-'
	char  Cancelchar=0x3F;//'?'
 	SYSTEMTIME SystemTime;
	bool isStat=FALSE;//�Ƿ�Ϊ��������!
	bool bLogFile=FALSE;
	int	 nItemCount,nResultLen,AnyTimes=0;
	int SampleID,i;
	char LSampleID[5];
	char SampleType,SampleIDLen;
	LPSTR lpPointer;
	bool isdataPacket;
	//�������ݵ�Log�ļ�
	Init()
	nItemCount =0;
	ResetOpPointer()
	if(NewLogFile("Vitros250.txt"))
		bLogFile=TRUE;
	EscapeCommFunction(hComm,SETRTS);
 	FillDevName()					
 	SetCommWaitMask(EV_RXCHAR ) 
	while (TRUE)
	{

	 	WaitEvent(EV_RXCHAR)  
		i=0;
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar == 0x21)
				isdataPacket = TRUE;
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;
			}
			if((RxChar==LF)& (isdataPacket == TRUE))
			{
				EscapeCommFunction(hComm,CLRRTS);
				DataBuffer[i]=RxChar;
				i++;
				break;
			}	
				
		}while(dwReadLength==1);
		isdataPacket=FALSE;
		dwRxBufferLength = i; 
		lpOpPointer=&DataBuffer[0];
	
		//����У���
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		if(lpOpPointer[0] == 0x21)
		{
			switch(*(lpOpPointer+4))//���ݰ�����
			{
			case 'a'://ͷ��¼ header record
				//���������Ÿ�ֵ
				SampleID=StrToInt(lpOpPointer+c_SampleIDOff,c_SampleIDOff);
				_itoa(SampleID,LSampleID,10);
				if((*(lpOpPointer+64))=='T')//����
				{
					TempWorkingID[0]=lpDevice->DevItemType;
					TempWorkingID[1]='S';
				//	strncpy(&TempWorkingID[2],LSampleID,5);
				
				}
				else//����
				{
					TempWorkingID[0]=lpDevice->DevItemType;
					TempWorkingID[1]=0x30;
				//	strncpy(&TempWorkingID[1],LSampleID,5);
				}
				//����������䡮0����ʹ���������ȱ�����6���ַ�
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[2],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
			    TempWorkingID[6]='\0';
				//
				SampleType=*(lpOpPointer+c_SampleTypeOff);
				//Ӧ������ͷ��¼
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'c'://����������¼patient description record
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'd'://ҽ��������¼doctor description record
		//		ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'e'://������¼ miscellaneous result
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'f'://���Խ����¼ test result
				/*����������Ŀ����
				�������ж�λ����ǰ������Ŀ��˳��ֵ,˳��ֵΪ��̬�����е�˳��ţ�
				��ӳΪ����еĴ�ӡ��ţ�Print_order��*/
				lpPointer=lpOpPointer+c_ResultOff;
				if(!((*lpPointer=='?')&(*lpPointer=='*')))
				{
				
					strncpy(OutResult[nItemCount].ItemID,lpOpPointer+c_ItemIDOff,c_ItemIDLen);
					switch(SampleType)
					{
					case  '1'://����ΪѪ��
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='\0';
							}
							else
							 OutResult[nItemCount].ItemID[3]='\0';
						}
						OutResult[nItemCount].ItemID[4]='\0';
						break;
					case  '2'://����Ϊ�Լ�Һ
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='C';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='C';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='C';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
		
					case  '3'://����Ϊ��Һ

						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='U';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='U';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='U';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
					}
					OutResult[nItemCount].ItemNo=nItemCount+1;

					OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//����ǰ�����¼�����������ָ��ָ���������������.
					
					DeleSpace(c_ResultLen)
					FillResult(nItemCount, lpPointer ,nResultLen ) 
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					GetSystemTime(&SystemTime);  
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 					nItemCount++;   
				}
			//	ResetRxPointer()
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'g'://�������Խ����¼Derived test result
				lpPointer=lpOpPointer+c_ResultOff;
				if((*lpPointer!='?')&(*lpPointer!='*'))
				{
				
					strncpy(OutResult[nItemCount].ItemID,lpOpPointer+c_ItemIDOff,c_ItemIDLen);
					switch(SampleType)
					{
					case  '1'://����ΪѪ��
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='\0';
							}
							else
							 OutResult[nItemCount].ItemID[3]='\0';
						}
						OutResult[nItemCount].ItemID[4]='\0';
						break;
					case  '2'://����Ϊ�Լ�Һ
						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='C';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='C';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='C';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
		
					case  '3'://����Ϊ��Һ

						if(OutResult[nItemCount].ItemID[3]==0x20)
						{
							if(OutResult[nItemCount].ItemID[2]==0x20)
							{
								OutResult[nItemCount].ItemID[2]='_';
								OutResult[nItemCount].ItemID[3]='U';
								OutResult[nItemCount].ItemID[4]='\0';
							}
							else
							{
								OutResult[nItemCount].ItemID[3]='_';
								OutResult[nItemCount].ItemID[4]='U';
								OutResult[nItemCount].ItemID[5]='\0';
								
							}
						}
						else
						{
							OutResult[nItemCount].ItemID[4]='_';
							OutResult[nItemCount].ItemID[5]='U';
							OutResult[nItemCount].ItemID[6]='\0';
						}
						break;
					}
					OutResult[nItemCount].ItemNo=nItemCount+1;
					OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//����ǰ�����¼�����������ָ��ָ���������������.
					DeleSpace(c_ResultLen+1)
					FillResult(nItemCount, lpPointer ,nResultLen ) 
					strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
					GetSystemTime(&SystemTime);
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 					nItemCount++;   
				}
			//	ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				break;
			case 'h'://������¼Trailer
				if (nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
		//		ResetRxPointer() 
				EscapeCommFunction(hComm,SETRTS);
				nItemCount=0;
				break;
			}
		}
		else
		{
			MessageBox(NULL,"DataError","HAha",MB_OK);
		//	ResetRxPointer() 
			EscapeCommFunction(hComm,SETRTS);
		}
	

	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;
}
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;//Line feed
	// ����ͨ���¼��ַ�,�����ڽ��յ����ַ������뻺������,����һ���¼�֪ͨ���ڼ���߳�
	//���ַ�����������ͬ���ı�. ��ע��  �º����2000.10.25 
}

