// CLT_JTIR.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "stdio.h"
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"

#define c_RxBufferLength	1024 //定义缓冲区大小		
#define	c_MaxItemNum		18  //定义项目数	 
 
#define c_DataOff           93
#define c_ResultOff         6
#define c_ItemIDLen         4
#define c_ResultLen         6
#define c_OneDataLen        16

#define One_Package_Length 399
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
	int i;
	char SampleID[5];
	bool bLogFile=FALSE;
	CommData
	SYSTEMTIME SystemTime;
    int	nItemCount,nDataOff;

	int  btCalCheckSum, btRxCheckSum,CK1,CK2,SampleIDLen;
	WORD 		wSampleNo;						
	BYTE		btResultType;
	LPSTR       lptemp1;
	Init()
	if(NewLogFile("Kltdata.txt"))
		bLogFile=TRUE;
	ResetOpPointer()				
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	//if (!SetCommMask(hComm,EV_RXCHAR))
	//	MessageBox(NULL,"SET EV_RXCHAR, FALSE","",MB_OK);

	
	while (TRUE)	
	{
        PurgeComm( hComm, PURGE_RXCLEAR );
        
		while(TRUE)
		{	
			WaitEvent(EV_RXFLAG)
			
            ResetRxPointer()
			GetInQueueLength(dwReadLength)
		   	ReadCommBlock(dwReadLength)
			if(bLogFile)
				WriteLogData(hLogFile,lpOpPointer,dwReadLength);
			//读取一个数据包，
			lptemp1=strchrnb(lpOpPointer,STX,1,20);
			if ((lpOpPointer[0]!=STX) & (lptemp1!=NULL)) 
			{
  				dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
				memmove( lpOpPointer, lptemp1,dwRxBufferLength );
			} 
   			//数据校验
			if (lpOpPointer[0]!=STX) 
			{
				MessageBox(NULL,"错误数据，请重发","警告",MB_OK);
				continue;
			}
		    btCalCheckSum =lpOpPointer[1];
		 //从第1位到结束符前两位字符相加，求校验和
			i=2;
			do 
			{
				if (lpOpPointer[i+2] ==0x03 )
				  break;
				btCalCheckSum +=lpOpPointer[i];
				btCalCheckSum=btCalCheckSum%256;	   
				i++;
			}while(TRUE);
			btCalCheckSum=256-btCalCheckSum;
			if (btCalCheckSum!=0)
			{
				CK1=lpOpPointer[i]-0x30;
				if(CK1>9)
					CK1 -= 7;
				CK2 = lpOpPointer[i+1]-0x30;
				if(CK2>9)
					CK2 -= 7;
				btRxCheckSum = CK1 * 16;
				btRxCheckSum += CK2;
				if (btRxCheckSum!=btCalCheckSum)
				{
					PurgeComm( hComm, PURGE_RXCLEAR );		
					WriteCommChar(NAK)
					continue;
				}
			}
           //将数据转存到结果数组中
		    nItemCount =0;
 		    GetLocalTime(&SystemTime);
		    wSampleNo =StrToInt(lpOpPointer+49, 10);
		    btResultType =c_TypeSample;


			
			IntToStr(SampleID,wSampleNo);

			nDataOff=c_DataOff;//=91

			while (nItemCount<18)
			{
				//合成工作单号，并将合成的工作单号写入结果数组OutResult
			     OutResult[nItemCount].WorkingID[0]=lpDevice->DevItemType;
			   //  strncpy(&OutResult[nItemCount].WorkingID[1],SampleID,5);
	 			 SampleIDLen=strlen(SampleID);
			 	 memset(&OutResult[nItemCount].WorkingID[1],0x30,5);
				 strncpy(&OutResult[nItemCount].WorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
				 OutResult[nItemCount].WorkingID[6]='\0';
				 OutResult[nItemCount].ItemNo=nItemCount+1;
		         FillSampleID(nItemCount, wSampleNo)
				 strncpy(OutResult[nItemCount].ItemID, lpOpPointer+nDataOff, c_ItemIDLen);
				 OutResult[nItemCount].ItemID[4]='\0';
				 if((OutResult[nItemCount].ItemID[2]==32)&(OutResult[nItemCount].ItemID[3]==32))
					OutResult[nItemCount].ItemID[2]='\0';
				 if(OutResult[nItemCount].ItemID[3]==32)
					OutResult[nItemCount].ItemID[3]='\0';
                 //将当前结果记录的仪器结果数指针指向仪器结果数变量.
                 OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
                 FillResult(nItemCount,  lpOpPointer+nDataOff+c_ResultOff,c_ResultLen)
				 ReplaceAllCharInStr(&OutResult[nItemCount].AssayResult[1],'N',' ',5);
				 ReplaceAllCharInStr(&OutResult[nItemCount].AssayResult[1],'R',' ',5);
			     ReplaceAllCharInStr(&OutResult[nItemCount].AssayResult[1],'B',' ',5);
			     FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
				 nItemCount++;
				 nDataOff+=c_OneDataLen;
			}
			
			//调用结果处理例程		
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
			}
		}
    
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;

} 

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}