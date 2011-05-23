// clinitek_1.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
//********************************************************************
//  ģ��:  Clinitek.cpp
//  ����: ��ģ��ΪClinitek�����ǵ���������(Win95��)
//	����:
  
#include "stdio.h"
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength	512 //���建������С		
#define	c_MaxItemNum		10  //������Ŀ��������ʮ��Ϊ10		 
 
#define c_SampleNoOff		6   //�������ݰ�����������������ݰ���ʼλ�õ�ƫ����
#define c_SampleNoLen		3   //���������ų���

#define c_OneDataLen		20 //����ÿ����¼�ĳ���

#define c_DataOff           26 //����������������ݰ���ʼλ�õĵ�ƫ����
#define c_ItemIDOff			0  //���������Ŀ��������ڽ�����λ����		
#define c_ItemIDLen			3  //�����������ƴ�С
#define c_ResultOff			6  //�������������ڽ�����ƫ����	
#define c_ResultLen			12 //�����������С


static AFX_EXTENSION_MODULE Clinitek_1DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CLINITEK_1.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(Clinitek_1DLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(Clinitek_1DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CLINITEK_1.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(Clinitek_1DLL);
	}
	return 1;   // ok
}

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nDataOff
		
		;
	BYTE  btCalCheckSum, btRxCheckSum;
	WORD 		wSampleNo;						
 	BYTE		btResultType;

	Init()
	nItemCount =0;
	ResetOpPointer()				
	FillDevName()					
	PurgeComm( hComm, PURGE_RXCLEAR );
	SetCommWaitMask(EV_RXFLAG)

	
	while (TRUE)	
	{
//      ��ʼ�����ݽ��յ�������
		ResetRxPointer()	
		do
		{
			GetInQueueLength(dwReadLength)
			if(dwReadLength==0 && dwRxBufferLength==0)
			{
				WaitEvent(EV_RXFLAG)  
				GetInQueueLength(dwReadLength)
			}
 
			ReadCommBlock(dwReadLength)	
			if ( dwRxBufferLength>=2)
			{
				if( (*(lpRxPointer-1)==ETX))
  				break;
			}
				
		}while(TRUE);				

        
//      ����У��
		btCalCheckSum =lpOpPointer[1];
		
		for(i=2;lpOpPointer[i+2]!=ETX;i++)     //�ӵ�2λ��������ǰ��λ�ַ���ӣ���У���
		{
			btCalCheckSum +=lpOpPointer[i];
		}

		if (btCalCheckSum!=0)
		{
			if (lpOpPointer[i]<=0x3A && lpOpPointer[i+1]<=0x3A)
			{
				btRxCheckSum =(lpOpPointer[i]-0x30)*16+lpOpPointer[i+1]-0x30;
			}
			if (lpOpPointer[i]>0x3A && lpOpPointer[i+1]<=0x3A)
            {
				btRxCheckSum =(lpOpPointer[i]-0x37)*16+lpOpPointer[i+1]-0x30;
			}
            if (lpOpPointer[i]<=0x3A && lpOpPointer[i+1]>0x3A)
            {
				btRxCheckSum =(lpOpPointer[i]-0x30)*16+lpOpPointer[i+1]-0x37;
			}
            if (lpOpPointer[i]>0x3A && lpOpPointer[i+1]>0x3A)
            {
				btRxCheckSum =(lpOpPointer[i]-0x37)*16+lpOpPointer[i+1]-0x37;
			}
			if (btRxCheckSum!=btCalCheckSum)
			{
				AfxMessageBox("���ݽ��մ���������!");
				PurgeComm( hComm, PURGE_RXCLEAR );		
				continue;
			}
			
		}

//      ������ת�浽���������
		nItemCount =0;

 		GetLocalTime(&SystemTime);
		wSampleNo =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
		btResultType =c_TypeSample;
				
					
		for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff+2)!=ETX  ;)
		{
			FillSampleID(nItemCount, wSampleNo)	
			FillItemName(nItemCount, lpOpPointer+nDataOff+c_ItemIDOff, c_ItemIDLen)	//��Ŀ��
				
			FillResult(nItemCount,  lpOpPointer+nDataOff+c_ResultOff,c_ResultLen )

			FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

			nItemCount++;
			nDataOff+=c_OneDataLen;
		}
//      ���ý����������		
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		lpDevice->dwRecordCount+=nItemCount;
	
//      ������һ�����ݰ�
		ResetRxPointer()
		PurgeComm( hComm, PURGE_RXCLEAR );

  }

	return TRUE;

} 

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =ETX;
}

