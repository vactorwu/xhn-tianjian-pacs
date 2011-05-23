#include "stdafx.h"
#include <Winbase.h>
#include "logFile.h"
HANDLE hLogFile;
bool NewLogFile(LPSTR  filename)
{
	hLogFile=CreateFile(filename,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hLogFile!=HANDLE(-1))
		
		return TRUE;
	else 
	{
		MessageBox(NULL,"文件创建失败","文件错误",MB_OK);
		return FALSE;
	}
	
}
void WriteLogData(HANDLE hFile,LPSTR Buffer,int size)
{
	DWORD dwLength;
	WriteFile(hFile,Buffer,size,&dwLength,NULL);
}
void CloseLogFile(HANDLE hFile)
{
	CloseHandle(hFile);
}