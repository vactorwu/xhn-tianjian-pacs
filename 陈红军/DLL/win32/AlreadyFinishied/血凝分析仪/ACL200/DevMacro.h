#define	 CommData								\
	DEVICE  *	lpDevice;						\
	HANDLE 		hComm ;								\
	RESULT		OutResult[c_MaxItemNum];		\
	WORD 		wSampleID;						\
	BYTE		dResult[c_MaxResultLength+1];				\
	BYTE   		RxChar=0, TxChar=0;				\
	DWORD		dwRxBufferLength=0, dwReadLength=0, dwWrittenLength=0;	\
	DWORD		dwEvtMask =0;					\
	DWORD   	dwError =0;						\
	COMSTAT 	ComStat;						\
	LPSTR  	lpRxPointer=NULL;				\
	LPSTR		lpOpPointer=NULL;				\
	BYTE    	RxBuffer[c_RxBufferLength] ;	\
	LPRESULT_PROCESS_ROUTINE	lpResultProcessRoutine;				\
	int		nIntCount;							

#define	 Init()									\
	lpDevice =(LPDEVICE)lpData;					\
	hComm =lpDevice->hComm;						\
	lpResultProcessRoutine =lpDevice->lpResultProcessRoutine;		\
	dwEvtMask =0 ;								\
												\
	for (nIntCount =0; nIntCount<c_MaxItemNum; nIntCount++)	\
		{										\
		memset(LPSTR(OutResult[nIntCount].InstrumentID), 0, c_MaxDevNameLength);	\
		OutResult[nIntCount].SampleID	=0;			\
		memset(LPSTR(OutResult[nIntCount].ItemID), 0, c_MaxItemNameLength);\
		memset(LPSTR(OutResult[nIntCount].AssayResult), 0, c_MaxResultLength);\
		OutResult[nIntCount].ResultType =c_TypeSample;			\
		OutResult[nIntCount].ErrorCode  =c_Reliable;			\
		memset(LPSTR(OutResult[nIntCount].Comment), 0, c_MaxCommentLength);\
		}														\
	lpRxPointer =(LPSTR)RxBuffer;								\
	PurgeComm( hComm, PURGE_RXCLEAR ) ;

#define	 ResetRxPointer()										\
	lpRxPointer =(LPSTR)RxBuffer;								\
	dwRxBufferLength =0;

#define	 ResetOpPointer()										\
	lpOpPointer =(LPSTR)RxBuffer;

#define SetCommWaitMask(WaitMask)								\
	SetCommMask(hComm, WaitMask);

#define WaitCommChar(WaitChar)								\
	SetCommMask(hComm, EV_RXCHAR);							\
	do{														\
		WaitCommEvent(hComm, &dwEvtMask, NULL );			\
		if ((dwEvtMask & EV_RXCHAR) != EV_RXCHAR)			\
			continue;										\
		ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	\
	} while(RxChar !=WaitChar);
	
#define GetCommChar()										\
	do{														\
		ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	\
	} while(dwReadLength==0);

#define ReadCommChar()										\
	ReadFile(hComm, &RxChar, 1, &dwReadLength, NULL);	

#define WriteCommChar(btTxChar)								\
	TxChar =btTxChar;										\
	WriteFile(hComm, &TxChar, 1, &dwWrittenLength, NULL);

#define TranCommChar(btTxChar)								\
	TransmitCommChar(hComm, btTxChar);

#define WaitEvent(WaitEvent)								\
	do{														\
		WaitCommEvent(hComm, &dwEvtMask, NULL );			\
	}while((dwEvtMask & WaitEvent) != WaitEvent);

#define GetInQueueLength(InQueueLength)						\
	ClearCommError( hComm, &dwError, &ComStat );			\
	InQueueLength = ComStat.cbInQue;

#define ReadCommBlock(nReadLength)							\
	ReadFile(hComm, (LPVOID)lpRxPointer, nReadLength , &dwReadLength, NULL);\
	lpRxPointer +=dwReadLength;								\
	dwRxBufferLength+=dwReadLength;

#define WriteCommBlock(lpszData, nWriteLength)								\
	WriteFile(hComm, LPVOID(lpszData), nWriteLength , &dwWrittenLength, NULL);
 //从缓冲区中读指定长度数据，如有，则返回；否则等待数据来	
#define FetchCommBlock(nFetchLength)						\
	nIntCount =nFetchLength;								\
	do {													\
		ReadFile(hComm, (LPVOID)lpRxPointer, nIntCount , &dwReadLength, NULL);\
		lpRxPointer +=dwReadLength;							\
		dwRxBufferLength+=dwReadLength;						\
		nIntCount -=dwReadLength;							\
	}while (nIntCount>0);

#define ClearCommRx()										\
 	PurgeComm( hComm, PURGE_RXCLEAR ) ;

#define FillDevName()										\
	for (nIntCount =0; nIntCount<c_MaxItemNum; nIntCount++)	\
	strcpy(OutResult[nIntCount].InstrumentID, lpDevice->lpszDevice);

#define FillDate(nItemNO, wYears,wMonths,wDays, wHours,wMinutes,wSeconds)				\
		OutResult[nItemNO].AssayDate.wYear =wYears;			\
		OutResult[nItemNO].AssayDate.wMonth=wMonths;		\
		OutResult[nItemNO].AssayDate.wDay	=wDays;			\
		OutResult[nItemNO].AssayDate.wHour	=wHours;		\
		OutResult[nItemNO].AssayDate.wMinute=wMinutes;		\
		OutResult[nItemNO].AssayDate.wSecond=wSeconds;
#define FillWorkingID(lpOpPointer, Length, nItemNo)          \
		strncpy(OutResult(nItemNo).WorkingID,lpOpPointer,Length);            \
		OutResult[nItemNo].WorkingID[Length]='\0';
#define FillSampleID(nItemNO, SampeID)						\
		OutResult[nItemNO].SampleID =SampeID;
#define FillResult(nItemNO, lpOpPointer, ResultLength)						\
		strncpy(OutResult[nItemNO].AssayResult, lpOpPointer, ResultLength);	\
		OutResult[nItemNO].AssayResult[ResultLength] ='\0';
	
#define FillItemName(nItemNO, lpOpPointer, ItemNameLength)	\
		strncpy(OutResult[nItemNO].ItemID, lpOpPointer, ItemNameLength);	\
		OutResult[nItemNO].ItemID[ItemNameLength] ='\0';

#define FillResultType(nItemNO, btResultType)				\
		OutResult[nItemNO].ResultType =btResultType;

#define FillErrorCode(nItemNO, btErrorCode)					\
		OutResult[nItemNO].ErrorCode =btErrorCode;

#define FillComment(nItemNO, lpOpPointer, CommentLength)		\
		memcpy(OutResult[nItemNO].Comment, lpOpPointer, CommentLength);	\
		OutResult[nItemNO].Comment[CommentLength] ='\0';

#define IncLine(lpOpPointer)									\
		lpOpPointer =strchr(lpOpPointer, c_LineFeedChar)+1;

#define MoveLines(lpPointer, Lines)								\
		if (lpPointer!=NULL)									\
			for (nIntCount=0; nIntCount<Lines; nIntCount++)		\
			{													\
				lpPointer =strchr(lpPointer, c_LineFeedChar);	\
				if (lpPointer==NULL)							\
					break;										\
				 lpPointer++;									\
			}
		
#define MoveRows(lpOpPointer, nRows)							\
		lpOpPointer +=nRows;


#define	DeleZero(_lpPointer, _ResultLen)		\
 				lpPointer=_lpPointer+nDataOff+c_ResultOff;	\
				nResultLen=_ResultLen;	\
 		for( ; *lpPointer=='0'; nResultLen--, lpPointer++)		\
		{								\
			if (*(lpPointer+1)!='0')	\
			{							\
				if (*(lpPointer+1)!='.')	\
				{						\
					lpPointer++;		\
					nResultLen--;		\
					break;				\
				}						\
				else break;				\
			}							\
		}	\
 	if (nResultLen==0)	\
	{	\
		lpPointer--;	\
		nResultLen++;	\
	}
#define DeleSpace(_ResultLen)	\
			for(nResultLen =_ResultLen; *lpPointer==SPACE; nResultLen--, lpPointer++); //去掉前导空格