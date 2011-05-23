// AU1000.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"

#define c_RxBufferLength	1024 //定义缓冲区大小		
#define	c_MaxItemNum		18  //定义项目数

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

	Init()
	ResetOpPointer()				

	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	TranCommChar(XON)
	while (TRUE)
	{


		WaitEvent(EV_RXFLAG)//等待接受数据包
		TranCommChar(XOFF)//中断仪器传送
		GetInQueueLength(dwReadLength)//分析数据
		ReadCommBlock(dwReadLength)
		//结果处理
		if(nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		nItemCount=0;	
		ResetRxPointer()		
		TranCommChar(XON)

	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
	//设定串口监控事件
}