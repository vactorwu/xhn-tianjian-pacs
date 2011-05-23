//-----------------------------------------------------------------------//
//   SF3000.cpp : Defines the entry point for the DLL application.       //
//																		 //
//   开发者				时	间				备	注                       //
//  --------          -----------          --------                      //
//   余  浩            2001-02-08           开	发                       //
//																		 //
//	 说明：SF3000工作模式设定为Class A, SampleID 应设为 13digits         //
//         开发中参照了陈红军的强生250程序                               //
//-----------------------------------------------------------------------//

#include	<stdio.h>
#include	<stdlib.h>
#include	"stdafx.h"
#include	"Const.h"
#include	"Device.h"
#include	"DevMacro.h"
#include	"str.h"
#include	"LogFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		c_RxBufferLength	512	
#define		c_MaxItemNum		60	


//引用外部变量来自 LogFile.cpp
extern		HANDLE  hLogFile;


//---------------------项目结果的起始位置和长度值的设定-----------------------//
#define		c_SampleIDOff	32
#define		c_SampleIDLen	3
//1	(长度本为6字节，但最后一字节为标志位，故长度要少一位，下同)
#define		c_WBCOff		54
#define		c_WBCLen		5
//2
#define		c_RBCOff		60
#define		c_RBCLen		4
//3
#define		c_HGBOff		65
#define		c_HGBLen		4
//4
#define		c_HCTOff		70
#define		c_HCTLen		4
//5
#define		c_MCVOff		75
#define		c_MCVLen		4
//6
#define		c_MCHOff		80
#define		c_MCHLen		4
//7
#define		c_MCHCOff		85
#define		c_MCHCLen		4
//8
#define		c_PLTOff		90
#define		c_PLTLen		4
//9
#define		c_LYMPH100Off	95
#define		c_LYMPH100Len	4
//10
#define		c_MONO100Off	100
#define		c_MONO100Len	4
//11
#define		c_NEUT100Off	105
#define		c_NEUT100Len	4
//12
#define		c_EO100Off		110
#define		c_EO100Len		4
//13
#define		c_BASO100Off	115
#define		c_BASO100Len	4
//14
#define		c_LYMPH9Off		120
#define		c_LYMPH9Len		5
//15
#define		c_MONO9Off		126
#define		c_MONO9Len		5
//16
#define		c_NEUT9Off		132
#define		c_NEUT9Len		5
//17
#define		c_EO9Off		138
#define		c_EO9Len		5
//18
#define		c_BASO9Off		144
#define		c_BASO9Len		5
//19
#define		c_RDW_CVOff		150
#define		c_RDW_CVLen		4
//20
#define		c_RDW_SDOff		155
#define		c_RDW_SDLen		4
//21
#define		c_PDWOff		160
#define		c_PDWLen		4
//22
#define		c_MPVOff		165
#define		c_MPVLen		4
//23
#define		c_P_LCROff		170
#define		c_P_LCRLen		4
//------------------------------------------------------------------------//


//--------------------------动态库的主入口函数----------------------------//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//------------------------------------------------------------------------//


//-----------------------------串口监视程序-------------------------------//
DWORD WINAPI CommMonitor(LPSTR lpData)
{	 	
	char		TempWorkingID[c_MaxWorkingIDLength+1];
	char		DataBuffer[240];	 //结果数据缓冲区
	char		LSampleID[5];
	SYSTEMTIME	SystemTime;
	bool		bLogFile=FALSE;
	int			nItemCount;	
	int         SampleID;
	int			i,SampleIDLen,j;
	LPSTR		lpPointer;
	LPSTR		lpResult;
	bool		isdataPacket=FALSE;  //是否为数据包标识	
	
	CommData
	Init()
	nItemCount=0; //项目个数初始化为零
	ResetOpPointer()	
	if(NewLogFile("SF3000.txt"))
		bLogFile=TRUE;	
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG)

	while (TRUE)
	{	
		//监控串口接收数据，并完成数据格式分析，将结果放入结果队列		
		WaitEvent(EV_RXFLAG)//等待接受数据包EV_RXCHAR,EV_RXFLAG
		i=0;		
		do
		{
			ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	
			if(RxChar==STX)
				isdataPacket = TRUE;			
			if(isdataPacket)
			{
				DataBuffer[i]=RxChar;
				i++;							
			}
			if((RxChar==ETX)&&(isdataPacket==TRUE))
			{			
				DataBuffer[i]=RxChar;
				i++;
				break;
			}
		}while(dwReadLength==1);

		isdataPacket = FALSE;
		dwRxBufferLength = i; 		
		lpOpPointer=&DataBuffer[0]; 

		if (bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);
		}
		
		//数据格式分析
		if((lpOpPointer[1]==0x44)&&(lpOpPointer[2]==0x31))//两首字符'D'和'1'
		{					
			switch(*(lpOpPointer+3))//判断数据包类型
			{
			//结果数据包
			case 'U':			
				//取得工作单号				
				SampleID=StrToInt(lpOpPointer+c_SampleIDOff,c_SampleIDLen);
				_itoa(SampleID,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				//向工作单中填充‘0’，使工作单长度保持在6个字符
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
		    	TempWorkingID[6]='\0';	

				//第一个项目WBC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='W';
				OutResult[nItemCount].ItemID[1]='B';
				OutResult[nItemCount].ItemID[2]='C';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_WBCOff;
				FillResult(nItemCount, lpPointer , c_WBCLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第二个项目RBC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='R';
				OutResult[nItemCount].ItemID[1]='B';
				OutResult[nItemCount].ItemID[2]='C';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_RBCOff;
				FillResult(nItemCount, lpPointer , c_RBCLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',3);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//第三个项目HGB
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='H';
				OutResult[nItemCount].ItemID[1]='G';
				OutResult[nItemCount].ItemID[2]='B';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_HGBOff;
				FillResult(nItemCount, lpPointer , c_HGBLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第四个项目MCV
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='V';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCVOff;
				FillResult(nItemCount, lpPointer , c_MCVLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//第五个项目HCT
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='H';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='T';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_HCTOff;
				FillResult(nItemCount, lpPointer , c_HCTLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//第六个项目MCH
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='H';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCHOff;
				FillResult(nItemCount, lpPointer , c_MCHLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第七个项目MCHC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='H';
				OutResult[nItemCount].ItemID[3]='C';
				OutResult[nItemCount].ItemID[4]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCHCOff;
				FillResult(nItemCount, lpPointer , c_MCHCLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第八个项目PLT
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='P';
				OutResult[nItemCount].ItemID[1]='L';
				OutResult[nItemCount].ItemID[2]='T';
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_PLTOff;
				FillResult(nItemCount, lpPointer , c_PLTLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第九个项目LYMPH%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='L';
				OutResult[nItemCount].ItemID[1]='Y';
				OutResult[nItemCount].ItemID[2]='M';
				OutResult[nItemCount].ItemID[3]='P';
				OutResult[nItemCount].ItemID[4]='H';				
				OutResult[nItemCount].ItemID[5]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_LYMPH100Off;
				FillResult(nItemCount, lpPointer , c_LYMPH100Len) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//第十个项目MONO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='O';
				OutResult[nItemCount].ItemID[2]='N';
				OutResult[nItemCount].ItemID[3]='O';				
				OutResult[nItemCount].ItemID[4]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MONO100Off;
				FillResult(nItemCount, lpPointer , c_MONO100Len) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   	
				
				//第十一个项目NEUT%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='N';
				OutResult[nItemCount].ItemID[1]='E';
				OutResult[nItemCount].ItemID[2]='U';
				OutResult[nItemCount].ItemID[3]='T';				
				OutResult[nItemCount].ItemID[4]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_NEUT100Off;
				FillResult(nItemCount, lpPointer , c_NEUT100Len) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   	

				//第十二个项目EO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='E';
				OutResult[nItemCount].ItemID[1]='O';				
				OutResult[nItemCount].ItemID[2]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_EO100Off;
				FillResult(nItemCount, lpPointer , c_EO100Len) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//第十三个项目BASO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='B';
				OutResult[nItemCount].ItemID[1]='A';
				OutResult[nItemCount].ItemID[2]='S';
				OutResult[nItemCount].ItemID[3]='O';				
				OutResult[nItemCount].ItemID[4]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_BASO100Off;
				FillResult(nItemCount, lpPointer , c_BASO100Len) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;  				
				
				//第二十个项目RDW-SD
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='R';
				OutResult[nItemCount].ItemID[1]='D';
				OutResult[nItemCount].ItemID[2]='W';				
				OutResult[nItemCount].ItemID[3]='\0';
				//将当前结果记录的仪器结果数指针指向仪器结果数变量.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_RDW_SDOff;
				FillResult(nItemCount, lpPointer , c_RDW_SDLen) 
				//对结果再处理(添加小数点,去掉前面的0字符)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				
				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				
				//应答				
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
				nItemCount=0;				
				break;

			//质控数据包
			case 'C':	
				//应答				
				PurgeComm(hComm, PURGE_RXCLEAR);	
				WriteCommChar(ACK)
				nItemCount=0;
				break;			
			}			
		}			
	}
	if(bLogFile)
	{
		CloseLogFile(hLogFile);
	}
	return TRUE;	
} 
//------------------------------------------------------------------------//


//-----------------------------设定事件字符-------------------------------//
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	 pComDcb->EvtChar = ETX;
	//设定串口监控事件
}
//------------------------------------------------------------------------//