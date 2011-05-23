
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
	DWORD   * lpDeviceResultCount; //采集结果仪器的结果数
	//指向Device结果中的dwRecordCount
	CHAR	InstrumentID[c_MaxDevNameLength+1];//仪器ID
	CHAR    DevItemType;//仪器分类
	WORD	SampleID;//结果编号数字型
	CHAR    WorkingID[c_MaxWorkingIDLength+1];//样本号(工作单号),chj
	BYTE    ItemNo;//仪器序号
	CHAR	ItemID[c_MaxItemNameLength+1];//仪器项目编号
	CHAR	AssayResult[c_MaxResultLength+1];//检验结果值
	SDATE	AssayDate;//检验日期
	BYTE	ResultType;//结果类型
	BYTE	ErrorCode;//错误码
	CHAR	Comment[c_MaxCommentLength+1];//注释
}RESULT, *PRESULT;
typedef PRESULT	LPRESULT;

typedef BOOL (WINAPI *PRESULT_PROCESS_ROUTINE) (int nDevNO, LPRESULT lpResult, int nItemNum);

typedef PRESULT_PROCESS_ROUTINE 	LPRESULT_PROCESS_ROUTINE;

typedef struct _DEVICE
{
	int			nDevNO;		//设备编号					 
	CHAR		lpszDevice[c_MaxDevNameLength+1];//设备编号
	HANDLE		hComm	;						//串口句柄
	HINSTANCE	hDriver	;					//动态库句柄
	HANDLE		hMonitorThread	;		//监控线程		
	LPRESULT_PROCESS_ROUTINE		 //结果处理函数指针
		lpResultProcessRoutine ;		
	DWORD		dwRecordCount;	   //结果数
	BOOL		bAutoLoad;			 //是否为自动装载
	int			nStatus;				//仪器状态
	CHAR    DevItemType;				//仪器分类
} DEVICE , *PDEVICE;

typedef PDEVICE LPDEVICE;

typedef struct _COMMAND
{
	LPDEVICE	lpDevice;
	unsigned char	lpszDataFileName[c_MaxFilePathLength+1];
} COMMAND, *PCOMMAND;

typedef PCOMMAND LPCOMMAND;
#endif