 /***************************************************************************** 
 *                            MultiDesk 2001                                 *
 *                                                                           *
 *                                                                           *
 * This file is for educational purposes only.  This file is not for         *
 * commerical use. There is no warrenty, implied or otherwise.               *
 *                                                                           *
 * toby@opferman.com                                                         *
 *                                                                           *
 * By Toby Opferman                                                          *
 *                                                                           *
 *    Copyright (c)2001, All Rights Reserved                                 *
 *****************************************************************************/
 
 #include <windows.h>
 #include <mdesk.h>
 #include <mdui.h>
 #include "mdres.h"
 
/*****************************************************************************
 * Internal APIs
 *****************************************************************************/
 UINT CALLBACK MDUI_SystemTrayCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
 void MDUI_InitializeMultiDeskUI(HWND hWnd);
 void MDUI_RegisterHotkeys(HWND hWnd);
 void MDUI_UnRegisterHotkeys(HWND hWnd);
 BOOL MDUI_CreateSystemTray(HWND hWnd);
 void MDUI_CreateMultipleDesktops(HWND hWnd);
 void MDUI_ParseCommandLine(HWND hWnd);
 void MDUI_SetSystemTrayIcon(HWND hWnd, UINT IconID);
 DWORD MDUI_GetDwordRegistryValue(PCHAR RegistryKey, PCHAR RegistryValue, DWORD Default);
 void MDUI_HandleSystemTray(HWND hWnd, UINT Cmd);
 void MDUI_DisplaySystemTrayMenu(HWND hWnd);
 void MDUI_SetDesktop(HWND hWnd, UINT DesktopNumber, UINT DesktopIconId,  DWORD dwNewCheck); 
 void MDUI_QueryMultiDeskApplicationExit(HWND hWnd);
 void MDUI_HandleCommands(HWND hWnd, UINT Cmd);
 void MDUI_HandleApplicationCleanup(HWND hWnd);
 void MDUI_DestroySystemTrayIcon(HWND hWnd);
 void MDUI_ToggleSafeMode(HWND hWnd);
 void MDUI_HandleHotKeys(HWND hWnd, UINT HotKey);
 void MDUI_SwitchToNextDesktop(HWND hWnd);
 void MDUI_AdjustDesktopDisplay(HWND hWnd);
 void MDUI_SwitchToNextDesktop(HWND hWnd);
 void MDUI_SwitchToPrevDesktop(HWND hWnd); 
 void MDUI_AddUniversalWindow(HWND hWnd);
 void MDUI_RemoveUniversalWindow(HWND hWnd);
 BOOL CALLBACK MDUI_AboutBoxProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
 void MDUI_HandleAboutBoxCommands(HWND hWnd, UINT Cmd);
 void MDUI_HandleAboutBoxTimer(HWND hWnd);
 void MDUI_InitializeAboutDialog(HWND hWnd);
 void MDUI_CheckForSafeModeRegKey(HWND hWnd);
 UINT MDUI_MessageBox(HINSTANCE hInstance, HWND hWnd, UINT TitleID, UINT MsgID, UINT Flags);


 
/*****************************************************************************
 * MDUI_CreateMdeskClass
 *
 * Puropose:
 *      Register the window class
 * 
 * Input:
 *      Application Instance
 * Output:
 *      Class Atom, 0 for failure
 *****************************************************************************/
 ATOM WINAPI MDUI_CreateMdeskClass(HINSTANCE hInstance)
 {
    WNDCLASSEX WndEx = {0};
     
    WndEx.cbSize         = sizeof(WndEx); 
    WndEx.style          = 0;  
    WndEx.lpfnWndProc    = MDUI_SystemTrayCallback; 
    WndEx.cbClsExtra     = 0; 
    WndEx.cbWndExtra     = 0; 
    WndEx.hInstance      = hInstance; 
    WndEx.hIcon          = 0; 
    WndEx.hCursor        = 0; 
    WndEx.hbrBackground  = 0; 
    WndEx.lpszMenuName   = 0; 
    WndEx.lpszClassName  = MDUI_CLASS_NAME; 
    WndEx.hIconSm        = 0;

    return RegisterClassEx(&WndEx);
 }



/*****************************************************************************
 * MDUI_CreateMdeskWindow
 *
 * Puropose:
 *      Create the application window
 * 
 * Input:
 *      Application Instance, Class Atom
 * Output:
 *      Handle to a created window
 *****************************************************************************/
 HWND WINAPI MDUI_CreateMdeskWindow(HINSTANCE hInstance, ATOM WinClass)
 {
    HWND hWnd = NULL;
    
    if(!FindWindow(MDUI_CLASS_NAME, NULL))
    {
        hWnd = CreateWindowEx(0, (char *)WinClass, NULL, 0, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                              CW_USEDEFAULT, HWND_DESKTOP, NULL, hInstance, NULL);
    }
                          
    return hWnd;
 }
 
 
 /*****************************************************************************
 * MDUI_MessageLoop
 *
 * Puropose:
 *      Dispatch Messages to a created window
 * 
 * Input:
 *      Handle To Window
 * Output:
 *      OS Error code
 *****************************************************************************/
 int WINAPI MDUI_MessageLoop(HWND hWnd)
 {
     MSG Msg;
     
     UpdateWindow(hWnd);
     ShowWindow(hWnd, SW_HIDE);
     
     while(GetMessage(&Msg, 0, 0, 0))
     {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
     }
     
     return Msg.wParam;
 }
 
  
 
 
/*****************************************************************************
 * MDUI_UnRegisterUIClass
 *
 * Puropose:
 *      Unregister A Created Window Class
 * 
 * Input:
 *      Application Instance, Class Atom
 * Output:
 *      Nothing
 *****************************************************************************/
 void WINAPI MDUI_UnRegisterUIClass(HINSTANCE hInstance, ATOM ClassAtom)
 {
    UnregisterClass((char *)ClassAtom, hInstance);
 }
 
 


/*****************************************************************************
 * MDUI_SystemTrayCallback
 *
 * Puropose:
 *      Window Callback
 * 
 * Input:
 *      Window Handle, Message, wParam, lParam
 * Output:
 *      Handled value
 *****************************************************************************/
UINT CALLBACK MDUI_SystemTrayCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
        DWORD dwRetVal = 0;
        
        switch(Msg)
        {
            case WM_CREATE:
                 MDUI_InitializeMultiDeskUI(hWnd);
                 break;
             
            case WM_MD_SYSTEM_TRAY:
                 MDUI_HandleSystemTray(hWnd, LOWORD(lParam));
                 break;
             
            case WM_HOTKEY:
                 MDUI_HandleHotKeys(hWnd, LOWORD(wParam));
                 break;
                  
            case WM_COMMAND:
                 MDUI_HandleCommands(hWnd, LOWORD(wParam));
                 break;
             
            case WM_DESTROY:
                 MDUI_HandleApplicationCleanup(hWnd);            
                 break;
                                  
            default:
                 dwRetVal = DefWindowProc(hWnd, Msg, wParam, lParam);
        }
        
        return dwRetVal;
}



/*****************************************************************************
 * MDUI_HandleHotKeys
 *
 * Puropose:
 *      Handle Application Hot Keys
 * 
 * Input:
 *      Window Handle, Hot Key ID
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleHotKeys(HWND hWnd, UINT HotKey)
{
        switch(HotKey)
        {
             case IDC_HOTKEY_DPSWITCH_FWD:
                  MDUI_SwitchToNextDesktop(hWnd);
                  break;
                  
             case IDC_HOTKEY_DPSWITCH_BCK:
                  MDUI_SwitchToPrevDesktop(hWnd);
                  break;
                  
             case IDC_HOTKEY_UNIVERSAL_ON:
                  MDUI_AddUniversalWindow(hWnd);
                  break;
                 
             case IDC_HOTKEY_UNIVERSAL_DOWN:
                  MDUI_RemoveUniversalWindow(hWnd);  
                  break;
        }
}



/*****************************************************************************
 * MDUI_AdjustDesktopDisplay
 *
 * Puropose:
 *      Adjust Desktop Display
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_AdjustDesktopDisplay(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
    UINT iCheckMarkList[] = { IDC_DT_ONE, IDC_DT_TWO, IDC_DT_THREE, IDC_DT_FOUR,
                              IDC_DT_FIVE, IDC_DT_SIX, IDC_DT_SEVEN, IDC_DT_EIGHT  };
    UINT iIcons[] = { IDC_DESKTOPONE_ICON, IDC_DESKTOPTWO_ICON, 
                      IDC_DESKTOPTHREE_ICON, IDC_DESKTOPFOUR_ICON,
                      IDC_DESKTOPFIVE_ICON, IDC_DESKTOPSIX_ICON, 
                      IDC_DESKTOPSEVEN_ICON, IDC_DESKTOPEIGHT_ICON } ;
    DWORD dwCurrentDesktop; 
                      
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
    
    dwCurrentDesktop = MDesk_GetCurrentDesktop(pMDUIStruc->hMdesk);
    
    MDUI_SetSystemTrayIcon(hWnd, iIcons[dwCurrentDesktop]);
     
    CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), pMDUIStruc->dwCurrentCheck, MF_UNCHECKED);
    pMDUIStruc->dwCurrentCheck = iCheckMarkList[dwCurrentDesktop];
    CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), pMDUIStruc->dwCurrentCheck, MF_CHECKED);    
             
}


/*****************************************************************************
 * MDUI_SwitchToNextDesktop
 *
 * Puropose:
 *      Switch to Next Desktop
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_SwitchToNextDesktop(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
          
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
    
    MDesk_SwitchDesktopForward(pMDUIStruc->hMdesk);
    
    MDUI_AdjustDesktopDisplay(hWnd);
            
}


/*****************************************************************************
 * MDUI_SwitchToPrevDesktop
 *
 * Puropose:
 *      Switch To Previous Desktop
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_SwitchToPrevDesktop(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
          
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);        
    
    MDesk_SwitchDesktopBackward(pMDUIStruc->hMdesk);
    
    MDUI_AdjustDesktopDisplay(hWnd);
}


/*****************************************************************************
 * MDUI_AddUniversalWindow
 *
 * Puropose:
 *      Add Universal Window
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_AddUniversalWindow(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
          
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
    MDesk_AddUniversalWindow(pMDUIStruc->hMdesk, GetForegroundWindow());        
}

/*****************************************************************************
 * MDUI_RemoveUniversalWindow
 *
 * Puropose:
 *      Remove Universal Window
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_RemoveUniversalWindow(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
          
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);        
    
    MDesk_RemoveUniversalWindow(pMDUIStruc->hMdesk, GetForegroundWindow());
}

/*****************************************************************************
 * MDUI_HandleApplicationCleanup
 *
 * Puropose:
 *      Handle Application Clean up
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleApplicationCleanup(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc = NULL;
          
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);        
    SetWindowLong(hWnd, GWL_USERDATA, 0);
    
    MDesk_DestroyDesktop(pMDUIStruc->hMdesk);
    
    MDUI_UnRegisterHotkeys(hWnd);
    
    LocalFree(pMDUIStruc);
    
    MDUI_DestroySystemTrayIcon(hWnd);
    
    PostQuitMessage(0);
}

/*****************************************************************************
 * MDUI_HandleSystemTray
 *
 * Puropose:
 *      Handle System Tray Actions Menu
 * 
 * Input:
 *      Window Handle, Command ID
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleSystemTray(HWND hWnd, UINT Cmd)
{
        switch(Cmd)
        {
            case WM_RBUTTONDOWN:
                 MDUI_DisplaySystemTrayMenu(hWnd);
                 break;
        }
}


/*****************************************************************************
 * MDUI_HandleCommands
 *
 * Puropose:
 *      Handle System Tray Menu Commands
 * 
 * Input:
 *      Window Handle, Command ID
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleCommands(HWND hWnd, UINT Cmd)
{
     switch(Cmd)
     {
        case IDC_SAFE_MODE:
             MDUI_ToggleSafeMode(hWnd);
             break;
        
        case IDC_DT_ONE:
             MDUI_SetDesktop(hWnd, 0, IDC_DESKTOPONE_ICON, Cmd);
             break;
        
        case IDC_DT_TWO:
             MDUI_SetDesktop(hWnd, 1, IDC_DESKTOPTWO_ICON, Cmd);
             break;
        
        case IDC_DT_THREE:
             MDUI_SetDesktop(hWnd, 2, IDC_DESKTOPTHREE_ICON, Cmd);
             break;
        
        case IDC_DT_FOUR:
             MDUI_SetDesktop(hWnd, 3, IDC_DESKTOPFOUR_ICON, Cmd);
             break;
        case IDC_DT_FIVE:
             MDUI_SetDesktop(hWnd, 4, IDC_DESKTOPFIVE_ICON, Cmd);
             break;
        
        case IDC_DT_SIX:
             MDUI_SetDesktop(hWnd, 5, IDC_DESKTOPSIX_ICON, Cmd);
             break;
        
        case IDC_DT_SEVEN:
             MDUI_SetDesktop(hWnd, 6, IDC_DESKTOPSEVEN_ICON, Cmd);
             break;
        
        case IDC_DT_EIGHT:
             MDUI_SetDesktop(hWnd, 7, IDC_DESKTOPEIGHT_ICON, Cmd);
             break;
             
        case IDC_UNI_CLEAR:
             MDesk_DestroyAllUniversalWindows(((PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA))->hMdesk);
             break;
             
        case IDC_MDHELP:
             WinHelp(hWnd, MD_HELPSTRING, HELP_CONTENTS, 0);
             break;
             
        case IDC_ABOUT:
             DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, MDUI_AboutBoxProc);
             break;
        
        case IDC_EXIT:
             MDUI_QueryMultiDeskApplicationExit(hWnd);
             break;
     }
}

/*****************************************************************************
 * MDUI_SetDesktop
 *
 * Puropose:
 *      Set A specific desktop
 * 
 * Input:
 *      Window Handle, Desktop Number, Desktop Icon ID, New Check mark
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_SetDesktop(HWND hWnd, UINT DesktopNumber, UINT DesktopIconId, DWORD dwNewCheck)
{
     PMDUISTRUC pMDUIStruc;
          
     pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);        
     
     MDesk_GoToDesktop(pMDUIStruc->hMdesk, DesktopNumber);
     MDUI_SetSystemTrayIcon(hWnd, DesktopIconId);
     
     CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), pMDUIStruc->dwCurrentCheck, MF_UNCHECKED);    
     pMDUIStruc->dwCurrentCheck = dwNewCheck;
     CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), pMDUIStruc->dwCurrentCheck, MF_CHECKED);    
     
     
}

/*****************************************************************************
 * MDUI_ToggleSafeMode
 *
 * Puropose:
 *      Toggle Between Safe mode
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_ToggleSafeMode(HWND hWnd)
{
     PMDUISTRUC pMDUIStruc;
     DWORD dwFlags;
          
     pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);        
     
     dwFlags = MDesk_GetFlags(pMDUIStruc->hMdesk);
     
     dwFlags ^= MDESK_SAFEMODE_FLAG;
     
     if(dwFlags)
        CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), IDC_SAFE_MODE, MF_CHECKED);    
     else
        CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), IDC_SAFE_MODE, MF_UNCHECKED);    
     
     MDesk_SetFlags(pMDUIStruc->hMdesk, dwFlags);
     
}

/*****************************************************************************
 * MDUI_QueryMultiDeskApplicationExit
 *
 * Puropose:
 *      Query if should quit multidesk
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_QueryMultiDeskApplicationExit(HWND hWnd)
{
    if(MDUI_MessageBox(GetModuleHandle(NULL), hWnd, IDS_EXIT_TITLE, IDS_EXIT_QUESTION, MB_YESNO | MB_ICONQUESTION) == IDYES)
       DestroyWindow(hWnd);
}

/*****************************************************************************
 * MDUI_DisplaySystemTrayMenu
 *
 * Puropose:
 *      Display System Tray Actions Menu
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_DisplaySystemTrayMenu(HWND hWnd)
{
    PMDUISTRUC pMDUIStruc;
    POINT CursorPosition;
     
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
    
    GetCursorPos(&CursorPosition);
    SetForegroundWindow(hWnd);
    TrackPopupMenu(GetSubMenu(pMDUIStruc->hPopupMenu, 0), TPM_LEFTBUTTON | TPM_CENTERALIGN, CursorPosition.x, CursorPosition.y, 0, hWnd, 0);
}

/*****************************************************************************
 * MDUI_InitializeMultiDeskUI
 *
 * Puropose:
 *      Initialize Window
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_InitializeMultiDeskUI(HWND hWnd)
{
   PMDUISTRUC pMDUIStruct = NULL;
   
   if(pMDUIStruct = LocalAlloc(LMEM_ZEROINIT, sizeof(MDUISTRUC)))
   {
       pMDUIStruct->hInstance = GetModuleHandle(NULL);
       SetWindowLong(hWnd, GWL_USERDATA, (ULONG)pMDUIStruct);
       
       MDUI_RegisterHotkeys(hWnd);
   
       if(MDUI_CreateSystemTray(hWnd))
       {
          MDUI_CreateMultipleDesktops(hWnd);
          MDUI_ParseCommandLine(hWnd); 
          MDUI_CheckForSafeModeRegKey(hWnd);
       }
   }
   
} 


/*****************************************************************************
 * MDUI_CreateSystemTray
 *
 * Puropose:
 *      Create The System Tray Menu & Icon
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
BOOL MDUI_CreateSystemTray(HWND hWnd)
{
    BOOL bRetValue = TRUE;
    NOTIFYICONDATA NoteData = {0};
    PMDUISTRUC pMDUIStruc;
 
     
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);

    NoteData.cbSize           = sizeof(NOTIFYICONDATA);
    NoteData.hWnd             = hWnd;
    NoteData.uID              = IDC_SYSTEM_TRAY;
    NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
    NoteData.uCallbackMessage = WM_MD_SYSTEM_TRAY;
    NoteData.hIcon            = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_DESKTOPONE_ICON));
    LoadString(GetModuleHandle(NULL), SYSTEM_TRAY_TOOLTIP, NoteData.szTip, sizeof(NoteData.szTip));

    Shell_NotifyIcon(NIM_ADD, &NoteData);
    
    pMDUIStruc->hPopupMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_SYSTRAY_MENU_DUMMY));
    CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), IDC_DT_ONE, MF_CHECKED);    
    pMDUIStruc->dwCurrentCheck = IDC_DT_ONE;
    
    return bRetValue;
}


/*****************************************************************************
 * MDUI_SetSystemTrayIcon
 *
 * Puropose:
 *      Set The System Tray ICON
 * 
 * Input:
 *      Window Handle, ID Of ICON
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_SetSystemTrayIcon(HWND hWnd, UINT IconID)
{
    NOTIFYICONDATA NoteData = {0};
    PMDUISTRUC pMDUIStruc;
 
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);

    NoteData.cbSize           = sizeof(NOTIFYICONDATA);
    NoteData.hWnd             = hWnd;
    NoteData.uID              = IDC_SYSTEM_TRAY;
    NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
    NoteData.uCallbackMessage = WM_MD_SYSTEM_TRAY;
    NoteData.hIcon            = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IconID));
    LoadString(GetModuleHandle(NULL), SYSTEM_TRAY_TOOLTIP, NoteData.szTip, sizeof(NoteData.szTip));

    Shell_NotifyIcon(NIM_MODIFY, &NoteData);
}



/*****************************************************************************
 * MDUI_DestroySystemTrayIcon
 *
 * Puropose:
 *      Clean up System Tray
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_DestroySystemTrayIcon(HWND hWnd)
{
    NOTIFYICONDATA NoteData = {0};
    PMDUISTRUC pMDUIStruc;
 
    pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);

    NoteData.cbSize           = sizeof(NOTIFYICONDATA);
    NoteData.hWnd             = hWnd;
    NoteData.uID              = IDC_SYSTEM_TRAY;
    NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
    NoteData.uCallbackMessage = WM_MD_SYSTEM_TRAY;
    NoteData.hIcon            = NULL;
    LoadString(GetModuleHandle(NULL), SYSTEM_TRAY_TOOLTIP, NoteData.szTip, sizeof(NoteData.szTip));

    Shell_NotifyIcon(NIM_DELETE, &NoteData);
       
}



/*****************************************************************************
 * MDUI_ParseCommandLine
 *
 * Puropose:
 *      Parse The Command Line
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_ParseCommandLine(HWND hWnd)
{
     char *pszCmdLne, szCmdLne[MAX_PATH];
     DWORD Index = 0, StartIndex;
     PMDUISTRUC pMDUIStruc;
     
     pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
      
     pszCmdLne = GetCommandLine();
     strncpy(szCmdLne, pszCmdLne, sizeof(szCmdLne));
     
     while(szCmdLne[Index])
     {
        StartIndex = Index;
        while(szCmdLne[Index] != ' ' && szCmdLne[Index])
           Index++;
        
        if(szCmdLne[Index] == ' ')
        {
           szCmdLne[Index] = 0;
           Index++;
        }
        
        if(!stricmp(szCmdLne + StartIndex, "-safemode"))
        {
           MDesk_SetFlags(pMDUIStruc->hMdesk, MDESK_SAFEMODE_FLAG);
           CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), IDC_SAFE_MODE, MF_CHECKED);  
        }
        
     }
     
}

/*****************************************************************************
 * MDUI_CheckForSafeModeRegKey(
 *
 * Puropose:
 *      Check if Safe Mode is enabled in the Registry
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_CheckForSafeModeRegKey(HWND hWnd)
{
     PMDUISTRUC pMDUIStruc;
     DWORD dwSafeMode;
          
     pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
     
     dwSafeMode = MDUI_GetDwordRegistryValue(MDUI_REG_KEY, MDUI_SAFEMODE_VALUE, DEFAULT_SAFEMODE);
     
     if(dwSafeMode)
     {
        MDesk_SetFlags(pMDUIStruc->hMdesk, MDESK_SAFEMODE_FLAG);
        CheckMenuItem(GetSubMenu(pMDUIStruc->hPopupMenu, 0), IDC_SAFE_MODE, MF_CHECKED);  
     }
}


/*****************************************************************************
 * MDUI_CreateMultipleDesktops
 *
 * Puropose:
 *      Create Multiple Desktops
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_CreateMultipleDesktops(HWND hWnd)
{
     PMDUISTRUC pMDUIStruc;
     pMDUIStruc = (PMDUISTRUC)GetWindowLong(hWnd, GWL_USERDATA);
     
     pMDUIStruc->hMdesk = MDesk_CreateDesktops(NUMBER_OF_DESKTOPS, 0);
}


/*****************************************************************************
 * MDUI_ErrorMsg
 *
 * Puropose:
 *      Display error message
 * 
 * Input:
 *      App Instance, Window Handle, Title Resource ID, Message Resource ID
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_ErrorMsg(HINSTANCE hInstance, HWND hWnd, UINT TitleID, UINT MsgID)
{
  char Caption[MAX_MSG_SIZE], Msg[MAX_MSG_SIZE];
  
  LoadString(hInstance, TitleID, Caption, sizeof(Caption));
  LoadString(hInstance, MsgID, Msg, sizeof(Msg));
  
  MessageBox(hWnd, Msg, Caption, MB_OK | MB_ICONERROR);
}

/*****************************************************************************
 * MDUI_MessageBox
 *
 * Puropose:
 *      Display a message to the user
 * 
 * Input:
 *      App Instance, Window Handle, Title Resource ID, Message Resource ID, Message box Flags
 * Output:
 *      Message Box Return
 *****************************************************************************/
UINT MDUI_MessageBox(HINSTANCE hInstance, HWND hWnd, UINT TitleID, UINT MsgID, UINT Flags)
{
  char Caption[MAX_MSG_SIZE], Msg[MAX_MSG_SIZE];
  
  LoadString(hInstance, TitleID, Caption, sizeof(Caption));
  LoadString(hInstance, MsgID, Msg, sizeof(Msg));
  
  return MessageBox(hWnd, Msg, Caption, Flags);
}


/*****************************************************************************
 * MDUI_RegisterHotkeys
 *
 * Puropose:
 *      Register Application Hotkeys
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_RegisterHotkeys(HWND hWnd)
{
   HINSTANCE hInstance = GetModuleHandle(NULL);
   DWORD Modifier;
   DWORD Key;

   Modifier = MDUI_GetDwordRegistryValue(MDUI_SD_REG_KEY, MDUI_UP_SD_MOD_VALUE, DEFAULT_MODIFIER);
   Key      = MDUI_GetDwordRegistryValue(MDUI_SD_REG_KEY, MDUI_UP_SD_KEY_VALUE, DEFAULT_KEY_SD_UP);
   
   if(!RegisterHotKey(hWnd, IDC_HOTKEY_DPSWITCH_FWD, Modifier, Key))
      MDUI_ErrorMsg(hInstance, hWnd, MDUI_HK_ERROR, HK_SDUP_ERROR);

   Modifier = MDUI_GetDwordRegistryValue(MDUI_SD_REG_KEY, MDUI_DWN_SD_MOD_VALUE, DEFAULT_MODIFIER);
   Key      = MDUI_GetDwordRegistryValue(MDUI_SD_REG_KEY, MDUI_DWN_SD_KEY_VALUE, DEFAULT_KEY_SD_DWN);
      
   if(!RegisterHotKey(hWnd, IDC_HOTKEY_DPSWITCH_BCK, Modifier, Key))
      MDUI_ErrorMsg(hInstance, hWnd, MDUI_HK_ERROR, HK_SDDOWN_ERROR);

   Modifier = MDUI_GetDwordRegistryValue(MDUI_UNI_REG_KEY, MDUI_ADD_UNI_MOD_VALUE, DEFAULT_MODIFIER);
   Key      = MDUI_GetDwordRegistryValue(MDUI_UNI_REG_KEY, MDUI_ADD_UNI_KEY_VALUE, DEFAULT_KEY_UNI_UP);
      
   if(!RegisterHotKey(hWnd, IDC_HOTKEY_UNIVERSAL_ON, Modifier, Key))
      MDUI_ErrorMsg(hInstance, hWnd, MDUI_HK_ERROR, HK_UNI_ADD_ERROR);

   Modifier = MDUI_GetDwordRegistryValue(MDUI_UNI_REG_KEY, MDUI_SUB_UNI_MOD_VALUE, DEFAULT_MODIFIER);
   Key      = MDUI_GetDwordRegistryValue(MDUI_UNI_REG_KEY, MDUI_SUB_UNI_KEY_VALUE, DEFAULT_KEY_UNI_DWN);
      
   if(!RegisterHotKey(hWnd, IDC_HOTKEY_UNIVERSAL_DOWN, Modifier, Key))
      MDUI_ErrorMsg(hInstance, hWnd, MDUI_HK_ERROR, HK_UNI_SUB_ERROR);

}


/*****************************************************************************
 * MDUI_GetDwordRegistryValue
 *
 * Puropose:
 *      Get Dword Value From Registry
 * 
 * Input:
 *      Registery Key, Registry Value, Default Value
 * Output:
 *      Registry Value
 *****************************************************************************/
DWORD MDUI_GetDwordRegistryValue(PCHAR RegistryKey, PCHAR RegistryValue, DWORD Default)
{
    DWORD RegValue = Default, Size = sizeof(RegValue);
    HKEY hKey;
    
    if(RegOpenKey(HKEY_CURRENT_USER, RegistryKey, &hKey) == ERROR_SUCCESS)
    {
        if(RegQueryValueEx(hKey, RegistryValue, 0, NULL, (BYTE *)&RegValue, &Size) != ERROR_SUCCESS)
           RegValue = Default;
           
        RegCloseKey(hKey); 
    }
    else if(RegOpenKey(HKEY_LOCAL_MACHINE, RegistryKey, &hKey) == ERROR_SUCCESS)
         {
             if(RegQueryValueEx(hKey, RegistryValue, 0, NULL, (BYTE *)&RegValue, &Size) != ERROR_SUCCESS)
                RegValue = Default;
           
             RegCloseKey(hKey); 
         }

    
    return RegValue;
}


/*****************************************************************************
 * MDUI_UnRegisterHotkeys
 *
 * Puropose:
 *      UnRegister Application Hotkeys
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_UnRegisterHotkeys(HWND hWnd)
{
   UnregisterHotKey(hWnd, IDC_HOTKEY_DPSWITCH_FWD);
   UnregisterHotKey(hWnd, IDC_HOTKEY_DPSWITCH_BCK);
   UnregisterHotKey(hWnd, IDC_HOTKEY_UNIVERSAL_ON);
   UnregisterHotKey(hWnd, IDC_HOTKEY_UNIVERSAL_DOWN);
}






/*****************************************************************************
 * MDUI_AboutBoxProc
 *
 * Puropose:
 *      About Box Dialog Proc
 * 
 * Input:
 *      Standard Window Callback Parameters
 * Output:
 *      Handled/Unhandled Message
 *****************************************************************************/
BOOL CALLBACK MDUI_AboutBoxProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL bRetValue = TRUE;
   
    switch(Msg)
    {
        case WM_INITDIALOG:
             MDUI_InitializeAboutDialog(hWnd);
             break;
             
        case WM_TIMER:
             MDUI_HandleAboutBoxTimer(hWnd);
             break;    
             
        case WM_COMMAND:
             MDUI_HandleAboutBoxCommands(hWnd, LOWORD(wParam));
             break;
             
        default:
             bRetValue = FALSE;
             break;
    }

    return bRetValue;
}



/*****************************************************************************
 * MDUI_InitializeAboutDialog
 *
 * Puropose:
 *      Init About Dialog
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_InitializeAboutDialog(HWND hWnd)
{
   SetTimer(hWnd, 0, 5000, NULL);
}

/*****************************************************************************
 * MDUI_HandleAboutBoxTimer
 *
 * Puropose:
 *      Handle Dialog Timer
 * 
 * Input:
 *      Window Handle
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleAboutBoxTimer(HWND hWnd)
{
   KillTimer(hWnd, 0);
   EndDialog(hWnd, 0);             
}

/*****************************************************************************
 * MDUI_HandleAboutBoxCommands
 *
 * Puropose:
 *      Handle Dialog Commands
 * 
 * Input:
 *      Window Handle, Command ID
 * Output:
 *      Nothing
 *****************************************************************************/
void MDUI_HandleAboutBoxCommands(HWND hWnd, UINT Cmd)
{
   switch(Cmd)
   {
        case IDOK:
        case IDCANCEL:
             MDUI_HandleAboutBoxTimer(hWnd);
             break;
   }
}
