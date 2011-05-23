#define	 CommData								\
	DEVICE  *	lpDevice;						\
	HANDLE 		hComm ;								\
	RESULT		OutResult[c_MaxItemNum];		\
	BYTE   		RxChar=0, TxChar=0;				\
	DWORD		dwRxBufferLength=0, dwReadLength=0, dwWrittenLength=0;	\
	DWORD		dwEvtMask =0;					\
	DWORD   	dwError =0;						\
	LPSTR  	    lpRxPointer=NULL;				\
	LPSTR		lpOpPointer=NULL;				\
	BYTE    	RxBuffer[c_RxBufferLength] ;	\
	LPRESULT_PROCESS_ROUTINE	lpResultProcessRoutine;				\
	int		    nIntCount;							

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

#define WaitEvent(WaitEvent)								\
	do{														\
		WaitCommEvent(hComm, &dwEvtMask, NULL );			\
	}while((dwEvtMask & WaitEvent) != WaitEvent);

#define ReadCommBlock(nReadLength)							\
	ReadFile(hComm, (LPVOID)lpRxPointer, nReadLength , &dwReadLength, NULL);\
	lpRxPointer +=dwReadLength;								\
	dwRxBufferLength+=dwReadLength;


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
		strncpy(OutResult[nItemNO].Comment, lpOpPointer, CommentLength);	\
		OutResult[nItemNO].Comment[CommentLength] ='\0';
