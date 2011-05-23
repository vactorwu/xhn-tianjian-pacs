// coulter.cpp : Defines the initialization routines for the DLL.
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
//  模块:  coulter.cpp
//  描述: 本模块为coulter化验仪的联机程序(Win95版)
//	函数:
  
#include "stdio.h"
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"

#define c_RxBufferLength	1024 //定义缓冲区大小		
#define	c_MaxItemNum		18  //定义项目数	 
 

#define One_Package_Length 389

static AFX_EXTENSION_MODULE CoulterDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("COULTER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(CoulterDLL, hInstance))
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

		new CDynLinkLibrary(CoulterDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("COULTER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(CoulterDLL);
	}
	return 1;   // ok
}
DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData
	SYSTEMTIME SystemTime;
    int	nItemCount;

//	BYTE  btCalCheckSum, btRxCheckSum;
	WORD 		wSampleNo;						
	BYTE		btResultType;

	Init()
	
	ResetOpPointer()				
	FillDevName()					
	SetCommWaitMask(EV_RXFLAG)

	
	while (TRUE)	
	{
        PurgeComm( hComm, PURGE_RXCLEAR );
		WaitEvent(EV_RXFLAG)
        
		while(TRUE)
		{
            ResetRxPointer()
		   	ReadCommBlock(One_Package_Length)

			//读取一个数据包，
			if(dwReadLength!=One_Package_Length)
				break;
   
/*			//数据校验
		    btCalCheckSum =lpOpPointer[0];
		
		    for(i=1;i<=One_Package_Length-2;i++)     //从第1位到结束符前两位字符相加，求校验和
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
				AfxMessageBox("数据接收错误，请重做!");
				break;
				}
			}
*/			

           //将数据转存到结果数组中
		    nItemCount =0;

 		    GetLocalTime(&SystemTime);
		    wSampleNo =StrToInt(lpOpPointer+53, 4);
		    btResultType =c_TypeSample;
			
		    FillSampleID(nItemCount, wSampleNo)	//WBC
		    FillItemName(nItemCount, lpOpPointer+91, 3)	
		    FillResult(nItemCount,  lpOpPointer+97,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
		    nItemCount++;
			
		    FillSampleID(nItemCount, wSampleNo)	//RBC
		    FillItemName(nItemCount, lpOpPointer+107, 3)
		    FillResult(nItemCount,  lpOpPointer+113,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//HGB
		    FillItemName(nItemCount, lpOpPointer+123, 3)
		    FillResult(nItemCount,  lpOpPointer+129,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//HCT
		    FillItemName(nItemCount, lpOpPointer+139, 3)
		    FillResult(nItemCount,  lpOpPointer+145,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//MCV
		    FillItemName(nItemCount, lpOpPointer+155, 3)
		    FillResult(nItemCount,  lpOpPointer+161,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//MCH
		    FillItemName(nItemCount, lpOpPointer+171, 3)
		    FillResult(nItemCount,  lpOpPointer+177,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//MCHC
		    FillItemName(nItemCount, lpOpPointer+187, 4)
		    FillResult(nItemCount,  lpOpPointer+193,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//RDW
		    FillItemName(nItemCount, lpOpPointer+203, 3)
	 	    FillResult(nItemCount,  lpOpPointer+209,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

            FillSampleID(nItemCount, wSampleNo)	//PLT
		    FillItemName(nItemCount, lpOpPointer+219, 3)
		    FillResult(nItemCount,  lpOpPointer+225,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;
		
            FillSampleID(nItemCount, wSampleNo)	//PCT
		    FillItemName(nItemCount, lpOpPointer+235, 3)
		    FillResult(nItemCount,  lpOpPointer+241,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//MPV
		    FillItemName(nItemCount, lpOpPointer+251, 3)
		    FillResult(nItemCount,  lpOpPointer+257,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;
		
		    FillSampleID(nItemCount, wSampleNo)	//PDW
		    FillItemName(nItemCount, lpOpPointer+267, 3)
		    FillResult(nItemCount,  lpOpPointer+273,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//LYB%
		    FillItemName(nItemCount, lpOpPointer+283, 4)
		    FillResult(nItemCount,  lpOpPointer+289,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//MO%
		    FillItemName(nItemCount, lpOpPointer+299, 4)
		    FillResult(nItemCount,  lpOpPointer+305,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//GRB%
		    FillItemName(nItemCount, lpOpPointer+315, 4)
		    FillResult(nItemCount,  lpOpPointer+321,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//LYB#
		    FillItemName(nItemCount, lpOpPointer+331, 4)
		    FillResult(nItemCount,  lpOpPointer+337,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;

		    FillSampleID(nItemCount, wSampleNo)	//MO#
		    FillItemName(nItemCount, lpOpPointer+347, 3)
		    FillResult(nItemCount,  lpOpPointer+353,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;		

		    FillSampleID(nItemCount, wSampleNo)	//GRB#
		    FillItemName(nItemCount, lpOpPointer+363, 3)
		    FillResult(nItemCount,  lpOpPointer+369,6 )
		    FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
            nItemCount++;
		
            //调用结果处理例程		
		    (*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
		    lpDevice->dwRecordCount+=nItemCount;
			}
    }

	return TRUE;

} 

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =LF;
}