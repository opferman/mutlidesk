/*--------------------------------------------------*
   MultiDesk 2000

   Toby Opferman

   Decemeber 2000

 *--------------------------------------------------*/


/* Header Files */
#include <windows.h>
#include <commctrl.h>
#include "md2k.h"
#include "worker.h"
#include "queue.h"



/* Prototypes */
 LRESULT CALLBACK SystemTrayProc(HWND, UINT, WPARAM, LPARAM);
 BOOL WINAPI RegClass(HINSTANCE);
 HWND WINAPI CreateWin(HINSTANCE);
 BOOL WINAPI SetTrayIcon(HINSTANCE, HWND, HMENU);
 HMENU WINAPI CreateSysTrayPopupMenu(HINSTANCE, HWND);
 BOOL CALLBACK AboutBoxProc(HWND, UINT, WPARAM,  LPARAM);
 DWORD WINAPI WorkItemComplete(PVOID, WORK_ID, DWORD);
 BOOL WINAPI EnumWindowsProc(HWND, LPARAM);
 DWORD WINAPI GetCurrentDesktop(DWORD);
 DWORD WINAPI SwitchDesktopProc(PVOID);
 DWORD WINAPI RestoreDesktops(COMPLETEDESKTOP);
 void WINAPI AddUniversalDesktop(PSWITCHDESKTOPS);
 void WINAPI RemoveUniversalDesktop(PSWITCHDESKTOPS);
 BOOL CheckCommandLineParametersForSafeMode(HWND hWnd, HMENU hPopupMenu);

/*-------------------------------------------------------*
     MAIN ENTRY POINT
 *-------------------------------------------------------*/
 int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HWND Hwnd;
    MSG Msg;
  
    /* If MultiDesk is Already Running, Exit */
    if(Hwnd = FindWindow(MDESK_CLASSNAME,NULL))
    {
        if(MessageBox(HWND_DESKTOP, TEXT("There is a version of MultiDesk already running."), TEXT("MultiDesk 2000 1.1"), MB_OK | MB_ICONEXCLAMATION) == IDNO)
           return 0;

        return 0;
    }
    
    /* Register Class */
    if(!RegClass(hInstance))
      return FALSE;

    InitCommonControls();

    /*  Create Window */
    if(!(Hwnd = CreateWin(hInstance)))
      return FALSE;

    /* Hide System Tray Window */
    ShowWindow(Hwnd, SW_HIDE);
    UpdateWindow(Hwnd);

    /* Message Loop */
    while(GetMessage(&Msg, 0, 0, 0))
    {
       TranslateMessage(&Msg);
       DispatchMessage(&Msg);
    }

    return Msg.wParam;
}


/*-------------------------------------------------------*
     SYSTEM TRAY WINDOW
 *-------------------------------------------------------*/
 LRESULT CALLBACK SystemTrayProc(HWND hWnd, UINT Msg, WPARAM wParam,  LPARAM lParam)
{
    static HINSTANCE hInstance;
    LRESULT ReturnValue;
    static WORKHANDLE WorkContext;
    static HMENU hPopupMenu;
    POINT CursorPosition;
    static WORKSTRUC WorkStruc;
    static SWITCHDESKTOPS SwitchDesktops;
    static COMPLETEDESKTOP CompleteDesktop;
    static int Freed = 1;

    switch(Msg)
    {

       case WM_CREATE:
           
                   hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

                   DialogBox(hInstance, TEXT("AboutBox"), hWnd, AboutBoxProc);

                   if(!RegisterHotKey(hWnd, IDC_HOTKEY1, MOD_ALT | MOD_CONTROL, VK_RIGHT))
                       MessageBox(HWND_DESKTOP, TEXT("Failed to register ALT + CONTROL + Right Arrow HotKey.  However, you can still use the system tray menu to switch desktops."), TEXT("HotKey Failure"), MB_OK | MB_ICONEXCLAMATION);

                   if(!RegisterHotKey(hWnd, IDC_HOTKEY2, MOD_ALT | MOD_CONTROL, VK_LEFT))
                       MessageBox(HWND_DESKTOP, TEXT("Failed to register ALT + CONTROL + Left Arrow HotKey.  However, you can still use the system tray menu to switch desktops."), TEXT("HotKey Failure"), MB_OK | MB_ICONEXCLAMATION);

                   if(!RegisterHotKey(hWnd, IDC_HOTKEY3, MOD_ALT | MOD_CONTROL, VK_UP))
                       MessageBox(HWND_DESKTOP, TEXT("Failed to register ALT + CONTROL + UP Arrow HotKey.  The Application Locking Feature will be disabled."), TEXT("HotKey Failure"), MB_OK | MB_ICONEXCLAMATION);
                   else if(!RegisterHotKey(hWnd, IDC_HOTKEY4, MOD_ALT | MOD_CONTROL, VK_DOWN))
                        {
                           UnregisterHotKey(hWnd, IDC_HOTKEY3);
                           MessageBox(HWND_DESKTOP, TEXT("Failed to register ALT + CONTROL + UP Arrow HotKey.  The Application Locking Feature will be disabled."), TEXT("HotKey Failure"), MB_OK | MB_ICONEXCLAMATION);
                        }

                   if(!(hPopupMenu = CreateSysTrayPopupMenu(hInstance, hWnd)))
                   {
                       MessageBox(HWND_DESKTOP, TEXT("Failed to create popup menu."), TEXT("Popup Menu Failure"), MB_OK | MB_ICONEXCLAMATION);
                       UnregisterHotKey(hWnd, IDC_HOTKEY1);
                       UnregisterHotKey(hWnd, IDC_HOTKEY2);
                       UnregisterHotKey(hWnd, IDC_HOTKEY3);
                       UnregisterHotKey(hWnd, IDC_HOTKEY4);

                       PostQuitMessage(0);
                       ReturnValue = 0;
                       break;
                   }
                   
                   SetTrayIcon(hInstance, hWnd, hPopupMenu);

                   if(!(WorkContext = CreateNewWorkerThreads(1, 0)))
                   {
                       MessageBox(HWND_DESKTOP, TEXT("Failed to create popup menu."), TEXT("Popup Menu Failure"), MB_OK | MB_ICONEXCLAMATION);
                       UnregisterHotKey(hWnd, IDC_HOTKEY1);
                       UnregisterHotKey(hWnd, IDC_HOTKEY2);
                       DestroyMenu(hPopupMenu);
                       PostQuitMessage(0);
                       ReturnValue = 0;
                       break;
                   }
               
                   CompleteDesktop.AppLocked      = 0;
                   CompleteDesktop.ProgMan        = FindWindow(PROGMAN, NULL);
                   CompleteDesktop.SysTray        = FindWindow(SYSTRAY, NULL);
                   CompleteDesktop.SafeMode       = CheckCommandLineParametersForSafeMode(hWnd, hPopupMenu);

                   ZeroMemory(CompleteDesktop.DeskTop, sizeof(CompleteDesktop.DeskTop));
                   
                   SwitchDesktops.hWnd             = hWnd;
                   SwitchDesktops.hMenu            = hPopupMenu;
                   SwitchDesktops.hInstance        = hInstance;
                   SwitchDesktops.pCompleteDesktop = &CompleteDesktop;

                   WorkStruc.WorkItem                = SwitchDesktopProc;
                   WorkStruc.WorkItemContext         = &SwitchDesktops;
                   WorkStruc.WorkItemFlags           = SEND_WORK_COMPLETE_FLAG;
                   WorkStruc.WorkItemComplete        = WorkItemComplete;
                   WorkStruc.WorkItemCompleteContext = &SwitchDesktops;
                   WorkStruc.WID                     = 0;
                   WorkStruc.NextWorkItem            = 0;

                   ReturnValue = 0;
                   UseQueue(QUEUE_INIT, 0);

                   break;

           case WM_HOTKEY:

                   switch(LOWORD(wParam))
                   {
                      case IDC_HOTKEY1:
                         UseQueue(QUEUE_PUSH, FORWARD);

                         if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                           MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);


                         ReturnValue = 0;
                         break;

                      case IDC_HOTKEY2:
                         UseQueue(QUEUE_PUSH, BACKWARD);

                         if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                           MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);

                         ReturnValue = 0;
                         break;

                      case IDC_HOTKEY3:
                         AddUniversalDesktop(&SwitchDesktops);
                         ReturnValue = 0;
                         break;

                      case IDC_HOTKEY4:
                         RemoveUniversalDesktop(&SwitchDesktops);
                         ReturnValue = 0;
                         break;

                   }

                   break;

           case WM_DESKTOPSWITCHCOMPLETE:
                   ReturnValue = 0;
                   break;

           /* System Tray Messages */
           case WM_TRAYICON:

          
           switch(LOWORD(lParam))
           {

               case WM_LBUTTONDOWN:
                          GetCursorPos(&CursorPosition);
                          SetForegroundWindow(hWnd);
                          TrackPopupMenu(hPopupMenu, TPM_LEFTBUTTON | TPM_CENTERALIGN, CursorPosition.x, CursorPosition.y, 0, hWnd, 0);
                          ReturnValue = 0;
                       break;

                       case WM_RBUTTONDOWN:
                          ReturnValue = 0;
               break;

                       default:
                           ReturnValue = DefWindowProc(hWnd, Msg, wParam, lParam);
               }


           break;
            case WM_COMMAND:

                   switch(LOWORD(wParam))
                   {
                       case IDC_POP0:
                         if(CompleteDesktop.SafeMode)
                         {
                            CompleteDesktop.SafeMode = FALSE;
                            CheckMenuItem(hPopupMenu, IDC_POP0, MF_UNCHECKED);
                         }
                         else
                         {
                            CompleteDesktop.SafeMode = TRUE;
                            CheckMenuItem(hPopupMenu, IDC_POP0, MF_CHECKED);
                         }
                         ReturnValue = 0;
                         break;

                       case IDC_POP1:
                         if(GetCurrentDesktop(GET_CURRENT_DESKTOP) != 0)
                         {
                            UseQueue(QUEUE_PUSH, 0x00000 | SETEXPLICIT);

                            if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                              MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);
                         }
                         ReturnValue = 0;
                         break;

                       case IDC_POP2:

                         if(GetCurrentDesktop(GET_CURRENT_DESKTOP) != 1)
                         {
   
                            UseQueue(QUEUE_PUSH, 0x10000 | SETEXPLICIT);

                            if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                              MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);
                         }

                         ReturnValue = 0;
                         break;

                       case IDC_POP3:
                         if(GetCurrentDesktop(GET_CURRENT_DESKTOP) != 2)
                         {

                            UseQueue(QUEUE_PUSH, 0x20000 | SETEXPLICIT);

                            if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                              MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);
                         }
                         ReturnValue = 0;
                         break;

                       case IDC_POP4:
                         if(GetCurrentDesktop(GET_CURRENT_DESKTOP) != 3)
                         {
   
                            UseQueue(QUEUE_PUSH, 0x30000 | SETEXPLICIT);

                            if(AddWorkItem(WorkContext, &WorkStruc) == INVALID_WORK_ID)
                              MessageBox(hWnd, TEXT("Unable to find resources to switch desktops."), TEXT("Desktop Switch Error"), MB_OK | MB_ICONEXCLAMATION);
                         }

                         ReturnValue = 0;
                         break;

                       case IDC_POP5:
                         DialogBox(hInstance, TEXT("AboutBox"), hWnd, AboutBoxProc);
                         ReturnValue = 0;
                         break;

                       case IDC_POP6:
                         WinHelp(hWnd, HELPSTRING, HELP_CONTENTS, 0);
                         ReturnValue = 0;
                         break;

                       case IDC_POP7:
                         if(MessageBox(hWnd, TEXT("Would you like to quit MultiDesk 2K Alpha 1.1?"), TEXT("Exit MultiDesk 2k Alpha 1.1"), MB_YESNO | MB_ICONQUESTION) == IDYES)
                         {
                             /* Poor Man's Semaphore */
                             if(!(--Freed))
                             {
                                NOTIFYICONDATA NoteData = {0};
 
                                CloseWorkerThreads(WorkContext);
                                RestoreDesktops(CompleteDesktop);
                                UnregisterHotKey(hWnd, IDC_HOTKEY1);
                                UnregisterHotKey(hWnd, IDC_HOTKEY2);
                                UnregisterHotKey(hWnd, IDC_HOTKEY3);
                                UnregisterHotKey(hWnd, IDC_HOTKEY4);
                                UseQueue(QUEUE_UNINIT, 0);
                                NoteData.cbSize           = sizeof(NOTIFYICONDATA);
                                NoteData.hWnd             = hWnd;
                                NoteData.uID              = IDC_TRAYICON;
                                NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
                                NoteData.uCallbackMessage = WM_TRAYICON;
                                strcpy(NoteData.szTip, TEXT("MultiDesk 2K Alpha 1.1"));
 
                                Shell_NotifyIcon(NIM_DELETE, &NoteData);
                                DestroyMenu(hPopupMenu);
                                PostQuitMessage(0);
                             }
                         }
                         ReturnValue = 0;
                         break;
                   }
                   break;
 
            case WM_DESTROY:
                 /* Poor Man's Semaphore */
                 if(!(--Freed))
                 {
                   NOTIFYICONDATA NoteData = {0};
                   CloseWorkerThreads(WorkContext);
                   RestoreDesktops(CompleteDesktop);
                   UnregisterHotKey(hWnd, IDC_HOTKEY1);
                   UnregisterHotKey(hWnd, IDC_HOTKEY2);
                   UnregisterHotKey(hWnd, IDC_HOTKEY3);
                   UnregisterHotKey(hWnd, IDC_HOTKEY4);
                   UseQueue(QUEUE_UNINIT, 0);
                   NoteData.cbSize           = sizeof(NOTIFYICONDATA);
                   NoteData.hWnd             = hWnd;
                   NoteData.uID              = IDC_TRAYICON;
                   NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
                   NoteData.uCallbackMessage = WM_TRAYICON;
                   strcpy(NoteData.szTip, TEXT("MultiDesk 2K Alpha 1.1"));

                   Shell_NotifyIcon(NIM_DELETE, &NoteData);

                   DestroyMenu(hPopupMenu);
                 }

           ReturnValue = 0;
           break;

            default:
           ReturnValue = DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return ReturnValue;
} 



/*-------------------------------------------------------*
     Check The Command Line Parameters
 *-------------------------------------------------------*/
BOOL CheckCommandLineParametersForSafeMode(HWND hWnd, HMENU hPopupMenu)
{
    char *szCmdLne, *Temp, Hold;
    BOOL RetValue = FALSE;   
 
    if(szCmdLne = GetCommandLine())
    {
       while(*szCmdLne)
       {
          if(*szCmdLne == '-' || *szCmdLne == '/')
          {
            szCmdLne++;
            Temp = szCmdLne;

            while(*Temp && *Temp != ' ')
                Temp++;
            Hold = *Temp;
            *Temp = 0;

            if(!_stricmp(szCmdLne, "safe"))
            {
                RetValue = TRUE;
                CheckMenuItem(hPopupMenu, IDC_POP0, MF_CHECKED);
            }
            
          }

          szCmdLne++;
       }
    }
        
    return RetValue;
}

/*-------------------------------------------------------*
     REGISTER CLASS
 *-------------------------------------------------------*/
 BOOL WINAPI RegClass(HINSTANCE hInstance)
{
    WNDCLASSEX wndclass;
     
    /* Define The Window */
    wndclass.style           = 0;
    wndclass.cbSize          = sizeof(wndclass);
    wndclass.lpfnWndProc     = SystemTrayProc;
    wndclass.cbClsExtra      = 0;
    wndclass.cbWndExtra      = 0;
    wndclass.hInstance       = hInstance;
    wndclass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor         = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName    = NULL;
    wndclass.lpszClassName   = MDESK_CLASSNAME;
    wndclass.hIconSm         = LoadIcon(NULL, IDI_APPLICATION);
 
    return RegisterClassEx(&wndclass);
}

/*-------------------------------------------------------*
     SET TRAY ICON
 *-------------------------------------------------------*/
 BOOL WINAPI SetTrayIcon(HINSTANCE hInstance, HWND hWnd, HMENU hMenu)
{
    NOTIFYICONDATA NoteData;
    DWORD TrayIcons[NUM_DESKTOPS] = { IDI_MD2K_1, IDI_MD2K_2, IDI_MD2K_3, IDI_MD2K_4 };
    DWORD PopMenu[NUM_DESKTOPS] = { IDC_POP1, IDC_POP2, IDC_POP3, IDC_POP4 };
    static BOOL FirstEntry = TRUE;
    DWORD DeskTop = 0, Incer;    

    if(FirstEntry) /* First Time, Init */
    {
        NoteData.cbSize           = sizeof(NOTIFYICONDATA);
        NoteData.hWnd             = hWnd;
        NoteData.uID              = IDC_TRAYICON;
        NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
        NoteData.uCallbackMessage = WM_TRAYICON;
        NoteData.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(TrayIcons[DeskTop]));
        strcpy(NoteData.szTip, TEXT("MultiDesk 2K Alpha 1.1"));

        Shell_NotifyIcon(NIM_ADD, &NoteData);
        
        CheckMenuItem(hMenu, PopMenu[DeskTop], MF_CHECKED);
        FirstEntry = FALSE;

        return TRUE;
    }

  
    DeskTop = GetCurrentDesktop(GET_CURRENT_DESKTOP);

    for(Incer = 0; Incer < NUM_DESKTOPS; Incer++)
      if(Incer == DeskTop)
        CheckMenuItem(hMenu, PopMenu[DeskTop], MF_CHECKED);
      else
        CheckMenuItem(hMenu, PopMenu[Incer], MF_UNCHECKED);



    NoteData.cbSize           = sizeof(NOTIFYICONDATA);
    NoteData.hWnd             = hWnd;
    NoteData.uID              = IDC_TRAYICON;
    NoteData.uFlags           = NIF_MESSAGE + NIF_ICON + NIF_TIP;
    NoteData.uCallbackMessage = WM_TRAYICON;
    NoteData.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(TrayIcons[DeskTop]));
    strcpy(NoteData.szTip, TEXT("MultiDesk 2K Alpha 1.1"));

    Shell_NotifyIcon(NIM_MODIFY, &NoteData);

    return TRUE;    
}


/*-------------------------------------------------------*
     CREATE WINDOW
 *-------------------------------------------------------*/
 HWND WINAPI CreateWin(HINSTANCE hInstance)
{
    return CreateWindowEx(0, MDESK_CLASSNAME, TEXT("MultiDesk 2K Alpha 1.1"),WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        HWND_DESKTOP, NULL, hInstance, NULL);
}



/*-------------------------------------------------------*
     ABOUT BOX DIALOG
 *-------------------------------------------------------*/
 BOOL CALLBACK AboutBoxProc(HWND hDlg, UINT Msg, WPARAM wParam,  LPARAM lParam)
{
    BOOL ReturnValue = FALSE;

    switch(Msg)
    {

          case WM_INITDIALOG:
                   ReturnValue = TRUE;
                   SetTimer(hDlg, 0, 5000, NULL);
 
                   break;

          case WM_TIMER:
                   KillTimer(hDlg, 0);
                   EndDialog(hDlg, 0); 
                   ReturnValue = TRUE;
                   break;


          case WM_COMMAND:
          
           switch(LOWORD(wParam))
           {
                       case IDCANCEL:
               case IDOK:
                           KillTimer(hDlg, 0);
                           EndDialog(hDlg, 0);
                           ReturnValue = TRUE;
                       break;
           }
           break;
    }

    return ReturnValue;
} 


/*-------------------------------------------------------*
     CREATE SYSTEM TRAY POPUP MENU
 *-------------------------------------------------------*/
 HMENU WINAPI CreateSysTrayPopupMenu(HINSTANCE hInstance, HWND hWnd)
{
  HMENU hMenu;


  if(!(hMenu = CreatePopupMenu()))
    return NULL;
 
  AppendMenu(hMenu, MF_STRING, IDC_POP0, MENUSTRING0);  
  AppendMenu(hMenu, MF_STRING, IDC_POP1, MENUSTRING1);
  AppendMenu(hMenu, MF_STRING, IDC_POP2, MENUSTRING2);
  AppendMenu(hMenu, MF_STRING, IDC_POP3, MENUSTRING3);
  AppendMenu(hMenu, MF_STRING, IDC_POP4, MENUSTRING4);
  AppendMenu(hMenu, MF_STRING, IDC_POP5, MENUSTRING5);
  AppendMenu(hMenu, MF_STRING, IDC_POP6, MENUSTRING6);
  AppendMenu(hMenu, MF_STRING, IDC_POP7, MENUSTRING7);

  return hMenu;
}  

/*-------------------------------------------------------*
     WORK COMPLETE CALLBACK
 *-------------------------------------------------------*/
 DWORD WINAPI WorkItemComplete(PVOID Context, WORK_ID WID, DWORD RetVal)
{
   PSWITCHDESKTOPS pSwitchDesktops = (PSWITCHDESKTOPS)Context;

   SendMessage(pSwitchDesktops->hWnd, WM_DESKTOPSWITCHCOMPLETE, WID, RetVal);
           
   return 0;
}


/*-------------------------------------------------------*
     SWITCH DESKTOPS
 *-------------------------------------------------------*/
 DWORD WINAPI SwitchDesktopProc(PVOID Context)
{
   PSWITCHDESKTOPS pSwitchDesktops = (PSWITCHDESKTOPS)Context;
   PDESKTOP pDeskTop, pTemp;
   DWORD Result;
   BOOL FirstWindow = TRUE;

   pSwitchDesktops->pCompleteDesktop->ProgMan = FindWindow(PROGMAN, NULL);
   pSwitchDesktops->pCompleteDesktop->SysTray = FindWindow(SYSTRAY, NULL);

   pSwitchDesktops->pCompleteDesktop->OldDesktop     = GetCurrentDesktop(GET_CURRENT_DESKTOP);
   pSwitchDesktops->pCompleteDesktop->CurrentDesktop = GetCurrentDesktop(UseQueue(QUEUE_POP, 0));
   pSwitchDesktops->pCompleteDesktop->Cur            = &pSwitchDesktops->pCompleteDesktop->DeskTop[pSwitchDesktops->pCompleteDesktop->OldDesktop];
   pSwitchDesktops->pCompleteDesktop->FirstWindow    = TRUE;
   pSwitchDesktops->pCompleteDesktop->DeskTop[pSwitchDesktops->pCompleteDesktop->OldDesktop].ThisDeskTopWindow = 0;

   LockWindowUpdate(GetDesktopWindow());

   EnumWindows(EnumWindowsProc, (LPARAM)pSwitchDesktops);
   SetTrayIcon(pSwitchDesktops->hInstance, pSwitchDesktops->hWnd, pSwitchDesktops->hMenu);
   
   if(pSwitchDesktops->pCompleteDesktop->DeskTop[pSwitchDesktops->pCompleteDesktop->CurrentDesktop].ThisDeskTopWindow)
   {
       pDeskTop = &pSwitchDesktops->pCompleteDesktop->DeskTop[pSwitchDesktops->pCompleteDesktop->CurrentDesktop];

       if(IsWindow(pDeskTop->ThisDeskTopWindow))
       {
          if(pSwitchDesktops->pCompleteDesktop->SafeMode)
             ShowWindowAsync(pDeskTop->ThisDeskTopWindow, SW_SHOW);
          else
             ShowWindow(pDeskTop->ThisDeskTopWindow, SW_SHOW);

          if(FirstWindow)
          {  
             SetForegroundWindow(pDeskTop->ThisDeskTopWindow);
             SetFocus(pDeskTop->ThisDeskTopWindow);
             FirstWindow = FALSE;
          }
       }

        

       pDeskTop->ThisDeskTopWindow = 0;
       pDeskTop = pDeskTop->Next;

       pSwitchDesktops->pCompleteDesktop->DeskTop[pSwitchDesktops->pCompleteDesktop->CurrentDesktop].Next = 0;       

       while(pDeskTop)
       {
          
          if(pSwitchDesktops->pCompleteDesktop->SafeMode)
             ShowWindowAsync(pDeskTop->ThisDeskTopWindow, SW_SHOW);
          else
             ShowWindow(pDeskTop->ThisDeskTopWindow, SW_SHOW);

          pTemp    = pDeskTop;
          pDeskTop = pDeskTop->Next;
          LocalFree(pTemp);
       }
   }

   LockWindowUpdate(NULL);

   return 0;
}

/*-------------------------------------------------------*
     GET DESKTOP NUMBER
 *-------------------------------------------------------*/
 DWORD WINAPI GetCurrentDesktop(DWORD Flag)
{
    static DWORD DeskTop = 0;

    if(Flag & GET_CURRENT_DESKTOP)
       return DeskTop;

    if(Flag & FORWARD)
    {


       DeskTop++;

       if(DeskTop >= NUM_DESKTOPS)
          DeskTop = 0;
    }

    if(Flag & BACKWARD)
    {

       if(DeskTop)
           DeskTop--;
       else
          DeskTop = NUM_DESKTOPS - 1;
    }

    if(Flag & SETEXPLICIT)
    {
       Flag>>=16;

       if(Flag >= NUM_DESKTOPS)
       {
          SetLastError(ERROR_RESOURCE_NOT_FOUND);
          return FALSE;
       }


       DeskTop = Flag;
    }

    return DeskTop;
}



/*-------------------------------------------------------*
     ENUMERATION OF WINDOWS
 *-------------------------------------------------------*/
 BOOL WINAPI EnumWindowsProc(HWND hDlg, LPARAM lParam)
{
   PSWITCHDESKTOPS pSwitchDesktops = (PSWITCHDESKTOPS)lParam;
   PDESKTOP pDeskTop;
   DWORD Result;
   
   /* Check If Window Is Visible */
   if(!IsWindowVisible(hDlg))
     return TRUE;

   /* Exclude ProgMan */
   if(hDlg == pSwitchDesktops->pCompleteDesktop->ProgMan)
     return TRUE;

   /* Exclude SysTray */
   if(hDlg == pSwitchDesktops->pCompleteDesktop->SysTray)
     return TRUE;

   pDeskTop = pSwitchDesktops->pCompleteDesktop->AppLocked;

   while(pDeskTop)
   {
     if(hDlg == pDeskTop->ThisDeskTopWindow)
       return TRUE;

     pDeskTop = pDeskTop->Next;
   }

   if(pSwitchDesktops->pCompleteDesktop->FirstWindow)  
   {
      pSwitchDesktops->pCompleteDesktop->Cur->ThisDeskTopWindow = hDlg;
      pSwitchDesktops->pCompleteDesktop->FirstWindow = FALSE;

      if(pSwitchDesktops->pCompleteDesktop->SafeMode)
          ShowWindowAsync(hDlg, SW_HIDE);
      else
          ShowWindow(hDlg, SW_HIDE);


      return TRUE;
   }

   if(!(pSwitchDesktops->pCompleteDesktop->Cur->Next = LocalAlloc(LMEM_ZEROINIT, sizeof(DESKTOP))))
      return FALSE;

   pSwitchDesktops->pCompleteDesktop->Cur = pSwitchDesktops->pCompleteDesktop->Cur->Next;
   pSwitchDesktops->pCompleteDesktop->Cur->ThisDeskTopWindow = hDlg;

   if(pSwitchDesktops->pCompleteDesktop->SafeMode)
       ShowWindowAsync(hDlg, SW_HIDE);
   else
       ShowWindow(hDlg, SW_HIDE);

   return TRUE;
}


/*-------------------------------------------------------*
     RESTORE ALL DESKTOPS
 *-------------------------------------------------------*/
 DWORD WINAPI RestoreDesktops(COMPLETEDESKTOP CompleteDesktop)
{
   PDESKTOP pDeskTop, pTemp;
   DWORD CurrentDesktop = 0, Result;
    
   for(CurrentDesktop = 0; CurrentDesktop < NUM_DESKTOPS; CurrentDesktop++)
      if(CompleteDesktop.DeskTop[CurrentDesktop].ThisDeskTopWindow)
      {
          pDeskTop = &CompleteDesktop.DeskTop[CurrentDesktop];

          if(CompleteDesktop.SafeMode)
             ShowWindowAsync(pDeskTop->ThisDeskTopWindow, SW_SHOW);
          else
             ShowWindow(pDeskTop->ThisDeskTopWindow, SW_SHOW);

          pDeskTop->ThisDeskTopWindow = 0;
          pDeskTop = pDeskTop->Next;

          CompleteDesktop.DeskTop[CurrentDesktop].Next = 0;

          while(pDeskTop)
          {
             if(CompleteDesktop.SafeMode)
               ShowWindowAsync(pDeskTop->ThisDeskTopWindow, SW_SHOW);
             else
               ShowWindow(pDeskTop->ThisDeskTopWindow, SW_SHOW);

             pTemp    = pDeskTop;
             pDeskTop = pDeskTop->Next;
             LocalFree(pTemp);
          }
          
       }

       pDeskTop = CompleteDesktop.AppLocked;

       while(pDeskTop)
       {
             pTemp    = pDeskTop;
             pDeskTop = pDeskTop->Next;
             LocalFree(pTemp);
       }
 
       return 0;
}


/*-------------------------------------------------------*
     ADD UNIVERSAL APPLICATION
 *-------------------------------------------------------*/
 void WINAPI AddUniversalDesktop(PSWITCHDESKTOPS pSwitchDesktop)
{
   PDESKTOP pDeskTop;
   HWND hDlg;

   hDlg = GetForegroundWindow();

   if(!hDlg)
     return;

   if(!pSwitchDesktop->pCompleteDesktop->AppLocked)
   {
      if(!(pSwitchDesktop->pCompleteDesktop->AppLocked = LocalAlloc(LMEM_ZEROINIT, sizeof(DESKTOP))))
         return;
      
      pSwitchDesktop->pCompleteDesktop->AppLocked->ThisDeskTopWindow = hDlg;
   }
   else
   {
      pDeskTop = pSwitchDesktop->pCompleteDesktop->AppLocked;

      while(pDeskTop->Next)
      {
          if(!IsWindow(pDeskTop->ThisDeskTopWindow))
          {
             pDeskTop->ThisDeskTopWindow = hDlg;
             return;
          }
          pDeskTop = pDeskTop->Next;
      }

      if(!IsWindow(pDeskTop->ThisDeskTopWindow))
      {
         pDeskTop->ThisDeskTopWindow = hDlg;
         return;
      }

      if(!(pDeskTop->Next = LocalAlloc(LMEM_ZEROINIT, sizeof(DESKTOP))))
         return;
      
      pDeskTop->Next->ThisDeskTopWindow = hDlg;
   }
}

/*-------------------------------------------------------*
     REMOVE UNIVERSAL APPLICATION
 *-------------------------------------------------------*/
 void WINAPI RemoveUniversalDesktop(PSWITCHDESKTOPS pSwitchDesktop)
{
   PDESKTOP pDeskTop, pTemp;
   HWND hDlg;


   hDlg = GetForegroundWindow();

   if(!hDlg)
     return;

   if(pSwitchDesktop->pCompleteDesktop->AppLocked)
   {
      pDeskTop = pSwitchDesktop->pCompleteDesktop->AppLocked;

      while(!IsWindow(pDeskTop->ThisDeskTopWindow))
      { 
          pTemp = pDeskTop;
          pDeskTop = pDeskTop->Next;
          LocalFree(pTemp);
          pSwitchDesktop->pCompleteDesktop->AppLocked = pDeskTop; 

          if(!pDeskTop)
             return;
      }

      if(pDeskTop->ThisDeskTopWindow == hDlg)
      {
          pTemp = pDeskTop;
          pDeskTop = pDeskTop->Next;
          LocalFree(pTemp);

          pSwitchDesktop->pCompleteDesktop->AppLocked = pDeskTop;

          if(!pDeskTop)
               return;
      }

      pTemp = pDeskTop;
      pDeskTop = pDeskTop->Next;
          

      while(pDeskTop)
      {
          if(pDeskTop->ThisDeskTopWindow == hDlg || !IsWindow(pDeskTop->ThisDeskTopWindow))
          {
            pTemp->Next = pDeskTop->Next;
            LocalFree(pDeskTop);
            pDeskTop = pTemp->Next;
          }
          else 
          {
             pTemp = pDeskTop;
             pDeskTop = pDeskTop->Next;
          }

      }
   }

}

  