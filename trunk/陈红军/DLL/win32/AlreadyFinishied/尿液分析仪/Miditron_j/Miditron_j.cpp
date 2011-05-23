// Miditron_j.cpp : Defines the entry point for the DLL application.
//Ϊ��������˾��Miditron Junior��Һ�����ǵ���������

#include "stdafx.h"
#include "str.h"
#include "const.h"
#include "device.h"
#include "Devmacro.h"
#include "logfile.h"

#define c_RxBufferLength	512		 
#define	c_MaxItemNum		10	

	 
//���ݰ�ID��ƫ��
#define c_PackIDOff			0x01
//������ݰ����ܺ�ƫ��
#define c_PackFuncOff       0x02	 

//������к�ƫ��
#define c_SeqNoOff          15
#define c_SeqNoLen          5

//��ͬ����ĳ���
#define c_PH_ResLen         0x08
#define c_SG_ResLen         0x08
#define c_NIT_ResLen        0x08
#define c_OTHER_ResLen      0x11

//���
#define c_SG_ItemOff		0x24	 
#define c_SG_ResultOff		0x26	

#define c_PH_ItemOff		0x31	 
#define c_PH_ResultOff		0x33	 

#define c_LEU_ItemOff		0x3c	 
#define c_LEU_ResultOff		0x3f

#define c_NIT_ItemOff		0x50	 
#define c_NIT_ResultOff		0x53	 

#define c_PRO_ItemOff		0x5C	 
#define c_PRO_ResultOff		0x5f	 

#define c_GLU_ItemOff		0x70	 
#define c_GLU_ResultOff		0x73

#define c_KET_ItemOff		0x84	 
#define c_KET_ResultOff		0x87

#define c_UBG_ItemOff		0x98	 
#define c_UBG_ResultOff		0x9b

#define c_BIL_ItemOff		0xac	 
#define c_BIL_ResultOff		0xaf	 
	 
#define c_ERY_ItemOff		0xc0	  
#define c_ERY_ResultOff		0xc3	  

//#define c_SG_ItemOff	    	0x39	 
//#define c_SG_ResultOff		0x3d	 

//#define c_PH_ItemOff	    	0x4d	 
//#define c_PH_ResultOff		0x51	 

//#define c_LEU_ItemOff		0x61	 
//#define c_LEU_ResultOff		0x65	  

//#define c_NIT_ItemOff		0x7b	 
//#define c_NIT_ResultOff		0x7f	 

//#define c_PRO_ItemOff		0x95	 
//#define c_PRO_ResultOff		0x99	 

//#define c_GLU_ItemOff		0xaf	 
//#define c_GLU_ResultOff		0xb3	 

//#define c_KET_ItemOff		0xc9	 
//#define c_KET_ResultOff		0xcd	 

//#define c_UBG_ItemOff		0xe3	 
//#define c_UBG_ResultOff		0xe7	 

//#define c_BIL_ItemOff		0xfd	 
//#define c_BIL_ResultOff		0x101	 

//#define c_ERY_ItemOff		0x117	  
//#define c_ERY_ResultOff		0x11b	  


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
extern HANDLE hLogFile;

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//������,chj
	//Ӧ���
	char* Pro_MOR="\x2>\x3\x33\x45\xD";//"STX<ETX3ECR"
	char* Pro_REP="\x2?\x3\x33\x46\xD";//"STX?ETX3FCR"
	char* Pro_SPM="\x2<\x3\x33\x43\xD";//"STX>ETX3CCR"
	char SampleID[5];
	int  l_ACKTYPE;
 	SYSTEMTIME SystemTime;
	int	 nItemCount,SequenceNo,SampleIDLen;
	bool bLogFile=FALSE;
	int  i,btCalCheckSum,btRxCheckSum,CK1,CK2;//�����������õ��ı���
	LPSTR  lptemp1;
	CommData
	Init()
	nItemCount =0;
	ResetOpPointer()				 
 	FillDevName()		
	if(NewLogFile("Miditon_j.txt"))
		bLogFile=TRUE;		 
 	SetCommWaitMask(EV_RXFLAG)
	//SetCommWaitMask(EV_RXCHAR)
	while (TRUE)
	{
		ResetRxPointer()		 
 		GetInQueueLength(dwReadLength)
 		WaitEvent(EV_RXFLAG)
	//	WaitEvent(EV_RXCHAR)
  		GetInQueueLength(dwReadLength)
  		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	
			continue; 
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		if (lpOpPointer[0]!=STX) 
		{
  			dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
			memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
		}
//		if(bLogFile)
//			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		//����У���	
		i = 2;
		btCalCheckSum = lpOpPointer[1];
		do 
		{
			if ((lpOpPointer[i] ==ETX)||(i>(int)(dwRxBufferLength-4)))
			  break;
			btCalCheckSum +=lpOpPointer[i];
			btCalCheckSum=btCalCheckSum%256;	   
			i++;
		}while(TRUE);
		if (btCalCheckSum!=0)
		{
			CK1=lpOpPointer[i+1]-0x30;
			if(CK1>9)
				CK1 -= 7;
		    CK2 = lpOpPointer[i+2]-0x30;
			if(CK2>9)
				CK2 -= 7;
			btRxCheckSum = CK1 * 16;
		    btRxCheckSum += CK2;
			if (btRxCheckSum!=btCalCheckSum)
			{
				PurgeComm( hComm, PURGE_RXCLEAR );		
				WriteCommBlock(Pro_REP ,strlen(Pro_REP))//У��ʹ����ط�����
				l_ACKTYPE=1;
				continue;
			}
			
		}
		//������
		switch(*(lpOpPointer+c_PackIDOff))
		{
		case '<'://SPM
			PurgeComm( hComm, PURGE_RXCLEAR );
			WriteCommBlock(Pro_MOR,strlen(Pro_MOR))
			l_ACKTYPE=2;
			break;
		case ';'://SPE
			GetLocalTime(&SystemTime);
			switch(*(lpOpPointer+c_PackFuncOff))
			{
			case 0x44://Data Protocol Color+turbidity'D'
				break;
			case 0x45://Data Protocol Results'E'
				TempWorkingID[0]=lpDevice->DevItemType;
				SequenceNo=StrToInt(lpOpPointer+c_SeqNoOff,c_SeqNoLen);
				_itoa(SequenceNo,SampleID,10);
				SampleIDLen=strlen(SampleID);
			 	memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';			
				nItemCount=0;
				//SG
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_SG_ItemOff, 2)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_SG_ResultOff ,c_SG_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//PH
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_PH_ItemOff, 2)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_PH_ResultOff ,c_PH_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//LEU
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_LEU_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_LEU_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//NIT
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_NIT_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_NIT_ResultOff ,c_NIT_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//PRO
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_PRO_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_PRO_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//GLU
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_GLU_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_GLU_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//KET
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_KET_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_KET_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//UBG
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_UBG_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_UBG_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//BIL
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_BIL_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_BIL_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//ERY
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_ERY_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_ERY_ResultOff ,c_OTHER_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				if (nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				break;
			}
			PurgeComm(hComm,PURGE_RXCLEAR);
			WriteCommBlock(Pro_MOR,strlen(Pro_MOR))
			l_ACKTYPE=2;
			break;
		case ':'://END����Ӧ��
			PurgeComm(hComm,PURGE_RXCLEAR);
			break;
		case '?'://REP
			PurgeComm(hComm,PURGE_RXCLEAR);
			switch(l_ACKTYPE)
			{
			case 1:
				WriteCommBlock(Pro_REP,strlen(Pro_REP))
				break;
			case 2:
				WriteCommBlock(Pro_MOR,strlen(Pro_MOR))
				break;
			default:
				WriteCommBlock(Pro_REP,strlen(Pro_REP))
				break;

			}
			break;
		}

	}
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
 	pComDcb->EvtChar = CR;
}

