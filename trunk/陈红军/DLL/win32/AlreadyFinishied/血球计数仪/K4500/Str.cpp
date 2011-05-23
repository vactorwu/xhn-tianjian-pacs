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
	//���ҹ������ŵ���ʼλ��
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
//��������ת��Ϊ�ַ���
void IntToStr(char * lpStr,int Value)
{
	_itoa(Value,lpStr, 10);
}
//�����ַ��ڻ������еĵ���ʼλ��
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
 




