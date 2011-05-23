#include "str.h"
#include "stdafx.h"
#include <stdlib.h>
int StrToInt(const char * lpStr, int nMaxLength)
{
	int nCount, nResult;
	for (nCount=0; nCount<nMaxLength	&& lpStr[nCount] !=0x00
			&& (lpStr[nCount] ==0x09 || lpStr[nCount] ==0x20 || lpStr[nCount] ==0x30);
			nCount++);

	for (nResult =0; lpStr[nCount] !=0x00	&&	lpStr[nCount] !=0x09
			&&	lpStr[nCount] !=0x20 &&  nCount<nMaxLength;
			nCount++)
	{
		if (lpStr[nCount]<'0' || lpStr[nCount]>'9')
			break;
		nResult *=10;
		nResult +=(lpStr[nCount]-0x30);
	}
	return 	nResult;
}
int FindStrStart(const char * lpStr, int nMaxLength)
{
	//查找工作单号的起始位置
	int nCount;
	for (nCount=0; nCount<nMaxLength	&& lpStr[nCount] !=0x00
			&& (lpStr[nCount] ==0x09 || lpStr[nCount] ==0x20 || lpStr[nCount] ==0x30);
			nCount++);
	
	return 	nCount;
}

int HStrToInt(const char * lpStr, int nMaxLength)
{
	int nCount, nResult;
	for (nCount=0; nCount<nMaxLength	&& lpStr[nCount] !=0x00
			&& (lpStr[nCount] ==0x09 || lpStr[nCount] ==0x20 || lpStr[nCount] ==0x30);
			nCount++);

	for (nResult =0; lpStr[nCount] !=0x00 && lpStr[nCount] !=0x09
			&&	lpStr[nCount] !=0x20 &&  nCount<nMaxLength;
			nCount++)
	{
		if (!((lpStr[nCount]>='0' && lpStr[nCount]<='9')||
			(lpStr[nCount]>='A' && lpStr[nCount]<='Z')||
			(lpStr[nCount]>='a' && lpStr[nCount]<='z')))
			break;

		if (lpStr[nCount]>='a')
			nResult = nResult*16+(lpStr[nCount]-0x57);	
		else
			if (lpStr[nCount]>='A')
				nResult =nResult*16+(lpStr[nCount]-0x37);
			else
				nResult =nResult*16+(lpStr[nCount]-0x30);
	}
	return nResult;
}
//在字符串中定位指定的字符：
//char * lpStr ：指定的字符串
//const char cChartofind ：要查找的字符
//int Range ：查找的范围
//int Numbers ：查找字符为从当前位置起的第Numbers 个指定字符
//返回值：若找到，返回偏移值，否则，返回 －1。
int  FindCharInStr(char * lpStr,const char cChartofind,int Range,int Numbers )
{
	int point,Number;
	Number=Numbers;
	for(point=0;point<=Range;point++)
	{
		if (lpStr[point]==cChartofind)
		{
			Number--;
			if(Number==0)
				return point;
		}
	}
	return -1;
}
double StrToFloat(const char * lpStr, int nMaxLength)
{
	int nCount, nDotCount;
	int bIsNegative=0;
	double dResult;
	for (nCount=0; nCount<nMaxLength && (lpStr[nCount] ==0x09
				|| lpStr[nCount] ==0x20	|| lpStr[nCount] ==0x30);
				nCount++);
	if (nCount<nMaxLength && lpStr[nCount]=='-')
	{
		nCount++;
		bIsNegative =1;
		for (; nCount<nMaxLength && (lpStr[nCount] ==0x09
				|| lpStr[nCount] ==0x20	|| lpStr[nCount] ==0x30);
				nCount++);
	}

	for (nDotCount =0, dResult =float(0); lpStr[nCount] !=0x00
			&&  lpStr[nCount] !=0x09 &&	lpStr[nCount] !=0x20 &&  nCount<nMaxLength;
			nCount++)
		{
		if (lpStr[nCount] =='.')
		{
		   nDotCount=10;
		   continue;
		}
		
		if (lpStr[nCount]<'0' || lpStr[nCount]>'9')
			break;
	
		if (nDotCount>0)
		{
		   dResult +=(float(lpStr[nCount]-0x30)/nDotCount);
		   nDotCount *=10;
		}
		else
		{
		   dResult *=10;
		   dResult +=(lpStr[nCount]-0x30);
		}
	}	
	if (bIsNegative)
		dResult =-dResult;
	return dResult;
}
//将整形数转化为字符串
void IntToStr(char * lpStr,int Value)
{
	_itoa(Value,lpStr, 10);
}
//查找字符在缓冲区中的的起始位置
LPSTR  strchrnb(char * lpStr,const char cChartofind,int startpoint, int length)
{
	int point;
	for(point=0;point<=length;point++)
	{
		if (lpStr[point]==cChartofind)
			return lpStr+point;
	}
	return NULL;
}
 




