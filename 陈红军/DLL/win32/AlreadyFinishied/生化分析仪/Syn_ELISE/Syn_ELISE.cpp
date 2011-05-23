// Syn_ELISE.cpp : Defines the entry point for the DLL application.
//美国贝克曼公司的Synchron EL_ISE ELECTROLYTE  System 动态链接库

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"

#define c_RxBufferLength	800		//数据处理缓冲区长
#define	c_MaxItemNum		8		//一次最多处理的项目数
#define c_FuncNumOff		9		//包类别第二字节偏移
#define	c_ItemNameOff		17		//非计算项目名偏移
#define	c_ItemNameLen		3		
#define	c_StreamIDOff		4	    //流类别偏移
#define	c_StreamIDLen		3	
#define	c_CalcItemNameOff	27		//计算项目名称偏移
#define	c_CalcItemNameLen	1		
#define	c_SampleNoOff		11		
#define	c_SampleNoEOff		25		//样本号在杯尾包的偏移
#define	c_ResultOff			21		//非计算结果偏移
#define	c_SampleNoLen		5		//样本号长度
#define	c_ResultLen			9		//结果长度
#define	c_CalcResultOff		17		//计算结果偏移
#define	c_TrayNumHOff		11		//盘号在杯头包内的偏移
#define	c_CupNumHOff		14		
#define	c_TrayNumEOff		31		
#define	c_CupNumEOff		34		//杯号在杯尾包内的偏移
#define	c_TrayNumLen		2		//盘号长度
#define	c_CupNumLen			2		//杯号长度
#define	c_CupHeaderPackLen	71		//杯头包长	
#define	c_TestResultPackLen 89		//结果包长
#define c_SPCalcuPackLen	33		//计算包长
#define	c_EndCupPackLen		41		//杯结束包长
#define	c_EndTrayPackLen	15		//盘结束包长

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
//监控串口接收数据，并完成数据格式分析，将结果放入结果队列
	CommData
	SYSTEMTIME SystemTime;
	int	 nItemCount,TrayNum,CupNum;//样本项目号,盘号,杯号
	BYTE  PackFun,SampleIDLen ;//数据包功能号,样本ID长度
	WORD	wSampleNo;
	LPSTR hRXBuffer;
	LPSTR lptemp1;
	bool blogfile;
	CHAR    TempWorkingID[c_MaxWorkingIDLength+1];//样本号,chj
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
		//上面函数用来定位包起始位在接收缓冲区中的位置.???
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
		
	//校验和的计算方法:checksum=256-((sum of character )modulo 256))
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
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);//填写工作单号
					OutResult[nItemCount].ItemNo=nItemCount+1;
					if (PackFun=='3')
					{
	
						FillItemName(nItemCount,lpOpPointer+c_ItemNameOff ,c_ItemNameLen)	
//填结果
						FillResult(nItemCount,lpOpPointer+c_ResultOff,c_ResultLen)

					}
					else	
					{
	
						FillItemName(nItemCount,lpOpPointer+c_CalcItemNameOff ,c_CalcItemNameLen)	

						FillResult(nItemCount,lpOpPointer+c_CalcResultOff,c_ResultLen)
					}
//填日期
					GetSystemTime(&SystemTime); 
					FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
					nItemCount++;
					break;	//继续接收	

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
	//设定串口监控事件
}

