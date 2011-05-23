// Miditron_J2.cpp : Defines the entry point for the DLL application.
//为宝灵曼公司的Miditron Junior II 尿液分析仪的联机程序
//数据偏移的定义按10位ID来定。
#include "stdafx.h"
#include "str.h"
#include "const.h"
#include "device.h"
#include "Devmacro.h"
#include "logfile.h"

#define c_RxBufferLength	512		 
#define	c_MaxItemNum		10	

//数据包ID号偏移
#define c_PackIDOff			0x01
//结果数据包功能号偏移
#define c_PackFuncOff       0x02	 

//结果序列号偏移
#define c_SeqNoOff          15
#define c_SeqNoLen          5

//不同结果的长度
#define c_PH_ResLen         0x08
#define c_SG_ResLen         0x08
#define c_NIT_ResLen        0x08
#define c_OTHER_ResLen      0x11

//结果偏移
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

#define c_NAG_ItemOff		0xd5
#define c_NAG_ResultOff     0xd8

#define c_COLOR_ItemOff         37
#define c_COLOR_ResLen         7
 
#define c_TURBIDITY_ItemOff     56
#define c_TURBIDITY_ResLen     18
  
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
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
	/*应答包*/
	char* Pro_MOR="\x2>\x3\x33\x45\xD";//"STX<ETX3ECR"
	char* Pro_REP="\x2?\x3\x33\x46\xD";//"STX?ETX3FCR"
	char* Pro_SPM="\x2<\x3\x33\x43\xD";//"STX>ETX3CCR"
	char SampleID[5];
	int  l_ACKTYPE;//上次应答的类型
 	SYSTEMTIME SystemTime;
	int	 nItemCount,SequenceNo,SampleIDLen;
	bool bLogFile=FALSE;
	int  i,btCalCheckSum,btRxCheckSum,CK1,CK2;//计算检验核所用到的变量
	CommData   //公用数据块
	Init()     //初始化
	nItemCount =0;
	ResetOpPointer() //重置操作指针				 
 	FillDevName()		
	if(NewLogFile("Miditonj_II.txt"))
		bLogFile=TRUE;		 
 	SetCommWaitMask(EV_RXFLAG)
	while (TRUE)
	{
		ResetRxPointer()		 
 		GetInQueueLength(dwReadLength)
 		WaitEvent(EV_RXFLAG)
  		GetInQueueLength(dwReadLength)
  		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	
			continue; 
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
	//	lptemp1=strchrnb(lpOpPointer,STX,1,20);
	//	if (lpOpPointer[0]!=STX) 
	//	{
  	//		dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
	//		memcpy( lpOpPointer, lptemp1,dwRxBufferLength );
	//	}
		if(bLogFile)
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		//计算校验核	
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
				WriteCommBlock(Pro_REP ,strlen(Pro_REP))//校验和错误，重发数据
				l_ACKTYPE=1;
				continue;
			}
			
		}//计算校验核结束

		//处理结果
		switch(*(lpOpPointer+c_PackIDOff))
		{
		case '<'://SPM
			PurgeComm( hComm, PURGE_RXCLEAR );
			WriteCommBlock(Pro_MOR,strlen(Pro_MOR))
			l_ACKTYPE=2;
			break;
		case ';'://SPE 结果数据处理
			GetLocalTime(&SystemTime);
			switch(*(lpOpPointer+c_PackFuncOff))
			{
			case 0x44://Data Protocol Color+turbidity'D'
				TempWorkingID[0]=lpDevice->DevItemType;
				SequenceNo=StrToInt(lpOpPointer+c_SeqNoOff,c_SeqNoLen);
				_itoa(SequenceNo,&SampleID[1],10);
				SampleIDLen=strlen(SampleID);
			 	memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';	
				nItemCount=0;
				//COLOR
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, "COLOR", 5)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_COLOR_ItemOff ,c_COLOR_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				//TURBIDITY
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, "TURBIDITY", 9)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_TURBIDITY_ItemOff ,c_TURBIDITY_ResLen) 
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
				break;

			case 0x45://Data Protocol Results'E'
/*				TempWorkingID[0]=lpDevice->DevItemType;
				SequenceNo=StrToInt(lpOpPointer+c_SeqNoOff,c_SeqNoLen);
				_itoa(SequenceNo,&SampleID[1],10);
				SampleIDLen=strlen(SampleID);
			 	memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';			
				nItemCount=0;*/
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
				//NAG
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				FillItemName(nItemCount, lpOpPointer+c_NAG_ItemOff, 3)
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				FillResult(nItemCount, lpOpPointer+c_NAG_ResultOff ,c_OTHER_ResLen) 
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
		case ':'://END--不需应答，清空接收、发送缓冲区
			PurgeComm(hComm,PURGE_RXCLEAR);
			break;
		case '?'://REP--重发上次发送的应答包
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

