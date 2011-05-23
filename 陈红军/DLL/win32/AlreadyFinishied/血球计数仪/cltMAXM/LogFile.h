bool NewLogFile(HANDLE& hLogFile, LPSTR  filename);
void WriteLogData(HANDLE hFile,LPSTR Buffer,int size);
void WriteLogChar(HANDLE hFile,char bChar);
void CloseLogFile(HANDLE hFile);