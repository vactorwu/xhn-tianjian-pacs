#include	"str.h"
#include	"stdafx.h"
#include	"Const.h"
#include	<stdlib.h>

extern  char     Field_delimiter;
extern  char     component_delimiter;
extern  char     repeat_delimiter;
extern  char     escape_character;

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

//针对ASTM标准而定义的函数,取需要的结果数据
//	lpStr以Record_Type_ID开始
void  GetResultStr( char * lpStr, LPSTR  lpDes, int Position)
{
	bool    ibexist;
	int     i;
	int     j;
	int     count;
	int     strlength;	

	ibexist = FALSE;
	count = 0;
	j = 0;	
	strlength = strlen(lpStr);
	for(i=0;i<strlength;i++)
	{
		if(*(lpStr+i) == '|')
		{
			ibexist = TRUE;
			count++;
		}
		if(count >= Position) break;
		if((count == Position - 1)&&(ibexist == TRUE)&&(*(lpStr+i+1)!= '|')&&(*(lpStr+i+1)!= '^'))
		{
			lpDes[j] = *(lpStr+i+1);
			j++;			
		}		
		lpDes[j] = '\0';
	}	
}

void	SubString(char * lpStr, LPSTR  lpDes, int  begin_pos,int  end_pos)
{	
	int     i;
	for(i=begin_pos;i<=end_pos;i++)
	{
		lpDes[i - begin_pos] = *(lpStr+i);
	}
	lpDes[i - begin_pos] = '\0';
}
	

