
int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long FAR PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FAR PASCAL About(HWND, unsigned, WORD, LONG);
BOOL FAR PASCAL DemoDlg(HWND hDlg, unsigned message, WORD wParam, LONG lParam);

void OnDialogInit(HWND hDlg);
void CenterWindow(HWND hWnd);



