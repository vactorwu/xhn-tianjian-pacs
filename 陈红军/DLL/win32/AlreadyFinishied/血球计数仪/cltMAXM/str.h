#include <string.h>
#include <stdio.h>
#include <stdlib.h>
double StrToFloat(const char * lpStr, int nMaxLength);
int HStrToInt(const char * lpStr, int nMaxLength);
int HexToInt(char bChar);
int StrToInt(const char * lpStr, int nMaxLength);
int FindStrStart(const char * lpStr, int nMaxLength);
void IntToStr( char * lpStr,int Value);
LPSTR  strchrnb( char * lpStr,const char cChartofind,int startpoint, int length);
LPSTR  strchrnbNotNull(char * lpStr,const char cChartofind,int startpoint, int length);
