/****************************************************************************

    PROGRAM: DOGDEMO

    PURPOSE: This application demonstrates how to invoke the Microdog's
             API for Win32 Application
             
    Copyright 1994-1998 Beijing Goldensoft Co.,Ltd. All Rights Reserved

****************************************************************************/

#include "windows.h"		    /* required for all Windows applications */
#include "string.h"
#include "stdio.h"
#include "dogdemo.h"                /* specific to this program              */
#include "resource.h"

#include "gsmh.h"


HANDLE hInst;

/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

****************************************************************************/

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    DLGPROC lpDialogProc;
	HINSTANCE mlib;

	mlib = LoadLibrary(TEXT("SETUPdll.dll"));
		
    if(mlib != NULL)
	  (FARPROC)InstDriver = GetProcAddress(mlib, "InstDriver");
	else
	{
	  MessageBox(hInstance,"SETUPDLL.DLL not found!","Error",MB_OK);
	  return 0;
	}

    hInst = hInstance;

    lpDialogProc = MakeProcInstance(DemoDlg, hInstance);

    DialogBox(hInstance,(LPCSTR)IDD_DIALOG, NULL,lpDialogProc);

    FreeProcInstance(lpDialogProc);

    return 0;
}


/****************************************************************************

    FUNCTION: About(HWND, unsigned, WORD, LONG)

****************************************************************************/

BOOL FAR PASCAL About(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
	CenterWindow(hDlg);
        return (TRUE);

    case WM_COMMAND:                  
	if (wParam == IDOK|| wParam == IDCANCEL) {
	   EndDialog(hDlg, TRUE);
	   return (TRUE);
	}
	break;
    }
    return (FALSE);                       
}




/****************************************************************************

    FUNCTION: OnDialogInit (HWND)

****************************************************************************/

void OnDialogInit(HWND hDlg)
{
	HMENU hSysMenu;

	hSysMenu = GetSystemMenu(hDlg,FALSE);
    AppendMenu (hSysMenu,MF_SEPARATOR,0,0);
    AppendMenu (hSysMenu,MF_STRING,IDM_ABOUT,"About...");
    RemoveMenu (hSysMenu,SC_SIZE,MF_BYCOMMAND);
    RemoveMenu (hSysMenu,SC_MAXIMIZE,MF_BYCOMMAND);
	RemoveMenu (hSysMenu,SC_MINIMIZE,MF_BYCOMMAND);
	
	CenterWindow(hDlg);
}


/****************************************************************************

    FUNCTION: OnButtonWriteClicked (HWND)

****************************************************************************/

void OnButtonInstDriver(HWND hDlg) 
{
	int RetCode;
	char Message[100];

	RetCode = InstDriver(1,0);
	if(RetCode)
	{
		wsprintf (Message, "Install dirver failed.\r error code = %d",RetCode);
		SetWindowText ( GetDlgItem( hDlg, IDC_STATIC_RESULT ), Message);
	}
	else
	{
		wsprintf (Message, " The dirver has been successful installed .");
		SetWindowText ( GetDlgItem( hDlg, IDC_STATIC_RESULT ), Message);
	}
}



/****************************************************************************

    FUNCTION: DemoDlg(HWND, unsigned, WORD, LONG)

****************************************************************************/

BOOL FAR PASCAL DemoDlg(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
	FARPROC lpProcAbout;
    
    switch (message)
    {

    case WM_INITDIALOG:
        OnDialogInit(hDlg);
		return (TRUE);

    case WM_COMMAND:
		switch(wParam)
        {
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return (TRUE);
		
		case IDC_BUTTON_INSTALL:
			OnButtonInstDriver(hDlg);
			break;
		
		}
		break;

	case WM_SYSCOMMAND:
        if (wParam==IDM_ABOUT)
        {
			lpProcAbout = MakeProcInstance(About, hInst);
			DialogBox(hInst,"AboutBox",hDlg,lpProcAbout);
			FreeProcInstance(lpProcAbout);
        }
        break;
    }
    return (FALSE);                       
}

void CenterWindow(HWND hWnd)
{
    RECT rect;
    int systemX,systemY;
    int newX,newY;
	
    GetWindowRect(hWnd,&rect);
    systemX=GetSystemMetrics(SM_CXFULLSCREEN);
    systemY=GetSystemMetrics(SM_CYFULLSCREEN);
	
    newX = (systemX-(rect.right-rect.left))/2;
    newY = (systemY-(rect.bottom-rect.top))/2;

    SetWindowPos(hWnd,NULL,newX,newY,0,0,SWP_NOSIZE);
}
