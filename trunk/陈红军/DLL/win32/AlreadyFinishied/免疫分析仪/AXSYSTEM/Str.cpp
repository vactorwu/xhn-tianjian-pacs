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

//���ASTM��׼������ĺ���,ȡ��Ҫ�Ľ������
//	lpStr��Record_Type_ID��ʼ
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
void  GetItemStr( char * lpStr, LPSTR  lpDes, int Position,int pos2)
{
	bool    ibexist;
	int     i;
	int     j;
	int     count,count1;
	int     strlength;	
	
	ibexist = FALSE;
	count1=count = 0;
	j = 0;	
	strlength = strlen(lpStr);
	for(i=0;i<strlength;i++)
	{
		if(*(lpStr+i) == '|')
		{
			ibexist = TRUE;
			count++;
		}
		if(*(lpStr+i) == 0x5e)
		{
			count1++;;

		}
		if(count >= Position)
		{
			lpDes[j] =*(lpStr+i-1);
			lpDes[j+1]='\0';
			break;
		}
		if((count == Position - 1)&&(ibexist == TRUE)&&(*(lpStr+i+1)!= '|')&&(*(lpStr+i+1)!= '^'))
		{
			if((count1 == pos2 - 1)&&(ibexist == TRUE)&&(*(lpStr+i+1)!= '|')&&(*(lpStr+i+1)!= '^'))
			{
				lpDes[j] = *(lpStr+i+1);
				j++;			
			}
		}		
		lpDes[j] = '\0';
	}	
}
void  GetSampleStr( char * lpStr, LPSTR  lpDes, int Position,int pos2)
{
	bool    ibexist;
	int     i;
	int     j;
	int     count,count1;
	int     strlength;	
	
	ibexist = FALSE;
	count1=count = 0;
	j = 0;	
	strlength = strlen(lpStr);
	for(i=0;i<strlength;i++)
	{
		if(*(lpStr+i) == '|')
		{
			ibexist = TRUE;
			count++;
		}
		if(*(lpStr+i) == 0x5e)
		{
			count1++;;

		}
		if(count >= Position)
		{
		//	lpDes[j] =*(lpStr+i-1);
		//	lpDes[j+1]='\0';
			break;
		}
		if((count == Position - 1)&&(ibexist == TRUE)&&(*(lpStr+i+1)!= '|')&&(*(lpStr+i+1)!= '^'))
		{
			if((count1 == pos2 - 1)&&(ibexist == TRUE)&&(*(lpStr+i+1)!= '|')&&(*(lpStr+i+1)!= '^'))
			{
				lpDes[j] = *(lpStr+i+1);
				j++;			
			}
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
	

