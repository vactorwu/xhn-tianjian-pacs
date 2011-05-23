#include <string.h>
#include <stdio.h>
double StrToFloat(const char * lpStr, int nMaxLength);
int HStrToInt(const char * lpStr, int nMaxLength);
int StrToInt(const char * lpStr, int nMaxLength);
int FindStrStart(const char * lpStr, int nMaxLength);
void IntToStr( char * lpStr,int Value);
LPSTR  strchrnb(char * lpStr,const char cChartofind,int startpoint, int length);
void  ReplaceAllCharInStr(char * lpStr,const cCharBeReplace,const cCharReplacer,int Length);