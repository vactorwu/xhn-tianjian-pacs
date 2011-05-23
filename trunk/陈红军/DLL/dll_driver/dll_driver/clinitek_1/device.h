
#ifndef _DEVICE_H_
#define _DEVICE_H_
#include <windows.h>
#include "const.h"
 
#define	c_MaxItemNameLength		8
#define	c_MaxSampleIDLength		5
#define	c_MaxResultLength		20
#define	c_MaxCommentLength		8


typedef DWORD (WINAPI * PCOMM_MONITOR_ROUTINE)( 
	LPVOID lpData);
typedef PCOMM_MONITOR_ROUTINE LPCOMM_MONITOR_ROUTINE;

typedef DWORD (WINAPI * PBEFORE_SETCOMMSTATE_ROUTINE)(
	DCB * pComDCB );
typedef PBEFORE_SETCOMMSTATE_ROUTINE LPBEFORE_SETCOMMSTATE_ROUTINE;

typedef DWORD (WINAPI * PSEND_COMMAND_ROUTINE)(
	DCB * pComDCB );
typedef PSEND_COMMAND_ROUTINE LPSEND_COMMAND_ROUTINE;

typedef struct _DATE
{
	WORD	wYear;
	WORD	wMonth;
	WORD	wDay;
	WORD	wHour;
	WORD	wMinute;
	WORD	wSecond;
}SDATE;

typedef struct _RESULT
{
	CHAR	InstrumentID[c_MaxDevNameLength+1];
	WORD	SampleID;
	CHAR	ItemID[c_MaxItemNameLength+1];
	CHAR	AssayResult[c_MaxResultLength+1];
	SDATE	AssayDate;
	BYTE	ResultType;
	BYTE	ErrorCode;
	CHAR	Comment[c_MaxCommentLength+1];
}RESULT, *PRESULT;
typedef PRESULT	LPRESULT;

typedef BOOL (WINAPI *PRESULT_PROCESS_ROUTINE) (int nDevNO, LPRESULT lpResult, int nItemNum);

typedef PRESULT_PROCESS_ROUTINE 	LPRESULT_PROCESS_ROUTINE;

typedef struct _DEVICE
{
	int			nDevNO;							 
	CHAR		lpszDevice[c_MaxDevNameLength+1];
	HANDLE		hComm	;						
	HINSTANCE	hDriver	;					
	HANDLE		hMonitorThread	;				
	LPRESULT_PROCESS_ROUTINE
		lpResultProcessRoutine ;		
	DWORD		dwRecordCount;
	BOOL		bAutoLoad;
	int			nStatus;
} DEVICE , *PDEVICE;

typedef PDEVICE LPDEVICE;

typedef struct _COMMAND
{
	LPDEVICE	lpDevice;
	unsigned char	lpszDataFileName[c_MaxFilePathLength+1];
} COMMAND, *PCOMMAND;

typedef PCOMMAND LPCOMMAND;
#endif