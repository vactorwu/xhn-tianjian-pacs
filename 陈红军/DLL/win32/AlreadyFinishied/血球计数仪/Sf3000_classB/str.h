#include <string.h>
#include <stdio.h>
#include <stdlib.h>
double StrToFloat(const char * lpStr, int nMaxLength);
int HStrToInt(const char * lpStr, int nMaxLength);
int StrToInt(const char * lpStr, int nMaxLength);
int FindStrStart(const char * lpStr, int nMaxLength);
void IntToStr( char * lpStr,int Value);
LPSTR  strchrnb( char * lpStr,const char cChartofind,int startpoint, int length);

//在字符串指定的的posi位置上插入cChartofind字符(针对SF3000补充的函数)
LPSTR  InsertStr( char * lpStr,const char cChartofind,int posit);
