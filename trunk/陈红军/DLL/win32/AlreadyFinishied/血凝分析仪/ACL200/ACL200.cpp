// ACL200.cpp : Defines the entry point for the DLL application.
//

//********************************************************************
//  模块: clt-acl.cpp
//  描述: 本模块为库尔特(Coulter)公司的acl系列分析仪的联机程序(Win95版)
#include "stdafx.h"

#include <stdio.h>
#include "Const.h"
#include "Device.h"
#include "DevMacro.h"
#include "str.h"
#include "LogFile.h"

#define c_RxBufferLength	100		  
#define	c_MaxItemNum		20

#define	c_StartSymbolLength	1		 
#define	c_ProtocolHeadLength 16		  
#define	c_MsgHeadLength	    52
#define	c_DataMsgLength		28
#define	c_BatchMsgLength	16
#define c_MsgLengthOff		11
#define c_MsgLengthLen		4
#define	c_RxCheckSumLen 2

//NP Result
#define c_NormalPTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_NormalPTLen	4
#define c_NormalFIBOff	c_StartSymbolLength+c_ProtocolHeadLength+6
#define c_NormalFIBLen	4
#define c_NormalAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_NormalAPTTLen	4
#define c_NormalTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_NormalTTLen	4
#define c_NormalTTAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength+6
#define	c_NormalPTFIBAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength+12	

//Common define 
#define c_TestIDOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestIDLen		2
#define c_MsgIDOff		c_StartSymbolLength+c_ProtocolHeadLength+24
#define c_MsgIDLen		2
#define c_FormatOff		c_StartSymbolLength+c_ProtocolHeadLength+4


//Test Result
//pt-fib
#define c_TestPTOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestPTLen		4
#define	c_TestPERCENOff	c_StartSymbolLength+c_ProtocolHeadLength+5
#define	c_TestPERCENLen	3
#define	c_TestR_INROff	c_StartSymbolLength+c_ProtocolHeadLength+9
#define	c_TestR_INRLen	4
#define c_TestFIBOff	c_StartSymbolLength+c_ProtocolHeadLength+13
#define c_TestFIBLen	4
//------------------------
//aptt
#define c_TestAPTTOff	c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestAPTTLen	4
#define	c_TestAPTT_ROff	c_StartSymbolLength+c_ProtocolHeadLength+4
#define	c_TestAPTT_RLen	4
//------------------------
//tt
#define c_TestTTOff		c_StartSymbolLength+c_ProtocolHeadLength
#define c_TestTTLen		4
#define	c_TestTT_ROff	c_StartSymbolLength+c_ProtocolHeadLength+4
#define	c_TestTT_RLen	4
//




//以下为在W型结果中各个项目的偏移及长度定义
//AT_III_ANALYSIS
#define c_AT_III_ANALYSIS_PERCENT_OFF c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_AT_III_ANALYSIS_PERCENT_Len       3
#define c_AT_III_ANALYSIS_Delta_OFF c_StartSymbolLength+c_ProtocolHeadLength+5
#define c_AT_III_ANALYSIS_Delta_Len       5
//PRO_IL_COMPLEX_ANALYSIS
#define c_Pro_IL_Cpx_A_Percent_Off c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_Pro_IL_Cpx_A_Percent_Len		  5
#define c_Pro_IL_Cpx_A_R_Off c_StartSymbolLength+c_ProtocolHeadLength+7
#define c_Pro_IL_Cpx_A_R_Len		  4
#define c_Pro_IL_Cpx_A_S_Off c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_Pro_IL_Cpx_A_S_Len		  4
//HEPATOCOMPLEX ANALYSIS
#define c_Hepato_cpx_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_Hepato_cpx_P_Len   5
#define c_Hepato_cpx_R_Off c_StartSymbolLength+c_ProtocolHeadLength+7
#define c_Hepato_cpx_R_Len		  4
#define c_Hepato_cpx_S_Off c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_Hepato_cpx_S_Len		  4
//PATHWAYS
#define  c_Pathways_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define  c_Pathways_P_Len      3
#define  c_Pathways_S_Off   c_StartSymbolLength+c_ProtocolHeadLength+5
#define  c_Pathways_S_Len      4
//SingleFactor
#define c_SingleFactorH_P_Off c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_SingleFactorH_P_Len  3
#define c_SinlgeFactorH_S_Off  c_StartSymbolLength+c_ProtocolHeadLength+9
#define c_SingleFactorH_S_Len  4

#define c_SingleFactorL_P_Off c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_SingleFactorL_P_Len  4
#define c_SinlgeFactorL_S_Off  c_StartSymbolLength+c_ProtocolHeadLength+10
#define c_SingleFactorL_S_Len  3

//AT_III
#define c_AT_III_P_Off      c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_AT_III_P_Len       3
#define c_AT_III_Delta_Off  c_StartSymbolLength+c_ProtocolHeadLength+5
#define c_AT_III_Delta_Len   5
//ANTIPLASMIN
#define c_ANTI_PASMIN_P_Off      c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_ANTI_PASMIN_P_Len       3
#define c_ANTI_PASMIN_Delta_Off  c_StartSymbolLength+c_ProtocolHeadLength+5
#define c_ANTI_PASMIN_Delta_Len   5
//PLASMINOGEN 
#define c_PLASMINOGEM_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_PLASMINOGEM_P_Len   3
#define c_PLASMINOGEM_D_Off   c_StartSymbolLength+c_ProtocolHeadLength+5
#define c_PLASMINOGEM_D_Len   5

//FIBRINOGEN_C
#define c_FIBRINOGEN_C_mgdl_Off c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_FIBRINOGEN_C_mgdl_Len 3
#define c_FIBRINOGEN_C_gl_Off c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_FIBRINOGEN_C_gl_Len 4
#define c_FIBRINOGEN_C_S_Off   c_StartSymbolLength+c_ProtocolHeadLength+6
#define c_FIBRINOGEN_C_S_Len   4

//PROCHROM
#define c_PROCHROM_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_PROCHROM_P_Len   3
#define c_PROCHROM_Delta_Off   c_StartSymbolLength+c_ProtocolHeadLength+5
#define c_PROCHROM_Delta_Len   5
//HERAPIN 
#define c_HERAPIN_Uml_Off    c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_HERAPIN_Uml_Len     4
#define c_HERAPIN_Delta_Off  c_StartSymbolLength+c_ProtocolHeadLength+6
#define c_HERAPIN_Delta_Len   5
//PRO_IL_COMPLEX
#define c_PRO_IL_Cpx_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_PRO_IL_Cpx_P_Len    5
#define c_PRO_IL_Cpx_R_Off   c_StartSymbolLength+c_ProtocolHeadLength+7
#define c_PRO_IL_Cpx_R_Len   4
#define c_PRO_IL_Cpx_S_Off   c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_PRO_IL_Cpx_S_Len   4
//HEPATOCOCOMPLEX
#define c_HEPATO_Cpx_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_HEPATO_Cpx_P_Len    5
#define c_HEPATO_Cpx_R_Off   c_StartSymbolLength+c_ProtocolHeadLength+7
#define c_HEPATO_Cpx_R_Len   4
#define c_HEPATO_Cpx_S_Off   c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_HEPATO_Cpx_S_Len   4
//PROCLOT
#define c_PROCLOT_P_Off   c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_PROCLOT_P_Len    5
#define c_PROCLOT_R_Off   c_StartSymbolLength+c_ProtocolHeadLength+7
#define c_PROCLOT_R_Len   4
#define c_PROCLOT_S_Off   c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_PROCLOT_S_Len    4
//PROTEIN_S
#define c_PROTEIN_S_P_Off    c_StartSymbolLength+c_ProtocolHeadLength+1
#define c_PROTEIN_S_P_Len    5
#define c_PROTEIN_S_S_Off    c_StartSymbolLength+c_ProtocolHeadLength+11
#define c_PROTEIN_S_S_Len    4

//删除批号和样本号

#define c_DeleteBTNo    1
#define c_DeleteSpNo    2
//定义Test ID值
#define c_PT_FIB		3
#define c_APTT			6
#define c_TT            9
#define c_Extrinsic_Pathway 15
#define c_Intrinsic_Pathway 21
#define c_PT_FIB_APTT	27
#define c_TT_APTT       30
#define c_Ex_Singlefactor 33
#define c_In_Singlefactor 36
#define c_AT_III_Analysis 46
#define c_AT_III          48
#define c_FIBRINOGEN_C    49
#define c_PROCHROM        50
#define c_HEPARIN         51
#define c_ANTIPLASMIN     54
#define c_PLASMINOGEN     57
#define c_PRO_IL_COMPLEX_Analysis   67
#define c_Hepatocomplex_Analysis    76
#define c_Proclot         81
#define c_Protein_S       108

//一个样本做两次！
#define c_Double_FIB      39
#define c_Double_APIT   42
#define c_Double_TT     45
#define c_Double_PT_FIB_APTT  99


#define c_batch4IDOff   55  
#define c_batch4IDLen   12

extern  HANDLE  hLogFile;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}




DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData	
		
	SYSTEMTIME SystemTime;
	bool bLogFile=FALSE;
	int	i, nOneMsgLength, nRxCheckSum, nCalCheckSum, nItemCount,SampleIDLen;
	unsigned char  lpszNormalPT[c_NormalPTLen+1], lpszNormalFIB[c_NormalFIBLen+1],
		lpszNormalAPTT[c_NormalAPTTLen+1], lpszNormalTT[c_NormalTTLen+1];
	BYTE btTestID, btMsgID,PT_FIB_APTT_FLAG=0, TT_APTT_FLAG=0;
	BOOL bUnitIsmgdl =TRUE;
	div_t CheckSumDiv;
	/*	div_t.quot;//商
	div_t.rem//余数*/
	unsigned int batchNo;
	char  TempWorkingID[7],LSampleID[7];


	Init()
	if(NewLogFile("ACL200.Txt"))
		bLogFile=TRUE;
	memset(LPSTR(lpszNormalPT), 0, c_NormalPTLen+1);
	memset(LPSTR(lpszNormalFIB), 0, c_NormalFIBLen+1);
	memset(LPSTR(lpszNormalAPTT), 0, c_NormalAPTTLen+1);
	memset(LPSTR(lpszNormalTT), 0, c_NormalTTLen+1);

	ResetOpPointer()		
 		

	FillDevName()			
 
	SetCommWaitMask(EV_RXFLAG)
	EscapeCommFunction(hComm,SETRTS);
	while (TRUE)
	{
		
		nOneMsgLength=0;

		ResetRxPointer()		

		GetInQueueLength(dwReadLength)

		if (dwReadLength ==0)
		WaitEvent(EV_RXFLAG)
	//从缓冲区中读协议头长度数据，如有，则返回；否则等待数据来
		FetchCommBlock(c_StartSymbolLength +c_ProtocolHeadLength)

		nOneMsgLength =HStrToInt(lpOpPointer+c_MsgLengthOff, c_MsgLengthLen);
		switch(nOneMsgLength)
		{
		//判断数据长度是否为合法长度
		case 16:
		case 28:
		case 52:
			//从缓冲区中读消息头长度数据+校验和数据，如有，则返回；否则等待数据来
			FetchCommBlock(nOneMsgLength +c_RxCheckSumLen)
			break;
		default:
			ClearCommRx()//清空接受缓冲区
			WriteCommChar('!');
			WriteCommBlock("????", 4);
			continue;


		}
		if(bLogFile)
		{
			WriteLogData(hLogFile,lpOpPointer,dwRxBufferLength);	
		}
		//计算校验和
		nRxCheckSum =HStrToInt(lpOpPointer+c_StartSymbolLength
					+c_ProtocolHeadLength +nOneMsgLength, 
					c_RxCheckSumLen);  

		for(nCalCheckSum=0, i =1; i<int(dwRxBufferLength-c_RxCheckSumLen); i++)
 			nCalCheckSum +=RxBuffer[i];
		do{
			CheckSumDiv =div(nCalCheckSum, 256);
			nCalCheckSum =CheckSumDiv.quot +CheckSumDiv.rem;
		}while(nCalCheckSum>256);
		nCalCheckSum =256 -nCalCheckSum;
		if (nCalCheckSum !=nRxCheckSum)
		{ //NAK
			ClearCommRx()//清空接受缓冲区
			WriteCommChar('!');
			WriteCommBlock("????", 4);
			continue;
		}
	
	//消息长度:52——MessageHeader
	//		  28——Message Contents
	//        16——Delete Batch No||Delete Sample ID
		switch (nOneMsgLength)	
		{
			case	c_MsgHeadLength: // 52
				btTestID =(BYTE)HStrToInt(lpOpPointer+c_TestIDOff, c_TestIDLen); 
				if (lpOpPointer[c_FormatOff]=='0' || lpOpPointer[c_FormatOff]=='1' || lpOpPointer[c_FormatOff]=='8' || lpOpPointer[c_FormatOff]=='9')
					bUnitIsmgdl	=TRUE;			
				else
					bUnitIsmgdl	=FALSE;
				GetLocalTime(&SystemTime);		
				nItemCount =0;
				batchNo =StrToInt(lpOpPointer+c_batch4IDOff, c_batch4IDLen)-1;
				ClearCommRx()//清空接受缓冲区
				PurgeComm( hComm, PURGE_TXCLEAR );
				WriteCommChar('!')//发送肯定应答！
				WriteCommBlock(lpOpPointer+1, 4);

 				break;
 			
			case	c_DataMsgLength://28
				btMsgID =StrToInt(lpOpPointer+c_MsgIDOff, c_MsgIDLen);
				switch (btMsgID)
				{
					case 50://message with NP(Normal Pool) result identifer <NP-ID>
					case 70://message with coefficients of cal. line <m,q,r2>
					case 81://message with coordinates of cal. point <Y,X> 
					case 82:
					case 83:
						break;
					case  60:	//message whith results of NP
						switch (btTestID)	
						{
							case  c_PT_FIB:
								strncpy(LPSTR(lpszNormalPT), lpOpPointer+c_NormalPTOff+1, c_NormalPTLen);	
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--;
								strncpy(LPSTR(lpszNormalFIB), lpOpPointer+c_NormalFIBOff+1, i);	 
								lpszNormalFIB[i] ='\0';
								break;
							case  c_APTT:
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalAPTTOff+1, c_NormalAPTTLen);
								break;
							case  c_TT:
								strncpy(LPSTR(lpszNormalTT), lpOpPointer+c_NormalTTOff+1, c_NormalTTLen);
								break;
							case  c_PT_FIB_APTT:
								strncpy(LPSTR(lpszNormalPT), lpOpPointer+c_NormalPTOff+1, c_NormalPTLen);	
																i =c_NormalFIBLen;
																if (bUnitIsmgdl) i--; 
								strncpy(LPSTR(lpszNormalFIB), lpOpPointer+c_NormalFIBOff+1, i);	 //Error
																lpszNormalFIB[i] ='\0';
								if (bUnitIsmgdl)	
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalPTFIBAPTTOff, c_NormalAPTTLen);				 				
								else
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalPTFIBAPTTOff+1 , c_NormalAPTTLen);	
								break;
							case  c_TT_APTT:
								strncpy(LPSTR(lpszNormalTT), lpOpPointer+c_NormalTTOff+1, c_NormalTTLen);
								strncpy(LPSTR(lpszNormalAPTT), lpOpPointer+c_NormalTTAPTTOff+1, c_NormalAPTTLen);
								break;
							case c_Extrinsic_Pathway:
								break;
							case c_Intrinsic_Pathway:
								break;
							case c_PRO_IL_COMPLEX_Analysis:
								break;
							case c_Hepatocomplex_Analysis:
								break;
							case c_Proclot:
								break;
							case c_Protein_S:
								break;
						}
						break;
					default:
						if( (btMsgID >=21)& (btMsgID<=28))//21..38:message with sample id of sample 1..18;
							break;	
						////01..18:results of sample1..18[Test result]
						//以下为W型的结果PT_FIB,APTT,TT,
						//TT_APTT,PT_FIB_APTT,HEPATOCPOMPLEX,HEPATOCPOMPLEX_Complex,PROCLOT,PROCLO_S
						//项目后缀为S：时间，  R：比率，P：百分比，D：Delta  OD
						wSampleID =(WORD)(btMsgID+batchNo*18);
						IntToStr(LSampleID,wSampleID);
						TempWorkingID[0]=lpDevice->DevItemType;
				    	//向工作单号中添加'0'
						SampleIDLen=strlen(LSampleID);
						memset(&TempWorkingID[1],0x30,5);
						strncpy(&TempWorkingID[6-SampleIDLen],&LSampleID[0],SampleIDLen);
						TempWorkingID[6]='\0';
						//打印序号
						switch (btTestID)	//switch (btTestID)--B
						{
							case  c_PT_FIB:
								nItemCount = 0;
								FillItemName(nItemCount, "PT_S", 4)
 								FillSampleID(nItemCount, wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
							    FillResult(nItemCount, lpOpPointer+c_TestPTOff, c_TestPTLen )
								OutResult[nItemCount].ItemNo=1;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 
								nItemCount++;
								FillItemName(nItemCount, "PT_P", 4)
 								FillSampleID(nItemCount, wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_TestPERCENOff,c_TestPERCENLen) 
								OutResult[nItemCount].ItemNo=2;
								if (*(lpOpPointer+c_TestPERCENOff)=='*' || *(lpOpPointer+c_TestPERCENOff)=='-')
								{
									FillErrorCode(nItemCount, c_UnReliable )	
								}
								else 
								{
									if(*(lpOpPointer+c_TestPERCENOff-1)!='*' && *(lpOpPointer+c_TestPERCENOff-1)!=' ') //有错误代码
									{
										FillErrorCode(nItemCount, c_UnReliable )	
									}
								}								
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 
								nItemCount++;
								FillItemName(nItemCount, "PT_R", 4)
 								FillSampleID(nItemCount, wSampleID)	//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount,lpOpPointer+c_TestR_INROff, c_TestR_INRLen) 
								OutResult[nItemCount].ItemNo=3;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)		 
								nItemCount++;
 								FillItemName(nItemCount, "FIB",3)
								FillSampleID(nItemCount, wSampleID)	//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								i =c_NormalFIBLen;
								if (bUnitIsmgdl) i--; //FIB单位为mg/dl或//FIB单位为g/l
 								FillResult(nItemCount,lpOpPointer+c_TestFIBOff+1 ,i ) 
								OutResult[nItemCount].ItemNo=4;
								if (*(lpOpPointer+c_TestFIBOff+1)=='*' || *(lpOpPointer+c_TestFIBOff+1)=='-')
								{
									FillErrorCode(nItemCount, c_UnReliable )	
								}
								else 
								{
									if(*(lpOpPointer+c_TestFIBOff)!='*' && *(lpOpPointer+c_TestFIBOff)!=' ') 
									{
										FillErrorCode(nItemCount, c_UnReliable )	
									}
								}
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case  c_APTT:
								nItemCount = 0;
								FillItemName(nItemCount, "APTT_S",6)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen)
								OutResult[nItemCount].ItemNo=5;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								
								FillItemName(nItemCount, "APTT_R", 6)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen) 
								OutResult[nItemCount].ItemNo=6;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;  	
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;

							case  c_TT:
								nItemCount = 0;
								FillItemName(nItemCount, "TT_S", 4)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount,lpOpPointer+c_TestTTOff, c_TestTTLen  ) 
								OutResult[nItemCount].ItemNo=7;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;	

								FillItemName(nItemCount, "TT_R", 4)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount,lpOpPointer+c_TestTT_ROff,c_TestTT_RLen)
								OutResult[nItemCount].ItemNo=8;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}
								nItemCount = 0;
								break;

							case  c_PT_FIB_APTT:
								nItemCount = 0;
								if(PT_FIB_APTT_FLAG==0)
								{
									FillItemName(nItemCount, "PT_S", 4)
 									FillSampleID(nItemCount, wSampleID)	//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestPTOff, c_TestPTLen) 
									OutResult[nItemCount].ItemNo=1;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)		 
									nItemCount++;

									FillItemName(nItemCount, "PT_P", 4)
 									FillSampleID(nItemCount, wSampleID)	//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestPERCENOff  , c_TestPERCENLen) 
									OutResult[nItemCount].ItemNo=2;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;

									FillItemName(nItemCount, "PT_R", 4)
 									FillSampleID(nItemCount, wSampleID)	//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestR_INROff, c_TestR_INRLen) 
									OutResult[nItemCount].ItemNo=3;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
										nItemCount++;
 									FillItemName(nItemCount, "FIB", 3)
									FillSampleID(nItemCount, wSampleID)	//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									i =c_NormalFIBLen;
									if (bUnitIsmgdl) i--; 
 									FillResult(nItemCount,lpOpPointer+c_TestFIBOff+1 ,i) 
									OutResult[nItemCount].ItemNo=4;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									PT_FIB_APTT_FLAG=1;
								}
								else
								{
	//APTT RESULT	
									FillItemName(nItemCount, "APTT_S", 6)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen) 
									OutResult[nItemCount].ItemNo=5;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									FillItemName(nItemCount, "APTT_R", 6)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen)
									OutResult[nItemCount].ItemNo=6;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									PT_FIB_APTT_FLAG=0;
									if (nItemCount>0)
									{
										(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
										lpDevice->dwRecordCount+=nItemCount;
									}		
									nItemCount = 0;

								}
 								break;
			 
							case  c_TT_APTT:
								nItemCount = 0;
								if (TT_APTT_FLAG ==0)
								{
									//TT Result
									FillItemName(nItemCount, "TT_S", 4)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestTTOff, c_TestTTLen) 
									OutResult[nItemCount].ItemNo=7;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									
									FillItemName(nItemCount, "TT_R", 4)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									FillResult(nItemCount,lpOpPointer+c_TestTT_ROff,c_TestTT_RLen)
									OutResult[nItemCount].ItemNo=8;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond) 		 
									nItemCount++;
									TT_APTT_FLAG =1;
								}
								else
								{
									FillItemName(nItemCount, "APTT_S", 6)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen)
									OutResult[nItemCount].ItemNo=5;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									
									FillItemName(nItemCount, "APTT_R", 6)
									FillSampleID(nItemCount, wSampleID)//样本号
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 									FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen)
									OutResult[nItemCount].ItemNo=6;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									if (nItemCount>0)
									{
										(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
										lpDevice->dwRecordCount+=nItemCount;
									}			
									nItemCount = 0;
									TT_APTT_FLAG =0;
								}
 								break;
							case c_AT_III_Analysis:
								nItemCount = 0;
								FillItemName(nItemCount, "AT3A_P", 6)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount,lpOpPointer+c_TestAPTTOff, c_TestAPTTLen)
								OutResult[nItemCount].ItemNo=9;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								
								FillItemName(nItemCount, "AP3A_R", 6)
								FillSampleID(nItemCount, wSampleID)//样本号
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
 								FillResult(nItemCount, lpOpPointer+c_TestAPTT_ROff, c_TestAPTT_RLen)
								OutResult[nItemCount].ItemNo=10;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_PRO_IL_COMPLEX_Analysis:
								nItemCount = 0;
								FillItemName(nItemCount,"PILA_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pro_IL_Cpx_A_Percent_Off, c_Pro_IL_Cpx_A_Percent_Len)
								OutResult[nItemCount].ItemNo=11;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PILA_R",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pro_IL_Cpx_A_R_Off, c_Pro_IL_Cpx_A_R_Len)
								OutResult[nItemCount].ItemNo=12;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PILA_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pro_IL_Cpx_A_S_Off, c_Pro_IL_Cpx_A_S_Len)
								OutResult[nItemCount].ItemNo=13;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_Hepatocomplex_Analysis:
								nItemCount = 0;
								FillItemName(nItemCount,"HCXA_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Hepato_cpx_P_Off, c_Hepato_cpx_P_Len)
								OutResult[nItemCount].ItemNo=14;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;

								FillItemName(nItemCount,"HCXA_R",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Hepato_cpx_R_Off, c_Hepato_cpx_R_Len)
								OutResult[nItemCount].ItemNo=15;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"HCXA_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Hepato_cpx_S_Off, c_Hepato_cpx_S_Len)
								OutResult[nItemCount].ItemNo=16;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_Extrinsic_Pathway:
								nItemCount = 0;
								FillItemName(nItemCount,"EXPW_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pathways_P_Off, c_Pathways_P_Len)
								OutResult[nItemCount].ItemNo=17;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;

								FillItemName(nItemCount,"EXPW_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pathways_S_Off, c_Pathways_S_Len)
								OutResult[nItemCount].ItemNo=18;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;	
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_Intrinsic_Pathway:
								nItemCount = 0;
								FillItemName(nItemCount,"INPW_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pathways_P_Off, c_Pathways_P_Len)
								OutResult[nItemCount].ItemNo=19;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;

								FillItemName(nItemCount,"INPW_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_Pathways_S_Off, c_Pathways_S_Len)
								OutResult[nItemCount].ItemNo=20;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;	
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_Ex_Singlefactor:
								nItemCount = 0;
								FillItemName(nItemCount,"EXSF_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_SingleFactorH_P_Off, c_SingleFactorH_P_Len)
								OutResult[nItemCount].ItemNo=22;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"EXSF_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=23;
								FillResult(nItemCount, lpOpPointer+c_SinlgeFactorH_S_Off, c_SingleFactorH_S_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_In_Singlefactor:
								FillItemName(nItemCount,"INSF_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_SingleFactorL_P_Off, c_SingleFactorL_P_Len)
								OutResult[nItemCount].ItemNo=24;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"INSF_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_SinlgeFactorL_S_Off, c_SingleFactorL_S_Len)
								OutResult[nItemCount].ItemNo=25;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_AT_III:
								FillItemName(nItemCount,"AT3_P",5)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_AT_III_P_Off, c_AT_III_P_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								OutResult[nItemCount].ItemNo=26;
								nItemCount++;
								FillItemName(nItemCount,"AT3_D",5)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_AT_III_Delta_Off, c_AT_III_Delta_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								OutResult[nItemCount].ItemNo=27;
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_ANTIPLASMIN:
								nItemCount = 0;
								FillItemName(nItemCount,"APAS_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_ANTI_PASMIN_P_Off, c_ANTI_PASMIN_P_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								OutResult[nItemCount].ItemNo=28;
								nItemCount++;
								FillItemName(nItemCount,"APAS_D",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=29;
								FillResult(nItemCount, lpOpPointer+c_ANTI_PASMIN_Delta_Off, c_ANTI_PASMIN_Delta_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_PLASMINOGEN:
								FillItemName(nItemCount,"PLAS_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=30;
								FillResult(nItemCount, lpOpPointer+c_PLASMINOGEM_P_Off, c_PLASMINOGEM_P_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PLAS_D",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=30;
								FillResult(nItemCount, lpOpPointer+c_PLASMINOGEM_D_Off, c_PLASMINOGEM_D_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}								
								nItemCount = 0;
								break;
							case c_FIBRINOGEN_C:
								nItemCount = 0;
								if(bUnitIsmgdl)
								{
									FillItemName(nItemCount,"FIB",3)
									FillSampleID(nItemCount,wSampleID)	
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									OutResult[nItemCount].ItemNo=5;
									FillResult(nItemCount, lpOpPointer+c_FIBRINOGEN_C_mgdl_Off, c_FIBRINOGEN_C_mgdl_Len)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									FillItemName(nItemCount,"FIB_S",5)
									FillSampleID(nItemCount,wSampleID)	
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									FillResult(nItemCount, lpOpPointer+c_FIBRINOGEN_C_S_Off, c_FIBRINOGEN_C_S_Len)
									OutResult[nItemCount].ItemNo=31;
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									if (nItemCount>0)
									{
										(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
										lpDevice->dwRecordCount+=nItemCount;
									}								

								}
								else
								{
									FillItemName(nItemCount,"FIB",3)
									FillSampleID(nItemCount,wSampleID)	
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									OutResult[nItemCount].ItemNo=4;
									FillResult(nItemCount, lpOpPointer+c_FIBRINOGEN_C_gl_Off, c_FIBRINOGEN_C_gl_Len)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									FillItemName(nItemCount,"FIB_S",5)
									FillSampleID(nItemCount,wSampleID)	
									strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
									OutResult[nItemCount].ItemNo=31;
									FillResult(nItemCount, lpOpPointer+c_FIBRINOGEN_C_S_Off, c_FIBRINOGEN_C_S_Len)
									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
									nItemCount++;
									if (nItemCount>0)
									{
										(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
										lpDevice->dwRecordCount+=nItemCount;
									}								

								}
								nItemCount = 0;
								break;
							case c_PROCHROM:
								nItemCount = 0;
								FillItemName(nItemCount,"PROM_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=32;
								FillResult(nItemCount, lpOpPointer+c_PROCHROM_P_Off, c_PROCHROM_Delta_Off)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PROM_D",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=33;
								FillResult(nItemCount, lpOpPointer+c_PROCHROM_Delta_Off, c_PROCHROM_Delta_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}
								nItemCount = 0;
								break;
							case c_HEPARIN:
								nItemCount = 0;
								FillItemName(nItemCount,"HERP_U",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=34;
								FillResult(nItemCount, lpOpPointer+c_HERAPIN_Uml_Off, c_HERAPIN_Uml_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"HERP_D",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								
								FillResult(nItemCount, lpOpPointer+c_HERAPIN_Delta_Off, c_HERAPIN_Delta_Len)
								OutResult[nItemCount].ItemNo=35;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}
								nItemCount = 0;
								break;
							case c_Proclot:
								FillItemName(nItemCount,"PROC_P",6)
								FillSampleID(nItemCount,wSampleID)	
								OutResult[nItemCount].ItemNo=36;
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_PROCLOT_P_Off, c_PROCLOT_P_Len)
								
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PROC_R",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_PROCLOT_R_Off, c_PROCLOT_R_Len)
								OutResult[nItemCount].ItemNo=37;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								OutResult[nItemCount].ItemNo=38;
								FillItemName(nItemCount,"PROC_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_PROCLOT_S_Off, c_PROCLOT_S_Len)
						
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}
								nItemCount = 0;
								break;
							case c_Protein_S:
								FillItemName(nItemCount,"PROT_P",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								OutResult[nItemCount].ItemNo=39;
								FillResult(nItemCount, lpOpPointer+c_PROTEIN_S_P_Off, c_PROTEIN_S_P_Len)
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								FillItemName(nItemCount,"PROT_S",6)
								FillSampleID(nItemCount,wSampleID)	
								strncpy(OutResult[nItemCount].WorkingID,TempWorkingID,7);
								FillResult(nItemCount, lpOpPointer+c_PROTEIN_S_S_Off, c_PROTEIN_S_S_Len)
								OutResult[nItemCount].ItemNo=40;
								FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)
								nItemCount++;
								if (nItemCount>0)
								{
									(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
									lpDevice->dwRecordCount+=nItemCount;
								}
								nItemCount = 0;
								break;
						}//end of switch (btTestID)--B

				}//end of switch (btMsgID)
				ClearCommRx()//清空接受缓冲区
				PurgeComm( hComm, PURGE_TXCLEAR );
				WriteCommChar('!')//发送肯定应答！
				WriteCommBlock(lpOpPointer+1, 4);

				break;
			case	c_BatchMsgLength://16
				ClearCommRx()//清空接受缓冲区
				PurgeComm( hComm, PURGE_TXCLEAR );
				WriteCommChar('!')//发送肯定应答！
				WriteCommBlock(lpOpPointer+1, 4);
				
				break;
			default :
				WriteCommChar('!');
				WriteCommBlock("????", 4);
				continue;
		}

//		ClearCommRx()//清空接受缓冲区
//		PurgeComm( hComm, PURGE_TXCLEAR );
// 		WriteCommChar('!')//发送肯定应答！
//		WriteCommBlock(lpOpPointer+1, 4);
	
 	
	}//end of while(TRUE)
	if(bLogFile)
		CloseLogFile(hLogFile);
	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =':';//监看数据起始字符
}

 