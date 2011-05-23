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
#define c_RxBufferLength	300		//接收缓冲区长度
#define	c_MaxItemNum		23		//最大项目数
#define c_FuncNumOff		1		//函数号偏移
#define c_FuncNumLen		2		//函数号字节数
#define c_SampleInfoOff		3		//样本信息偏移
#define c_SampleIDOff		4		//样本ID偏移
#define c_SampleIDLen		4		//样本ID字节数
#define c_ControlIDOff		6		//控制ID偏移
#define c_ControlIDLen		2		//控制ID长度
#define c_StatIDOff			5		//状态ID偏移
#define c_StatIDLen			3		//状态ID长度
#define c_DataOff			25		//数据偏移
#define c_OneDataLen		9		//一个数据长度
#define c_ItemIDOff			0		//项目ID偏移
#define c_ItemIDLen			2		//项目ID长度
#define c_ResultOff			2		//结果偏移
#define c_ResultLen			6		//结果长度
#define c_ErrorCodeOff		8		//错误码偏移
#define c_ErrorCodeLen		1		//错误码长度

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
	CommData
	char  DataBuffer[400];// 数据缓冲区！
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nFuncNum,nDataOff,nResultLen;
	int Start;//工作单字符串起始位置
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
		//监控系统的字符事件，并判断类型，将其放入结果队列。
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
						MessageBox(NULL,"队列长度不够","警告",MB_OK);
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
							MessageBox(NULL,"队列长度不够","警告",MB_OK);
						IsPacketEnd=TRUE;
					}
					
				}					
			}while(dwReadLength==1);

			if(isdataPacket)
			{
				//计算校验和
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

							//WorkingID=仪器分类编码+样本序号,
							//它同检验报告日期一起可定位检验申请单号.
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
								OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//将当前结果记录的仪器结果数指针指向仪器结果数变量.
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