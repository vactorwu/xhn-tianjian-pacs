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
#define c_FuncNumOff		9		//�����ڶ��ֽ�ƫ��
#define	c_ItemNameOff		17		//�Ǽ�����Ŀ��ƫ��
#define	c_ItemNameLen		3		
#define	c_StreamIDOff		4	    //�����ƫ��
#define	c_StreamIDLen		3	
#define	c_CalcItemNameOff	27		//������Ŀ����ƫ��
#define	c_CalcItemNameLen	1		
#define	c_SampleNoOff		11		
#define	c_SampleNoEOff		25		//�������ڱ�β����ƫ��
#define	c_ResultOff			21		//�Ǽ�����ƫ��
#define	c_SampleNoLen		5		//�����ų���
#define	c_ResultLen			9		//�������
#define	c_CalcResultOff		17		//������ƫ��
#define	c_TrayNumHOff		11		//�̺��ڱ�ͷ���ڵ�ƫ��
#define	c_CupNumHOff		14		
#define	c_TrayNumEOff		31		
#define	c_CupNumEOff		34		//�����ڱ�β���ڵ�ƫ��
#define	c_TrayNumLen		2		//�̺ų���
#define	c_CupNumLen			2		//���ų���
#define	c_CupHeaderPackLen	71		//��ͷ����	
#define	c_TestResultPackLen 89		//�������
#define c_SPCalcuPackLen	33		//�������
#define	c_EndCupPackLen		41		//����������
#define	c_EndTrayPackLen	15		//�̽�������

#define	c_CupsOfOneTray			40		
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
	BYTE  PackFun,SampleIDLen ;//���ݰ����ܺ�,����ID����
	WORD	wSampleNo;
	LPSTR hRXBuffer;
	LPSTR lptemp1;
	bool blogfile;
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
	char LSampleID[5];
	Init()
	
	ResetOpPointer()				
	
	ResetRxPointer()
	

 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	blogfile=NewLogFile("Syn_ELISE.txt");
	WriteCommChar(XON)
	while (TRUE)
	{
	


		WaitEvent(EV_RXFLAG)


		GetInQueueLength(dwReadLength)
 		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	continue; 
		lptemp1=strchrnb(lpOpPointer,'[',1,20);
		//���溯��������λ����ʼλ�ڽ��ջ������е�λ��.???
		if (lpOpPointer[0]!='[') 
		{	
			if(lptemp1!=NULL)
			{
				dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
				memmove( lpOpPointer, lptemp1,dwRxBufferLength );			
			}
		}
		hRXBuffer=lpOpPointer;
		if (blogfile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		}
		
	//У��͵ļ��㷽��:checksum=256-((sum of character )modulo 256))
	//              covert checksum to hexdecimals

		PackFun= *(hRXBuffer+c_FuncNumOff);
		switch (PackFun)
		{
			case '1':		
				if (strncmp(hRXBuffer+c_StreamIDOff ,"102",3)==0)
				{
				
					TrayNum=(int)StrToInt(hRXBuffer+c_TrayNumEOff,c_TrayNumLen);				
					CupNum=(int)StrToInt(hRXBuffer+c_CupNumEOff,c_CupNumLen);
				
					
					wSampleNo =(int)StrToInt(hRXBuffer+c_SampleNoEOff,c_SampleNoLen);
					_itoa(wSampleNo,LSampleID,10);
					TempWorkingID[0]=lpDevice->DevItemType;
					memset(&TempWorkingID[1],0x30,5);
					SampleIDLen=strlen(LSampleID);
					strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
					TempWorkingID[6]='\0';
				    break;
				}
				else	
				{
					nItemCount=0;
					hRXBuffer=(LPSTR)RxBuffer;
					ResetRxPointer()		
					ResetOpPointer()
					continue;
				}
			case '3':	
				
			case '5':	
					FillSampleID(nItemCount, wSampleNo)	
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);//��д��������
					OutResult[nItemCount].ItemNo=nItemCount+1;
					if (PackFun=='3')
					{
	
						FillItemName(nItemCount,lpOpPointer+c_ItemNameOff ,c_ItemNameLen)	
//����
						FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)

					}
					else	
					{
	
						FillItemName(nItemCount,lpOpPointer+c_CalcItemNameOff ,c_CalcItemNameLen)	

						FillResult(nItemCount,lpOpPointer+c_CalcResultOff,c_ResultLen)
					}
//������
					GetSystemTime(&SystemTime); 
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nItemCount++;
					break;	//��������	

			case '7':	
				
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;  
					nItemCount=0;
		
					break;
			case '9':	
				break;			
			default :	
				break;
		}
        WriteCommChar(ACK)
	
		ResetRxPointer()		
		ResetOpPointer()
	
	}
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
	//�趨���ڼ���¼�
}

