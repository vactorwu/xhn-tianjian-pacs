// RL747.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "device.h"
#include "devmacro.h"
#include "str.h"
#define c_RxBufferLength	512	
#define	c_MaxItemNum		60		
#define c_PacketFuncOff		1		
#define c_TransModeOff		3		
#define c_SampleTypeOff		2		

#define c_SampleNoOff		8		
#define c_SampleNoLen		5		
#define c_SampleIDOff		14		
#define c_SampleIDLen		13		
 

#define c_DataOff			29		
#define c_OneDataLen		9		
#define c_ItemIDOff			0		
#define c_ItemIDLen			2		
#define c_ResultOff			2		
#define c_ResultLen			6		
#define c_ErrorCodeOff		8		
#define c_ErrorCodeLen		1		

#define c_RxCheckSumLen		2		


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

 	SYSTEMTIME SystemTime;
	int	 nItemCount,nDataOff,nResultLen,AnyTimes=0;
	int  i,btResultType, btCalCheckSum,btRxCheckSum,CK1,CK2;//计算检验核所用到的变量
	int SampleIDLen;//工作单字符串起始位置
	DWORD wSampleID1;
	LPSTR lpPointer,lptemp1;
	char *p_MOR  ="\x2>\x3\x33\x45\r";		//DATA ACCEPPED 数据包
	char *p_REP  ="\x2?\x3\x33\x46\r";	  //重复上一个 数据包	
	char *p_SingleMax="\x2=D \x33\x41\x31\r";	//OPC Host wants no limits to frames;
 	char TempRx[c_RxBufferLength];	
	char LastFuncChar;	
	char LSampleID[7];

	Init()
 
	nItemCount =0;
	ResetOpPointer()				
 	FillDevName()					
 	SetCommWaitMask(EV_RXFLAG ) 
	while (TRUE)
	{
		ResetRxPointer()
		GetInQueueLength(dwReadLength)
 		WaitEvent(EV_RXFLAG)  
  		GetInQueueLength(dwReadLength)
  		if (dwReadLength!=0 )
		{
			ReadCommBlock(dwReadLength)
		}
		else	continue; 
		lptemp1=strchrnb(lpOpPointer,STX,1,20);
		//上面函数用来定位包起始位在接收缓冲区中的位置.???
		if (lpOpPointer[0]!=STX) 
		{	
			if(lptemp1!=NULL)
			{
				dwRxBufferLength=dwRxBufferLength-(lptemp1-lpOpPointer);
				memmove( lpOpPointer, lptemp1,dwRxBufferLength );			
			}
		}
		i =2;
		btCalCheckSum =lpOpPointer[1];
		do 
		{
			if (lpOpPointer[i] ==ETX )
			  break;
			btCalCheckSum +=lpOpPointer[i];
			btCalCheckSum=btCalCheckSum%256;	   
		   i++;
		}while(TRUE);
		if (btCalCheckSum!=0)
		{
			CK1=lpOpPointer[i+1]-0x30;
			if(CK1>9)
				CK1 -= 7;
		    CK2 = lpOpPointer[i+2]-0x30;
			if(CK2>9)
				CK2 -= 7;
			btRxCheckSum = CK1 * 16;
		    btRxCheckSum += CK2;
			if (btRxCheckSum!=btCalCheckSum)
			{
				PurgeComm( hComm, PURGE_RXCLEAR );		
				WriteCommBlock(p_REP ,strlen(p_REP))
				continue;
			}
			
		}
		if(lpOpPointer[0]==STX) 
		{
				switch (*(lpOpPointer+c_PacketFuncOff)) 
				{	 
					case	'>':
						AnyTimes++; 
						if (AnyTimes%2)	
						{
							PurgeComm( hComm, PURGE_RXCLEAR );
							WriteCommBlock(p_SingleMax ,strlen(p_SingleMax))	
						}
						else	
						{
							PurgeComm( hComm, PURGE_RXCLEAR );
							WriteCommBlock(p_MOR ,strlen(p_MOR))	
						}
						continue;
					case	'1':
 					case	'2':
 					case	':':
						AnyTimes=0;
						break;	
					
					default :	
						PurgeComm( hComm, PURGE_RXCLEAR );
						WriteCommBlock(p_MOR,strlen(p_MOR))
						continue;
				}
					nItemCount =0;
					switch (*(lpOpPointer+c_TransModeOff))	
					{				  
						case	'A': 

						case	'B': 

							GetLocalTime(&SystemTime);

							wSampleID =StrToInt(lpOpPointer+c_SampleNoOff, c_SampleNoLen);
							_itoa(wSampleID,LSampleID,10);
							SampleIDLen=strlen(LSampleID);
							TempWorkingID[0]=lpDevice->DevItemType;
							memset(&TempWorkingID[1],0x30,5);
							strncpy(&TempWorkingID[5-SampleIDLen],&LSampleID[0],SampleIDLen);
							TempWorkingID[6]='\0';
					
				
			
			   				
							//样本号，1－10000
							wSampleID1 =StrToInt(lpOpPointer+c_SampleIDOff, c_SampleIDLen);
							//样本号的条形码值，若条形码读取器允许
							switch (*(lpOpPointer+c_SampleTypeOff ))
							{
								case	'A':
									btResultType =c_TypeSample;
									break;
								case	'I':
									btResultType =c_TypeQC;
									break;
								case	'G':
									btResultType =c_TypeStat;
									break;
								case	'B':
									btResultType = c_TypeUrine;
									break;
 								case	'C':
									btResultType =c_TypeAutoRerun ;
									break;					
							}
							if (*(lpOpPointer+ c_SampleTypeOff)=='A' ||
								*(lpOpPointer+ c_SampleTypeOff)=='B' ||
								*(lpOpPointer+ c_SampleTypeOff)=='C' ||
								*(lpOpPointer+ c_SampleTypeOff)=='G' ||
								*(lpOpPointer+ c_SampleTypeOff)=='I' )	
							{	
								for(nDataOff =c_DataOff; *(lpOpPointer+nDataOff)!=ETX ;)
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
									OutResult[nItemCount].lpDeviceResultCount=&(lpDevice->dwRecordCount);//将当前结果记录的仪器结果数指针指向仪器结果数变量.
									lpPointer=lpOpPointer+nDataOff+c_ResultOff;
									DeleSpace(c_ResultLen )

									FillResult(nItemCount, lpPointer ,nResultLen ) 
									strncpy(OutResult[nItemCount].WorkingID,&TempWorkingID[0],c_MaxWorkingIDLength+1);

									FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

									FillResultType(nItemCount, btResultType)

 									FillComment(nItemCount, lpOpPointer+nDataOff+c_ErrorCodeOff, c_ErrorCodeLen)	
									 
 									FillErrorCode(nItemCount, c_Reliable)	
 									nItemCount++;   
								
									nDataOff+=c_OneDataLen;  

								}	
							}
							break;
					}	

					if (nItemCount>0)
					{
						(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
						lpDevice->dwRecordCount+=nItemCount;
					}
					
		 			memmove( TempRx,lpOpPointer,dwRxBufferLength);	
					LastFuncChar=*(lpOpPointer+c_PacketFuncOff);			

	 				PurgeComm( hComm, PURGE_RXCLEAR );	
					WriteCommBlock(p_MOR ,strlen(p_MOR))	
			
		}
		else
		{
			
	 			 PurgeComm( hComm, PURGE_RXCLEAR );	
				 WriteCommBlock(p_REP ,strlen(p_REP))	
		}
		
	}

return TRUE;

}
void WINAPI BeforeSetCommState(DCB *pComDcb)
{
	pComDcb->EvtChar =CR;

	// 设置通信事件字符,当串口接收到该字符并移入缓冲区后,产生一个事件通知串口监控线程
	//此字符根据仪器不同而改变. 此注释  陈红军于2000.8.15 
}