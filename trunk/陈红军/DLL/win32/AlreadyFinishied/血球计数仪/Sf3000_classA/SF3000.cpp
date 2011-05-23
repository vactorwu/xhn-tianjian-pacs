//-----------------------------------------------------------------------//
//   SF3000.cpp : Defines the entry point for the DLL application.       //
//																		 //
//   ������				ʱ	��				��	ע                       //
//  --------          -----------          --------                      //
//   ��  ��            2001-02-08           ��	��                       //
//																		 //
//	 ˵����SF3000����ģʽ�趨ΪClass A, SampleID Ӧ��Ϊ 13digits         //
//         �����в����˳º����ǿ��250����                               //
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


//�����ⲿ�������� LogFile.cpp
extern		HANDLE  hLogFile;


//---------------------��Ŀ�������ʼλ�úͳ���ֵ���趨-----------------------//
#define		c_SampleIDOff	32
#define		c_SampleIDLen	3
//1	(���ȱ�Ϊ6�ֽڣ������һ�ֽ�Ϊ��־λ���ʳ���Ҫ��һλ����ͬ)
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


//--------------------------��̬�������ں���----------------------------//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//------------------------------------------------------------------------//


//-----------------------------���ڼ��ӳ���-------------------------------//
DWORD WINAPI CommMonitor(LPSTR lpData)
{	 	
	char		TempWorkingID[c_MaxWorkingIDLength+1];
	char		DataBuffer[240];	 //������ݻ�����
	char		LSampleID[5];
	SYSTEMTIME	SystemTime;
	bool		bLogFile=FALSE;
	int			nItemCount;	
	int         SampleID;
	int			i,SampleIDLen,j;
	LPSTR		lpPointer;
	LPSTR		lpResult;
	bool		isdataPacket=FALSE;  //�Ƿ�Ϊ���ݰ���ʶ	
	
	CommData
	Init()
	nItemCount=0; //��Ŀ������ʼ��Ϊ��
	ResetOpPointer()	
	if(NewLogFile("SF3000.txt"))
		bLogFile=TRUE;
	EscapeCommFunction(hComm,SETRTS);
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG)

	while (TRUE)
	{	
		//��ش��ڽ������ݣ���������ݸ�ʽ���������������������		
		WaitEvent(EV_RXFLAG)//�ȴ��������ݰ�EV_RXCHAR
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
				EscapeCommFunction(hComm,CLRRTS);
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
		
		//���ݸ�ʽ����
		if((lpOpPointer[1]==0x44)&&(lpOpPointer[2]==0x31))//�����ַ�'D'��'1'
		{					
			switch(*(lpOpPointer+3))//�ж����ݰ�����
			{
			//������ݰ�
			case 'U':			
				//ȡ�ù�������				
				SampleID=StrToInt(lpOpPointer+c_SampleIDOff,c_SampleIDLen);
				_itoa(SampleID,LSampleID,10);
				TempWorkingID[0]=lpDevice->DevItemType;
				//����������䡮0����ʹ���������ȱ�����6���ַ�
				SampleIDLen=strlen(LSampleID);
				memset(&TempWorkingID[1],0x30,5);
				strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
		    	TempWorkingID[6]='\0';	

				//��һ����ĿWBC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='W';
				OutResult[nItemCount].ItemID[1]='B';
				OutResult[nItemCount].ItemID[2]='C';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_WBCOff;
				FillResult(nItemCount, lpPointer , c_WBCLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//�ڶ�����ĿRBC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='R';
				OutResult[nItemCount].ItemID[1]='B';
				OutResult[nItemCount].ItemID[2]='C';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_RBCOff;
				FillResult(nItemCount, lpPointer , c_RBCLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',3);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//��������ĿHGB
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='H';
				OutResult[nItemCount].ItemID[1]='G';
				OutResult[nItemCount].ItemID[2]='B';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_HGBOff;
				FillResult(nItemCount, lpPointer , c_HGBLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//���ĸ���ĿMCV
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='V';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCVOff;
				FillResult(nItemCount, lpPointer , c_MCVLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//�������ĿHCT
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='H';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='T';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_HCTOff;
				FillResult(nItemCount, lpPointer , c_HCTLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   
				
				//��������ĿMCH
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='H';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCHOff;
				FillResult(nItemCount, lpPointer , c_MCHLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//���߸���ĿMCHC
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='H';
				OutResult[nItemCount].ItemID[3]='C';
				OutResult[nItemCount].ItemID[4]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MCHCOff;
				FillResult(nItemCount, lpPointer , c_MCHCLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//�ڰ˸���ĿPLT
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='P';
				OutResult[nItemCount].ItemID[1]='L';
				OutResult[nItemCount].ItemID[2]='T';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_PLTOff;
				FillResult(nItemCount, lpPointer , c_PLTLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',5);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//�ھŸ���ĿLYMPH%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='L';
				OutResult[nItemCount].ItemID[1]='Y';
				OutResult[nItemCount].ItemID[2]='M';
				OutResult[nItemCount].ItemID[3]='P';
				OutResult[nItemCount].ItemID[4]='H';				
				OutResult[nItemCount].ItemID[5]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_LYMPH100Off;
				FillResult(nItemCount, lpPointer , c_LYMPH100Len) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   

				//��ʮ����ĿMONO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='O';
				OutResult[nItemCount].ItemID[2]='N';
				OutResult[nItemCount].ItemID[3]='O';				
				OutResult[nItemCount].ItemID[4]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MONO100Off;
				FillResult(nItemCount, lpPointer , c_MONO100Len) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   	
				
				//��ʮһ����ĿNEUT%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='N';
				OutResult[nItemCount].ItemID[1]='E';
				OutResult[nItemCount].ItemID[2]='U';
				OutResult[nItemCount].ItemID[3]='T';				
				OutResult[nItemCount].ItemID[4]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_NEUT100Off;
				FillResult(nItemCount, lpPointer , c_NEUT100Len) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;   	

				//��ʮ������ĿEO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='E';
				OutResult[nItemCount].ItemID[1]='O';				
				OutResult[nItemCount].ItemID[2]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_EO100Off;
				FillResult(nItemCount, lpPointer , c_EO100Len) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//��ʮ������ĿBASO%
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='B';
				OutResult[nItemCount].ItemID[1]='A';
				OutResult[nItemCount].ItemID[2]='S';
				OutResult[nItemCount].ItemID[3]='O';				
				OutResult[nItemCount].ItemID[4]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_BASO100Off;
				FillResult(nItemCount, lpPointer , c_BASO100Len) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',2);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;  

				/*
				//��ʮ�ĸ���ĿLYMPH#
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='L';
				OutResult[nItemCount].ItemID[1]='Y';
				OutResult[nItemCount].ItemID[2]='M';
				OutResult[nItemCount].ItemID[3]='P';
				OutResult[nItemCount].ItemID[4]='H';
				OutResult[nItemCount].ItemID[5]='#';
				OutResult[nItemCount].ItemID[6]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_LYMPH9Off;
				FillResult(nItemCount, lpPointer , c_LYMPH9Len) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//��ʮ�����ĿMONO#
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='O';
				OutResult[nItemCount].ItemID[2]='N';
				OutResult[nItemCount].ItemID[3]='O';
				OutResult[nItemCount].ItemID[4]='#';
				OutResult[nItemCount].ItemID[5]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MONO9Off;
				FillResult(nItemCount, lpPointer , c_MONO9Len) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				
				//��ʮ������ĿNEUT#
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='N';
				OutResult[nItemCount].ItemID[1]='E';
				OutResult[nItemCount].ItemID[2]='U';
				OutResult[nItemCount].ItemID[3]='T';
				OutResult[nItemCount].ItemID[4]='#';
				OutResult[nItemCount].ItemID[5]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_NEUT9Off;
				FillResult(nItemCount, lpPointer , c_NEUT9Len) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//��ʮ�߸���ĿEO#
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='E';
				OutResult[nItemCount].ItemID[1]='O';
				OutResult[nItemCount].ItemID[2]='#';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_EO9Off;
				FillResult(nItemCount, lpPointer , c_EO9Len) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//��ʮ�˸���ĿBASO#
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='B';
				OutResult[nItemCount].ItemID[1]='A';
				OutResult[nItemCount].ItemID[2]='S';
				OutResult[nItemCount].ItemID[3]='O';
				OutResult[nItemCount].ItemID[4]='#';
				OutResult[nItemCount].ItemID[5]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_BASO9Off;
				FillResult(nItemCount, lpPointer , c_BASO9Len) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				
				
				//��ʮ�Ÿ���ĿRDW-CV
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='R';
				OutResult[nItemCount].ItemID[1]='D';
				OutResult[nItemCount].ItemID[2]='W';
				OutResult[nItemCount].ItemID[3]='-';
				OutResult[nItemCount].ItemID[4]='C';
				OutResult[nItemCount].ItemID[5]='V';
				OutResult[nItemCount].ItemID[6]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_RDW_CVOff;
				FillResult(nItemCount, lpPointer , c_RDW_CVLen) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				*/
				
				
				//�ڶ�ʮ����ĿRDW-SD
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='R';
				OutResult[nItemCount].ItemID[1]='D';
				OutResult[nItemCount].ItemID[2]='W';				
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_RDW_SDOff;
				FillResult(nItemCount, lpPointer , c_RDW_SDLen) 
				//�Խ���ٴ���(���С����,ȥ��ǰ���0�ַ�)				
				lpResult = InsertStr(OutResult[nItemCount].AssayResult,'.',4);			
				j=0;
				while((OutResult[nItemCount].AssayResult[j]=*(lpResult+j))!='\0') j++;
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				
				/*
				//�ڶ�ʮһ����ĿPDW
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='P';
				OutResult[nItemCount].ItemID[1]='D';
				OutResult[nItemCount].ItemID[2]='W';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_PDWOff;
				FillResult(nItemCount, lpPointer , c_PDWLen) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//�ڶ�ʮ������ĿMPV
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='M';
				OutResult[nItemCount].ItemID[1]='P';
				OutResult[nItemCount].ItemID[2]='V';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_MPVOff;
				FillResult(nItemCount, lpPointer , c_MPVLen) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;

				//�ڶ�ʮ������ĿLCR
				OutResult[nItemCount].ItemNo=nItemCount+1;
				OutResult[nItemCount].ItemID[0]='L';
				OutResult[nItemCount].ItemID[1]='C';
				OutResult[nItemCount].ItemID[2]='R';
				OutResult[nItemCount].ItemID[3]='\0';
				//����ǰ�����¼�����������ָ��ָ���������������.
				OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);
				lpPointer=lpOpPointer+c_P_LCROff;
				FillResult(nItemCount, lpPointer , c_P_LCRLen) 
				strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);
				GetSystemTime(&SystemTime);
				FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
 				nItemCount++;
				*/
				if(nItemCount>0)
				{
					(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
					lpDevice->dwRecordCount+=nItemCount;
				}
				EscapeCommFunction(hComm,SETRTS);
				DataBuffer[0]='\0';
				nItemCount=0;				
				break;

			//�ʿ����ݰ�
			case 'C':				
				EscapeCommFunction(hComm,SETRTS);
				DataBuffer[0]='\0';
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


//-----------------------------�趨�¼��ַ�-------------------------------//
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	 pComDcb->EvtChar = ETX;
	//�趨���ڼ���¼�
}
//------------------------------------------------------------------------//