// cltMAXM.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength	800		
#define	c_MaxItemNum		40	

#define c_BlockCountOff		2	//接收的数据中block count的偏移量
#define c_BlockCountLen		2	//block count的宽度(两个字节，即最多256个)

#define	c_BlockSize			256
#define	c_MaxBlocks			256

#define	c_blksz1_DL			256	//数据链路层数据块长度1
#define	c_blksz2_DL			128	//数据链路层数据块长度2

#define	c_GroupHeader		5	//DC1+FieldCount+CR+LF
#define c_CBCResultOff		5	//接收的数据中block count的偏移量
#define c_CBCResultLen		6	//block count的宽度(两个字节，即最多256个)

#define lgb_DEBUG

#ifdef lgb_DEBUG
#include "LogFile.h"
#define c_LogDataLink		"datalink.log"
#define c_LogPresentation	"presentation.log"
char	szDebug[100];
LPSTR	lpDebug;
#endif 

HANDLE hLogFileDL, hLogFilePR;

void calc_crc16(BYTE data_byte, BYTE& crcmsb, BYTE& crclsb);
bool Presentation_level(LPSTR lpData, LPSTR szBufferPresentation);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DWORD WINAPI CommMonitor( LPSTR lpData )
{
//	int	 nItemCount;
	char szBufferPresentation[c_BlockSize*c_MaxBlocks+1];
	int  nDataLength;

	LPSTR  lpSTX=NULL;
	LPSTR  lpETX=NULL;

	BYTE crcmsb,crclsb;

	int  nBlockCount;
	int  nBlockNo;

	int  nBlockLength;

	hLogFileDL = NULL;

	int  j;

	//监控串口接收数据，并完成数据格式分析，将结果放入结果队列
	CommData

	Init()

	EscapeCommFunction(hComm,SETDTR);
	EscapeCommFunction(hComm,SETRTS);
 

	ResetRxPointer()			
	ResetOpPointer()

	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );

	NewLogFile(hLogFileDL, c_LogDataLink);
	NewLogFile(hLogFilePR, c_LogPresentation);

	while (TRUE)
	{
		nDataLength = 0;
		
		WaitCommChar(SYN)		//----->SYN
		WriteLogChar(hLogFileDL,SYN);
		WriteCommChar(SYN)		//<-----SYN

		ResetRxPointer()		
		FetchCommBlock(2)		//------>Block Count		
		
		WriteLogData( hLogFileDL,lpOpPointer,2 );	//-->log

		nBlockCount = HexToInt(*lpOpPointer)*16 + HexToInt(*(lpOpPointer+1));
		
		if (nBlockCount==0)
		{
			WriteCommChar(NAK)	//<------NAK
			continue;
		}
		
		WriteCommChar(ACK);		//------>ACK
		
		//----------------------将链路层的数据组装到szBufferPresentation中去
								//------>block1--block(blockcount)
		nBlockNo = 0;
		while (nBlockNo<nBlockCount)
		{
								//------>Get BlockX
			ResetRxPointer()		
			ResetOpPointer()

			SetCommWaitMask(EV_RXFLAG)
			WaitEvent(EV_RXFLAG)	//wait for ETX
			
			GetInQueueLength(dwReadLength)
			ReadCommBlock(dwReadLength)

			WriteLogData(hLogFileDL,lpOpPointer,dwReadLength);	//-->log

								//find where is STX
			lpSTX = strchrnb(lpOpPointer, STX, 1, dwReadLength);
			if (lpSTX==NULL) continue;
								//find where is ETX
			lpETX = strchrnb(lpOpPointer, ETX, 1, dwReadLength);
			if (lpSTX==NULL) continue;
								//块长度.块结构:STX+(2字节块标识)+BLOCK(数据块)+CRC(4字节CRC)+ETX
			nBlockLength = (lpETX-4)-(lpSTX+3);
								//CRC16校验
			crcmsb = 0xff;
			crclsb = 0xff;
			for (j=0; j<nBlockLength; j++)	
				calc_crc16(*(lpSTX+3+j), crcmsb, crclsb);
			crcmsb ^= 0xff;
			crclsb ^= 0xff;
			
								//判断校验值是否正确
			//if (crcmsb==HexToInt(*(lpETX-4))*16 + HexToInt(*(lpETX-3)) && 
			//	crclsb==HexToInt(*(lpETX-2))*16 + HexToInt(*(lpETX-1)))
			if (true)
			{
				WriteCommChar(ACK)	//<------ACK
				
				//NULL、HT------>SP
				for (j=0; j<nBlockLength; j++)	//块长度.块结构:STX+(2字节块标识)+BLOCK+CRC(4字节CRC)+ETX
					if (*(lpSTX+3+j)==NULL || *(lpSTX+3+j)==HT)
						*(lpSTX+3+j) = SP ;
				
				//======>表现层(Presentation Level)缓冲区
				WriteLogData(hLogFilePR,lpSTX+3, nBlockLength );	//-->log

				memcpy( szBufferPresentation+nDataLength, lpSTX+3, nBlockLength );
				nDataLength += nBlockLength;
				nBlockNo++;
			}
			else
			{
				WriteCommChar(NAK)	//<------NAK
			}
		}	//end while

		WaitCommChar(SYN)			//------>SYN
		WriteLogChar(hLogFileDL,SYN);	//-->log
		
		szBufferPresentation[nDataLength]='\0';	

		if (Presentation_level(lpData, LPSTR(szBufferPresentation)))		//将数据包传送给表示层 
		{
			WriteCommChar(ACK)	//发送ACK
		}
		else
		{
			WriteCommChar(ACK)	//发送ACK!!!!!:目前资料上没有关于拒绝数据包的说明
		}
	}
	
	#ifdef lgb_DEBUG
		CloseLogFile(hLogFileDL);
		CloseLogFile(hLogFilePR);
	#endif

	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar = ETX; 	//设定串口监控事件
}

void calc_crc16(BYTE data_byte, BYTE& crcmsb, BYTE& crclsb)
{
	data_byte ^= crcmsb;
	data_byte ^= (data_byte / 16);
	crcmsb = crclsb ^ (data_byte / 8) ^ (data_byte * 16);
	crclsb = data_byte ^ (data_byte * 32);
}

//表现层：Presentation level
bool Presentation_level(LPSTR lpData, LPSTR szBufferPresentation)
{
	char *PREAMBLE="\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a--------------\x0d\x0a";
	char *POSTAMBLE="\x11\x0d\x0a\x0d\x0a--------------\x0d\x0a";
	LPSTR lpPtr,lpLineBegin,lpLineEnd,lpTagEnd;
	SYSTEMTIME SystemTime;
	char LSampleID[7],TempWorkingID[7];
	int nItemCount;
	char szTag[20+1];
	int SampleID,SampleIDLen;
	CommData

	lpDevice =(LPDEVICE)lpData;		
	lpResultProcessRoutine =lpDevice->lpResultProcessRoutine;		
											
	for (nIntCount =0; nIntCount<c_MaxItemNum; nIntCount++)	
	{										
		memset(LPSTR(OutResult[nIntCount].InstrumentID), 0, c_MaxDevNameLength);	
		OutResult[nIntCount].SampleID	=0;			
		memset(LPSTR(OutResult[nIntCount].ItemID), 0, c_MaxItemNameLength);
		memset(LPSTR(OutResult[nIntCount].AssayResult), 0, c_MaxResultLength);
		OutResult[nIntCount].ResultType =c_TypeSample;			
		OutResult[nIntCount].ErrorCode  =c_Reliable;			
		memset(LPSTR(OutResult[nIntCount].Comment), 0, c_MaxCommentLength);
	}								
	
	FillDevName()						
	lpPtr = szBufferPresentation ;
	lpLineBegin = szBufferPresentation ;
	lpLineEnd = szBufferPresentation ;

	GetLocalTime(&SystemTime);	
	nItemCount =0;
	
	//匹配包前缀PREAMBLE
	if (strncmp(lpLineBegin,PREAMBLE,lstrlen(PREAMBLE))==0)
		lpLineBegin += lstrlen(PREAMBLE) ;
	else
		return false ;

	//去掉包后缀POSTAMBLE
	//here we do nothing

	while (true)
	{
		lpLineEnd = (strchrnb(lpLineBegin, CR, 1, strlen(lpLineBegin)));
		if(lpLineEnd==NULL)
		{
			if(nItemCount>0)
			{
				(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
				lpDevice->dwRecordCount+=nItemCount;
			}
			return true;
		}
	
		
		lpTagEnd = (strchrnbNotNull(lpLineBegin, SP, 1, 20));
		if(lpTagEnd==lpLineBegin+20)
		{	
			memcpy( szTag , lpLineBegin, 20);
			szTag[20] = '\0';
		}
		else
		{
			memcpy( szTag , lpLineBegin, lpTagEnd - lpLineBegin );
			szTag[lpTagEnd - lpLineBegin] = '\0';
		}

		//判断tag
		//还有一种情况,试管本身上的条码未处理!
		//手工输入的号码
		
		if(strncmp(szTag,"ID1",3)==0)
		{
			SampleID=StrToInt(lpTagEnd + 1,16);
			if (SampleID!=0)
			{
				_itoa(SampleID,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				//向工作单中填充‘0’，使工作单长度保持在6个字符
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';
			}
		}
		//试管架上的号码
		else if(strncmp(szTag,"1EC/P",5)==0)
		{
			SampleID=StrToInt(lpTagEnd + 1,16);
			if (SampleID!=0)
			{
				_itoa(SampleID,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				//向工作单中填充‘0’，使工作单长度保持在6个字符
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';
			}

		}
		else if (strncmp(szTag,"WBC",3)==0) 
		{
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"RBC",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].AssayResult[6] = '\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"HGB",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"HCT",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[4]='\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
					
		else if (strncmp(szTag,"MCV",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
					
		else if (strncmp(szTag,"MCH",3)==0)
		{	
			if (strncmp(szTag,"MCHC",4)==0)
			{	
				strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
				strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
				OutResult[nItemCount].AssayResult[6] = '\0';
				
				strncpy(&OutResult[nItemCount].ItemID[0],szTag,4);
				OutResult[nItemCount].ItemID[4]='\0';
			
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
			}
			 else
			 {

				strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
				strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
				OutResult[nItemCount].AssayResult[6] = '\0';
				strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
				OutResult[nItemCount].ItemID[3]='\0';
				OutResult[nItemCount].ItemNo = nItemCount+1;
				nItemCount++;
			 }
		}
					
					
		else if (strncmp(szTag,"RDW",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
					
		else if (strncmp(szTag,"PLT",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
										
		else if (strncmp(szTag,"MPV",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"PDW",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"LY#",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"MO#",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"NE#",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"EO#",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"BA#",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"LY%",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"MO%",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"NE%",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"EO%",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}
		else if (strncmp(szTag,"BA%",3)==0)
		{	
			strncpy(&OutResult[nItemCount].WorkingID[0],&TempWorkingID[0],7);
			strncpy(&OutResult[nItemCount].AssayResult[0],lpTagEnd + 1, 6);
			OutResult[nItemCount].AssayResult[6] = '\0';
			strncpy(&OutResult[nItemCount].ItemID[0],szTag,3);
			OutResult[nItemCount].ItemID[3]='\0';
			OutResult[nItemCount].ItemNo = nItemCount+1;
			nItemCount++;
		}

		lpLineBegin = lpLineEnd + 2 ;
	}

	return true;
}

