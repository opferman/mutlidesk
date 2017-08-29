/*--------------------------------------------------*
   MultiDesk 2000

   Toby Opferman

   Decemeber 2000

 *--------------------------------------------------*/

#ifndef __MD2K_H__
#define __MD2K_H__

/* Constants */
#define IDI_MD2K_1     100
#define IDI_MD2K_2     101
#define IDI_MD2K_3     102
#define IDI_MD2K_4     103

#define WM_TRAYICON                 WM_USER
#define WM_DESKTOPSWITCHCOMPLETE    WM_USER + 1

#define IDC_TRAYICON   200
#define IDC_HOTKEY1    100
#define IDC_HOTKEY2    101
#define IDC_HOTKEY3    102
#define IDC_HOTKEY4    103


#define NUM_DESKTOPS   4

#define IDC_POP0        99
#define IDC_POP1       100
#define IDC_POP2       101
#define IDC_POP3       102
#define IDC_POP4       103
#define IDC_POP5       104
#define IDC_POP6       105
#define IDC_POP7       106


#define SETEXPLICIT          1
#define FORWARD              2
#define BACKWARD             4
#define GET_CURRENT_DESKTOP  8


/* Class Name Constants */
#define MDESK_CLASSNAME TEXT("MultiDesk")
#define SYSTRAY         TEXT("Shell_TrayWnd")
#define PROGMAN         TEXT("Progman")

#define HELPSTRING      TEXT("MDESK2K.HLP")

#define MENUSTRING0     TEXT("Safe Mode")
#define MENUSTRING1     TEXT("DeskTop One")
#define MENUSTRING2     TEXT("DeskTop Two")
#define MENUSTRING3     TEXT("DeskTop Three")
#define MENUSTRING4     TEXT("DeskTop Four")
#define MENUSTRING5     TEXT("About")
#define MENUSTRING6     TEXT("Help")
#define MENUSTRING7     TEXT("Exit")

/* Structures */
typedef struct __desktop_window_type {

  HWND ThisDeskTopWindow;
  struct __desktop_window_type *Next;  
   
} DESKTOP, *PDESKTOP;



typedef struct __complete_desktop_type {
  
  PDESKTOP AppLocked;
  HWND ProgMan, SysTray;
  DWORD CurrentDesktop, OldDesktop;
  BOOL FirstWindow;
  BOOL SafeMode;

  DESKTOP DeskTop[NUM_DESKTOPS], *Cur;

} COMPLETEDESKTOP, *PCOMPLETEDESKTOP;


typedef struct __switch_desktops_type {
  
  HWND hWnd;
  HINSTANCE hInstance;
  HMENU hMenu;
  PCOMPLETEDESKTOP pCompleteDesktop;

} SWITCHDESKTOPS, *PSWITCHDESKTOPS;


#endif





