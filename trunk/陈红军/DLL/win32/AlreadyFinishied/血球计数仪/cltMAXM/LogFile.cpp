#include "stdafx.h"
#include <Winbase.h>
#include "logFile.h"

bool NewLogFile(HANDLE& hLogFile, LPSTR  filename)
{
	hLogFile=CreateFile(filename,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
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
void WriteLogChar(HANDLE hFile,char bChar)
{
	char Buffer[2];
	DWORD dwLength;
	Buffer[0] = bChar;
	WriteFile(hFile,Buffer,1,&dwLength,NULL);
}
void CloseLogFile(HANDLE hFile)
{
	CloseHandle(hFile);
}