// F100.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "logfile.h"

#define c_RxBufferLength	30		  
#define	c_MaxItemNum		12

#define c_QueryBlockLegth 7
#define c_ScanValueOff    2

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
	
	BYTE  QueryArray[7]={0xc3,0xc3,2,1,0,0,0x89};//查询是否有结果可以发送。
	BYTE  QueryRec[7]={0xc3,0xc3,0x2,0,0,0,0};//查询相关记录的结果
	char  TempWorkingID[7],SampleID[6];//临时工作单号
	int nItemCount ,nCheckSum,i,ColorBlockValue,TestPaperType,High,Low,SampleIDLen,PrevSample;
	bool bLogFile;
	CommData
	Init()
	ResetOpPointer()				
	FillDevName()	
	nItemCount=PrevSample=0;			
	SetCommWaitMask(EV_RXCHAR)
	PurgeComm( hComm, PURGE_RXCLEAR );	
	memset(lpOpPointer , 0, c_RxBufferLength );
	bLogFile=NewLogFile("F100.txt");
	while (TRUE)
	{
		Sleep(20000);
	//	MessageBox(NULL,"1","NOTIFY",MB_OK);
		WriteCommBlock(&QueryArray[0],7)
		WaitEvent(EV_RXCHAR)
		//休眠20S
		FetchCommBlock(c_QueryBlockLegth-1)
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,6);
		}
		if((*(lpOpPointer+3)==0) & (*(lpOpPointer+4)==0))
		{
			//仪器忙，等待一段时间后在查询结果信息。
		//	MessageBox(NULL,"2","NOTIFY",MB_OK);
			Sleep(40000);//休眠20S
			ClearCommRx()//清空接受缓冲区
			ResetOpPointer()
			PurgeComm( hComm, PURGE_TXCLEAR );
			continue;
		}
		else
		{
			
			//结果处理
			/*
			1.C3H,CH3,02H,01H,00H,00H，CHECKSUM           －－－ 查询结果
			2.68H,68H,02H,LOW,HIGH,CHECKSUM               －－－ 仪器回码
			3.C3H,C3H,02H,00H,LOWPOINT,HIPOINT,CHECKSUM   －－－ 索取指定记录的内容
			4.68H，68H,02H,SCANVALUE(20byte),CHECKSUM     －－－ 仪器回码（指定记录的内容）			
			*/

			nCheckSum=0;
			wSampleID=(*(lpOpPointer+4))*256+(*(lpOpPointer+3));//计算工作单号
			if(PrevSample!=wSampleID)//
			{
			//	MessageBox(NULL,"3","NOTIFY",MB_OK);
				TempWorkingID[0]=lpDevice->DevItemType;
				_itoa(wSampleID,SampleID,10);
				SampleIDLen=strlen(SampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&SampleID[0],SampleIDLen);
				TempWorkingID[6]='\0';
				//准备查询结果。
				QueryRec[4]=*(lpOpPointer+3)-1;
				QueryRec[5]=*(lpOpPointer+4);	
				for(i=0;i<6;i++)
				{
					nCheckSum=(nCheckSum+QueryRec[i])%256;
				}
				QueryRec[6]=nCheckSum;
				ClearCommRx()//清空接受缓冲区
				PurgeComm( hComm, PURGE_RXCLEAR ) ;
				ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);
				PurgeComm( hComm, PURGE_TXCLEAR );
				WriteCommBlock(&QueryRec[0],7)
				WaitEvent(EV_RXCHAR)
				for(i=0;i<24;i++)
				{
					ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);
					*(lpOpPointer+i)=RxChar;
				}
			//	FetchCommBlock(24)
				if(bLogFile)
				{
					WriteLogData(hLogFile,lpOpPointer,24);
				}

				//进行结果处理，根据试纸类型不同，有不同的处理方法。
				//F11，F10，F8：
				TestPaperType=*(lpOpPointer+c_ScanValueOff+10);
				//中间有两个字节未用。
					/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
				ColorBlockValue=*(lpOpPointer+c_ScanValueOff+13);//******色块1、2**********
				High=ColorBlockValue/16;//高四位
				Low=ColorBlockValue&0x0f;//低四位
				strncpy(OutResult[nItemCount].ItemID,"URO",3);
				OutResult[nItemCount].ItemID[3]='\0';
				switch(Low)//1
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "+ -  3.3 u mol/L");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "+ -  16  u mol/L");
					break;
				case 2:
					strcpy(OutResult[nItemCount].AssayResult, "+    33  u mol/L");
					break;
				case 3:
					strcpy(OutResult[nItemCount].AssayResult, "+ +  66  u mol/L");
					break;
				case 4:
					strcpy(OutResult[nItemCount].AssayResult, "+++  130 u mol/L");
					break;
				default:
					strcpy(OutResult[nItemCount].AssayResult, "Err");
					break;
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
				/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
				ColorBlockValue=*(lpOpPointer+c_ScanValueOff+14);//******色块3、4*********
				High=ColorBlockValue/16;
				Low=ColorBlockValue&0x0f;
				strncpy(OutResult[nItemCount].ItemID,"BIL",3);
				switch(High)//2
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "neg");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "+    17  u mol/L");
					break;
				case 2:
					strcpy(OutResult[nItemCount].AssayResult, "++   50  u mol/L");
					break;
				case 3:
					strcpy(OutResult[nItemCount].AssayResult, "+++  100  u mol/L");
					break;
				default:
					strcpy(OutResult[nItemCount].AssayResult, "Err");
					break;
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
				strncpy(OutResult[nItemCount].ItemID,"KET",3);
				OutResult[nItemCount].ItemID[3]='\0';
				switch(Low)//2
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "neg");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "+ -  0.5   mmol/L");
					break;
				case 2:
					strcpy(OutResult[nItemCount].AssayResult, "+    1.5   mmol/L");
					break;
				case 3:
					strcpy(OutResult[nItemCount].AssayResult, "+ +  3.9   mmol/L");
					break;
				case 4:
					strcpy(OutResult[nItemCount].AssayResult, "+++  7.8   mmol/L");
					break;
				default:
					strcpy(OutResult[nItemCount].AssayResult, "Err");
					break;
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
				/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
				ColorBlockValue=*(lpOpPointer+c_ScanValueOff+15);//*****色块5、6*******
				High=ColorBlockValue/16;
				Low=ColorBlockValue&0x0f;
				strncpy(OutResult[nItemCount].ItemID,"BLD",3);
				OutResult[nItemCount].ItemID[3]='\0';
				switch(High)//3
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "neg");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "+ -  10  cells/ul");
					break;
				case 2:
					strcpy(OutResult[nItemCount].AssayResult, "+    25  cells/ul");
					break;
				case 3:
					strcpy(OutResult[nItemCount].AssayResult, "+ +  80  cells/ul");
					break;
				case 4:
					strcpy(OutResult[nItemCount].AssayResult, "+++  200 cells/ul");
					break;
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
				strncpy(OutResult[nItemCount].ItemID,"PRO",3);
				OutResult[nItemCount].ItemID[3]='\0';
				switch(Low)//4
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "neg");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "+ -");
					break;
				case 2:
					strcpy(OutResult[nItemCount].AssayResult, "+    0.3      g/L");
					break;
				case 3:
					strcpy(OutResult[nItemCount].AssayResult, "+ +  1.0      g/L");
					break;
				case 4:
					strcpy(OutResult[nItemCount].AssayResult, "+++  3.0      g/L");
					break;
				case 5:
					strcpy(OutResult[nItemCount].AssayResult, "++++ 10       g/L");
					break;
				default:
					strcpy(OutResult[nItemCount].AssayResult, "Err");
					break;
				}
				OutResult[nItemCount].ItemNo=nItemCount+1;
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				nItemCount++;
					/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
				ColorBlockValue=*(lpOpPointer+c_ScanValueOff+16);//******色块7、8******
				High=ColorBlockValue/16;
				Low=ColorBlockValue&0x0f;
				strncpy(OutResult[nItemCount].ItemID,"NIT",3);
				OutResult[nItemCount].ItemID[3]='\0';
				switch(High)//5
				{
				case 0:
					strcpy(OutResult[nItemCount].AssayResult, "neg");
					break;
				case 1:
					strcpy(OutResult[nItemCount].AssayResult, "pos");
					break;
				default:
					strcpy(OutResult[nItemCount].AssayResult, "Err");
					break;
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
				if(TestPaperType==0||TestPaperType==1)//试纸类型为F11，F10
				{
					strncpy(OutResult[nItemCount].ItemID,"LEU",3);
					OutResult[nItemCount].ItemID[3]='\0';
					switch(Low)//5
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "neg");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "+ -  15.0  cells/uL");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "+    70.0  cells/uL");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "+ +  125   cells/uL");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "+++  500 cells/uL");
						break;
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
				}
				else//试纸类型为F8
				{

					strncpy(OutResult[nItemCount].ItemID,"GLU",3);
					OutResult[nItemCount].ItemID[3]='\0';
					switch(Low)//5
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "neg");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "+ -");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "+    5.5 mmol/L");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "+ +   14 mmol/L");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "+++   28 mmol/L");
						break;
					case 5:
						strcpy(OutResult[nItemCount].AssayResult, "++++  55 mmol/L");
						break;
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
				}
				strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
				OutResult[nItemCount].ItemNo=nItemCount+1;
				nItemCount++;
					/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
				ColorBlockValue=*(lpOpPointer+c_ScanValueOff+17);//*******色块9、10**********
				High=ColorBlockValue/16;
				Low=ColorBlockValue&0x0f;
				switch(TestPaperType)
				{
				case 0://试纸类型为F11   －－色块9
				case 1://试纸类型为F10   －－色块9
					strncpy(OutResult[nItemCount].ItemID,"GLU",3);
					OutResult[nItemCount].ItemID[3]='\0';
					switch(High)//6
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "neg");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "+ -");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "+    5.5 mmol/L");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "+ +   14 mmol/L");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "+++   28 mmol/L");
						break;
					case 5:
						strcpy(OutResult[nItemCount].AssayResult, "++++  55 mmol/L");
						break;
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
					OutResult[nItemCount].ItemNo=nItemCount+1;
					nItemCount++;
					strncpy(OutResult[nItemCount].ItemID,"SG",2);
					OutResult[nItemCount].ItemID[2]='\0';
					switch(Low)//   －－色块10
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "1.030");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "1.025");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "1.020");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "1.015");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "1.010");
						break;
					case 5:
						strcpy(OutResult[nItemCount].AssayResult, "1.005");
						break;
					case 6:
						strcpy(OutResult[nItemCount].AssayResult, "1.005");
						break;
					case 7:
						strcpy(OutResult[nItemCount].AssayResult, "1.000");
						break;
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
					OutResult[nItemCount].ItemNo=nItemCount+1;
					nItemCount++;
					/*－－－－－－－－－－－－－－－－－－－－－－－－－－－*/
					ColorBlockValue=*(lpOpPointer+c_ScanValueOff+18);//********色块11、12*********
					High=ColorBlockValue/16;
					Low=ColorBlockValue&0x0f;
					strncpy(OutResult[nItemCount].ItemID,"PH",2);
					OutResult[nItemCount].ItemID[2]='\0';
					switch(High)//7
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "4.5");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "5.0");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "5.5");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "6.0");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "6.5");
					case 5:
						strcpy(OutResult[nItemCount].AssayResult, "7.0");
						break;
					case 6:
						strcpy(OutResult[nItemCount].AssayResult, "7.0");
						break;
					case 7:
						strcpy(OutResult[nItemCount].AssayResult, "7.5");
						break;
					case 8:
						strcpy(OutResult[nItemCount].AssayResult, "8.0");
						break;
					case 9:
						strcpy(OutResult[nItemCount].AssayResult, "8.5");
					case 10:
						strcpy(OutResult[nItemCount].AssayResult, "9.0");
						break;		
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
					OutResult[nItemCount].ItemNo=nItemCount+1;
					nItemCount++;
					if(TestPaperType==0)
					{
						strncpy(OutResult[nItemCount].ItemID,"VC",2);
						OutResult[nItemCount].ItemID[2]='\0';
						switch(Low)
						{
						case 0:
							strcpy(OutResult[nItemCount].AssayResult, "     5.0   mmol/L");
							break;		
						case 1:
							strcpy(OutResult[nItemCount].AssayResult, "     2.8   mmol/L");
							break;
						case 2:
							strcpy(OutResult[nItemCount].AssayResult, "     1.4   mmol/L");
							break;
						case 3:
							strcpy(OutResult[nItemCount].AssayResult, "     0.6   mmol/L");
							break;
						case 4:
							strcpy(OutResult[nItemCount].AssayResult, "     0     mmol/L");
							break;		
						default:
							strcpy(OutResult[nItemCount].AssayResult, "Err");
							break;
						}
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
					OutResult[nItemCount].ItemNo=nItemCount+1;
					nItemCount++;
					}
				
					break;
				case 2:	//试纸类型为F8
					strncpy(OutResult[nItemCount].ItemID,"PH",2);
					OutResult[nItemCount].ItemID[2]='\0';
					switch(High)//7
					{
					case 0:
						strcpy(OutResult[nItemCount].AssayResult, "4.5");
						break;
					case 1:
						strcpy(OutResult[nItemCount].AssayResult, "5.0");
						break;
					case 2:
						strcpy(OutResult[nItemCount].AssayResult, "5.5");
						break;
					case 3:
						strcpy(OutResult[nItemCount].AssayResult, "6.0");
						break;
					case 4:
						strcpy(OutResult[nItemCount].AssayResult, "6.5");
					case 5:
						strcpy(OutResult[nItemCount].AssayResult, "7.0");
						break;
					case 6:
						strcpy(OutResult[nItemCount].AssayResult, "7.0");
						break;
					case 7:
						strcpy(OutResult[nItemCount].AssayResult, "7.5");
						break;
					case 8:
						strcpy(OutResult[nItemCount].AssayResult, "8.0");
						break;
					case 9:
						strcpy(OutResult[nItemCount].AssayResult, "8.5");
					case 10:
						strcpy(OutResult[nItemCount].AssayResult, "9.0");
						break;		
					default:
						strcpy(OutResult[nItemCount].AssayResult, "Err");
						break;
					}
					strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,c_MaxWorkingIDLength+1);//添工作单
					OutResult[nItemCount].ItemNo=nItemCount+1;
					nItemCount++;
					break;
				}

				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				nItemCount=0;	
				ResetRxPointer()
				PrevSample=wSampleID;
			}//PrevSample==wSampleID;
			else
			{
		//		MessageBox(NULL,"4","NOTIFY",MB_OK);
				ResetRxPointer()
				ClearCommRx()//清空接受缓冲区
				PurgeComm( hComm, PURGE_TXCLEAR );
			}
		}

	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;
}  // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
	//设定串口监控事件
}