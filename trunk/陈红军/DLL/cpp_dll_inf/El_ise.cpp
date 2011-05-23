//********************************************************************
//  本模块为Beckman公司的ELECTROLYTE SYSTEM(电介质分析仪)的联机程序(Win95版)
//  描述: 全称：SYNCHRON EL-ISE 
 
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>

#define c_RxBufferLength	800		//数据处理缓冲区长
#define	c_MaxItemNum		8		//一次最多处理的项目数
#define c_FuncNumOff		9		//包类别第二字节偏移
#define	c_ItemNameOff		17		//非计算项目名偏移
#define	c_ItemNameLen		3		
#define	c_StreamIDOff		4		
#define	c_CalcItemNameOff	27		
#define	c_CalcItemNameLen	1		
#define	c_SampleNoOff		11		
#define	c_SampleNoEOff		25		//样本号在杯尾包的偏移
#define	c_SampleNoLen		5		//样本号长度
#define	c_ResultOff			21		//非计算结果偏移
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


DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i,nItemCount,TrayNum,CupNum,PackLen;
	BYTE  PackFun ;
	WORD	wSampleNo;
	LPSTR hRXBuffer=(LPSTR)RxBuffer;

	Init()
	ResetOpPointer()				

	
	ResetRxPointer()
	

 	nItemCount =0;
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	WriteCommChar(XON)
	while (TRUE)
	{
	


		WaitEvent(EV_RXFLAG)


		GetInQueueLength(dwReadLength)
		ReadCommBlock(dwReadLength)
 


 

		PackFun= *(hRXBuffer+c_FuncNumOff);
		switch (PackFun)
		{
			case '1':		
				if (strncmp(hRXBuffer+c_StreamIDOff ,"102",3)==0)
				{


					hRXBuffer +=c_CupHeaderPackLen;
//				WriteCommChar(ACK)
					continue;
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
				hRXBuffer +=c_TestResultPackLen;
//				WriteCommChar(ACK)
				continue;//继续接收			
			case '5':	
				hRXBuffer +=c_SPCalcuPackLen;
//				WriteCommChar(ACK)
				continue;//继续接收	
			case '7':	
		
				if (*(hRXBuffer+c_FuncNumOff-1) =='0' && strncmp(hRXBuffer+c_StreamIDOff ,"102",3)==0)//-----A
				{
					GetLocalTime(&SystemTime);

					TrayNum=(int)StrToInt(hRXBuffer+c_TrayNumEOff,c_TrayNumLen);				
					CupNum=(int)StrToInt(hRXBuffer+c_CupNumEOff,c_CupNumLen);

					

				wSampleNo =(int)StrToInt(hRXBuffer+c_SampleNoEOff,c_SampleNoLen);

					PackLen=0;
					while (TRUE)//----B
					{
						PackFun= *(lpOpPointer+PackLen+c_FuncNumOff);
						switch (PackFun)//-----C
						{
							case '1':
								PackLen +=c_CupHeaderPackLen;
								continue;
							case '3':
							case '5':
//填样本号
								FillSampleID(nItemCount, wSampleNo)	
								if (PackFun=='3')
								{
				
									FillItemName(nItemCount,lpOpPointer+PackLen+c_ItemNameOff ,c_ItemNameLen)	
//填结果
									FillResult(nItemCount,lpOpPointer+PackLen+c_ResultOff,c_ResultLen)
									PackLen +=c_TestResultPackLen;
								}
								else	
								{
				
									FillItemName(nItemCount,lpOpPointer+PackLen+c_CalcItemNameOff ,c_CalcItemNameLen)	

									FillResult(nItemCount,lpOpPointer+PackLen+c_CalcResultOff,c_ResultLen)
									PackLen +=c_SPCalcuPackLen;
								}
//填日期
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

								nItemCount++;
								continue;
							case '7':
								break;
						}//-------end switch C
						break;
					}//-----end while B

					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);

					lpDevice->dwRecordCount+=nItemCount;  
					break;
				}
				else	
				{
					break;
				}
			case '9':	
				break;			
			default :	
				break;
		}
		nItemCount=0;
		hRXBuffer=(LPSTR)RxBuffer;
		ResetRxPointer()		
//		ResetOpPointer()
//		PurgeComm( hComm, PURGE_RXCLEAR );	
		WriteCommChar(ACK)
	}
	return TRUE;
} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}

