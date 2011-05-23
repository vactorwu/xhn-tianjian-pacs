//********************************************************************
//  模块: clt_onyx.cpp
//  描述: 本模块为库而特公司的CLT_ONYX血球计数仪联机程序(Win95版)
  
#include <stdio.h>
#include <Const.h>
#include <Device.h>
#include <DevMacro.h>
#include <str.h>
#include <string.h>

#define c_RxBufferLength	512		//接收缓冲区长(一个包最长247)
#define	c_MaxItemNum		20		
#define c_RxCheckSumLen		2		
#define SeekRange           10		//最小搜索范围
#define MaxSeekRange        256		//最大搜索范围
#define Delimiter	'|'				
#define	sample_flagOff		18		//样本类型字符偏移






//************************************************************************
//DLLEXPORT DWORD WINAPI CommMonitorProc( LPSTR lpData )
//
//  描述:
//     串口监控线程的启动函数
//  参数:
//     LPSTR lpData
//        生化仪接口控制块
//************************************************************************

DWORD WINAPI CommMonitor( LPSTR lpData)
{
	CommData


	SYSTEMTIME SystemTime;
	int	 i, nItemCount, nOnePackLen  ,nDataLen,nItemIDLen,nSampleIDLen;
	BYTE sample_flag, btResultType, btCalCheckSum, btRxCheckSum;
	BYTE btRecordType;
 	char *lptempOpPointer, *ptemp1,*ptemp2,*ptemp3;	
 

	Init()




	nItemCount =0;

	ResetOpPointer()				



	FillDevName()					

	

	SetCommWaitMask(EV_RXCHAR )


	while (TRUE)
	{
		
		nOnePackLen =0;
 		ResetOpPointer()
		ResetRxPointer()

		do
		{
			GetInQueueLength(dwReadLength)


			if (dwReadLength ==0 )	//&& dwRxBufferLength==0)
			{
				WaitEvent(EV_RXCHAR )  

				
				Sleep(100);	
				GetInQueueLength(dwReadLength)
			}
			
			 if (dwReadLength!=0)  
			 {

				ReadCommBlock(dwReadLength)

				lpOpPointer[dwRxBufferLength ]='\0';
				if(strchrnb(lpOpPointer+dwRxBufferLength-dwReadLength,ENQ,1,dwReadLength)!=NULL)
				{
						ResetRxPointer()
						ResetOpPointer()
						WriteCommChar(ACK)	
	   					continue;
				}

				ptemp3=lpOpPointer+strlen(lpOpPointer)-1;
// 				if (strbchrnb(ptemp3,LF,1,3)!=NULL)
				if (*ptemp3==LF)
				{
					
					lpOpPointer[dwRxBufferLength ]='\0';
					

					lptempOpPointer=strchrnb(lpOpPointer,STX,1,SeekRange);
 
					if (lptempOpPointer==NULL)  
					{
						PurgeComm( hComm, PURGE_RXCLEAR );
					 	ResetRxPointer()
						ResetOpPointer()
						WriteCommChar(NAK)
						continue;
					}
					
					nOnePackLen=strlen( lptempOpPointer);
					break;	

				}
			 }


		}while(TRUE);	

 


switch(btRecordType)	//switch---A
{
	case 'H':		
	
		sample_flag=*(strchrnb(lptempOpPointer,CR,1,MaxSeekRange )-sample_flagOff);	
 		btResultType = c_TypeSample; 
		if (sample_flag=='Q') btResultType = c_TypeQC; 
		break;
	case 'O':	

		ptemp1 =strchrnb(lptempOpPointer,Delimiter,2, SeekRange)+1;

		ptemp2 =strchrnb(lptempOpPointer,'!',1, SeekRange*2);
//		ptemp2 =strchrnb(ptemp1 ,'!',1,8);
		if (ptemp2==NULL)  
		{

//		ptemp2 =strchrnb(ptemp1 , Delimiter,1,8);
		ptemp2 =strchrnb(lptempOpPointer,Delimiter,3, SeekRange*2);
		}


 		nSampleIDLen=ptemp2-ptemp1;
		wSampleID=StrToInt(ptemp1 , nSampleIDLen);		
		

		GetLocalTime(&SystemTime);
		nItemCount=0;
		break;
	case 'R':	

	ptemp1=strchrnb(lptempOpPointer,Delimiter,2, SeekRange);
	ptemp1 ++;
	ptemp2=strchrnb(lptempOpPointer,Delimiter,3, SeekRange*3);	
	nItemIDLen =ptemp2 - ptemp1 ;

	if (nItemIDLen>8)	break;	
 
 


//填样本号		 
		FillSampleID(nItemCount, wSampleID)	

//填项目名称
		FillItemName(nItemCount,ptemp1+3 ,nItemIDLen-3)	

//填日期
		FillDate(nItemCount, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond)

		FillResultType(nItemCount, btResultType)

		if (strncmp(ptemp2+1,".....",5)==0 || strncmp(ptemp2+1,"+++++",5)==0
			|| strncmp(ptemp2+1,"-----",5)==0 || strncmp(ptemp2+1,"     ",5)==0)
		{

			FillErrorCode(nItemCount,c_UnReliable )		
			nDataLen=5;	

		}
		else 
		{
			FillErrorCode(nItemCount, c_Reliable )	

char *ptemp4;
			ptemp4=strchrnb(ptemp2+1,'!',1,SeekRange);	

			if( ptemp4 !=NULL )	
			{

				ptemp1=strchrnb(ptemp4+1 ,Delimiter ,1, SeekRange);
				nDataLen=ptemp1-(ptemp4+1);
 				FillComment(nItemCount, ptemp4+1 , 1)	//nDataLen)

				nDataLen=ptemp4-(ptemp2+1);	
			}

			else	
			{
				ptemp1=strchrnb(ptemp2+1 ,Delimiter ,1, SeekRange);
				nDataLen=ptemp1-(ptemp2+1);
			}
		}


		FillResult(nItemCount,ptemp2+1, nDataLen )

	nItemCount++;	
 		break;
	case 'L':	
	
		if (nItemCount>0)
		{
			(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
			lpDevice->dwRecordCount+=nItemCount;
		}
		break;
	default:	
 ;
}//end case--A

 		PurgeComm( hComm, PURGE_RXCLEAR );
		WriteCommChar(ACK)

		ResetOpPointer()
 		continue;	
	
}//WHILE--END

	return TRUE;

} // end of CommWatchProc()

void WINAPI BeforeSetCommState(DCB *pComDcb)
{

}

