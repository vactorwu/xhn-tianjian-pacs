// RL7150.cpp : Defines the entry point for the DLL application.
//

#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "device.h"
#include "const.h"
#include "devmacro.h"
#include "str.h"
#include "Logfile.h"
#define c_RxBufferLength	300		//���ջ���������
#define	c_MaxItemNum		23		//�����Ŀ��
#define c_FuncNumOff		1		//������ƫ��
#define c_FuncNumLen		2		//�������ֽ���
#define c_SampleInfoOff		3		//������Ϣƫ��
#define c_SampleIDOff		4		//����IDƫ��
#define c_SampleIDLen		4		//����ID�ֽ���
#define c_ControlIDOff		6		//����IDƫ��
#define c_ControlIDLen		2		//����ID����
#define c_StatIDOff			5		//״̬IDƫ��
#define c_StatIDLen			3		//״̬ID����
#define c_DataOff			25		//����ƫ��
#define c_OneDataLen		9		//һ�����ݳ���
#define c_ItemIDOff			0		//��ĿIDƫ��
#define c_ItemIDLen			2		//��ĿID����
#define c_ResultOff			2		//���ƫ��
#define c_ResultLen			6		//�������
#define c_ErrorCodeOff		8		//������ƫ��
#define c_ErrorCodeLen		1		//�����볤��

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
	CommData
	char  DataBuffer[400];// ���ݻ�������
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nFuncNum,nDataOff,nResultLen;
	int Start;//�������ַ�����ʼλ��
	BYTE  btResultType, btCalCheckSum, btRxCheckSum;
	LPSTR lpPointer;
	bool isdataPacket=FALSE;
	bool IsPacketEnd=FALSE;
	Init()
	nItemCount =0;
	ResetOpPointer()				

	FillDevName()					
	SetCommMask(hComm,EV_RXCHAR);
	nItemCount=0;
	while (TRUE)
	{
		ResetRxPointer()
		nItemCount = 0;
		int ReciveChars=0;			
		//���ϵͳ���ַ��¼������ж����ͣ�������������С�
		do
		{
			ResetRxPointer()
			nItemCount = 0;
			int ReciveChars=0;			
			dwReadLength=0;
			i=0;
			WaitCommEvent(hComm, &dwEvtMask, NULL );			
			if ((dwEvtMask & EV_RXCHAR) != EV_RXCHAR)			
				continue;				
			do
			{
				ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
				ReciveChars++;
				if(RxChar==STX)
					isdataPacket=TRUE;
				if(isdataPacket)
				{
					DataBuffer[i]=RxChar;
					if( i>400 )
						MessageBox(NULL,"���г��Ȳ���","����",MB_OK);
					i++;
				}
				if(IsPacketEnd==TRUE)
				{
					break;
				}
					
				if(RxChar==ENQ||RxChar==EOT)
					break;
				if((RxChar==ETX)|(RxChar==ETB))
				{
					if(isdataPacket)
					{
						if( i>400 )
							MessageBox(NULL,"���г��Ȳ���","����",MB_OK);
						IsPacketEnd=TRUE;
					}
					
				}					
			}while(dwReadLength==1);

			if(isdataPacket)
			{
				//����У���
				btCalCheckSum=0;
				for(i=0;i<ReciveChars-1;i++)
				{
					btCalCheckSum=(DataBuffer[i]+btCalCheckSum)%256;


				}
				lpOpPointer=&DataBuffer[0];
				nFuncNum =StrToInt(lpOpPointer+c_FuncNumOff, c_FuncNumLen);
				nItemCount =0;
				switch (nFuncNum)
				{				  
					case	2:
					case	3:
					case	52:
					case	53:
						 
						GetLocalTime(&SystemTime);
						switch (*(lpOpPointer+c_SampleInfoOff))
						{

							//WorkingID=�����������+�������,
							//��ͬ���鱨������һ��ɶ�λ�������뵥��.
						case	'N':
								wSampleID =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
								Start=FindStrStart(lpOpPointer+c_SampleIDOff,c_SampleIDLen);
								OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
								strncpy(&OutResult[nItemCount].WorkingID[1],lpOpPointer+c_SampleIDOff+Start,c_SampleIDLen-Start);
								OutResult[nItemCount].WorkingID[c_SampleIDLen-Start+1]='\0';
								btResultType =c_TypeSample;
								break;
							case	'C':
								wSampleID =StrToInt(lpOpPointer+c_ControlIDOff, c_ControlIDLen);
								OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
								OutResult[nItemCount].WorkingID[1]='C';
								Start=FindStrStart(lpOpPointer+c_ControlIDOff,c_ControlIDLen);
								strncpy(&OutResult[nItemCount].WorkingID[2],lpOpPointer+c_ControlIDOff+Start,c_ControlIDLen-Start);
								OutResult[nItemCount].WorkingID[c_ControlIDOff-Start+2]='\0';
								btResultType =c_TypeQC;
								break;
							case	'E':
								wSampleID =StrToInt(lpOpPointer+c_StatIDOff, c_StatIDLen);
								OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
								OutResult[nItemCount].WorkingID[1]='S';
								Start=FindStrStart(lpOpPointer+c_StatIDOff,c_StatIDLen);
								strncpy(&OutResult[nItemCount].WorkingID[2],lpOpPointer+c_StatIDOff+Start,c_StatIDLen-Start);
								OutResult[nItemCount].WorkingID[c_StatIDLen-Start+2]='\0';
								wSampleID +=1000;
								btResultType =c_TypeEmergency;
								break;
						}
						strncpy(TempWorkingID,&OutResult[nItemCount].WorkingID[0],c_MaxWorkingIDLength+1);
						if (*(lpOpPointer+c_SampleInfoOff)=='N' || 
							*(lpOpPointer+c_SampleInfoOff)=='C' ||
							*(lpOpPointer+c_SampleInfoOff)=='E')
							for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX && *(lpOpPointer+nDataOff)!=ETB;)
							{
								FillSampleID(nItemCount, wSampleID)	
								//FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)	
								if(*(lpOpPointer+nDataOff+c_ItemIDOff)==' ')
								{
									OutResult[nItemCount].ItemID[0]=*(lpOpPointer+nDataOff+c_ItemIDOff+1);
									OutResult[nItemCount].ItemID[1]='\0';
									OutResult[nItemCount].ItemNo=StrToInt(lpOpPointer+nDataOff+c_ItemIDOff+1,1);
								}
								else
								{
									strncpy(OutResult[nItemCount].ItemID, lpOpPointer+nDataOff+c_ItemIDOff, 2);	
									OutResult[nItemCount].ItemNo=StrToInt(lpOpPointer+nDataOff+c_ItemIDOff,2);
								}
								OutResult[nItemCount].ItemID[2] ='\0';

								OutResult[nItemCount].ItemNo=StrToInt(lpOpPointer+nDataOff+c_ItemIDOff,c_ItemIDLen);
								DeleZero(lpOpPointer,c_ResultLen)
								strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
								FillResult(nItemCount, lpPointer ,nResultLen ) 
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								FillResultType(nItemCount, btResultType)
								FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
								OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//����ǰ�����¼�����������ָ��ָ���������������.
								FillErrorCode(nItemCount, c_Reliable)					
 								nItemCount++;   
								nDataOff+=c_OneDataLen;  
							}
						break;
				}
				if (nItemCount>0)
				{   
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				PurgeComm( hComm, PURGE_RXCLEAR );	
				if (nFuncNum>50)	
				{
		 			WriteCommChar(ACK)
				} 
			} 	
			isdataPacket=FALSE;
			IsPacketEnd=FALSE;
			ReciveChars=0;
			lpOpPointer=NULL;
		} while(TRUE);
}

return TRUE;

} // end of CommWatchProc()


void WINAPI BeforeSetCommState(DCB *pComDcb)
{

}