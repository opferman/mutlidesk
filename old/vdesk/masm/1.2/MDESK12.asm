;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Toby Opferman                                               ;
;                   M U L T I  -  D E S K                     ;
;                                                             ;
; Virtual Desktop                                             ;
;                                                             ;
; http://www.opferman.com                                     ;
;                                                             ;
; _Secret/Scrat                                               ;
; toby@opferman.com                                           ;
; Copyright 2000                                              ;
;                                                             ;
; MultiDesk 1.2                                               ;
; MASM Source                                                 ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;

; Set Up Processor & Model
.486p
.MODEL FLAT, STDCALL
option casemap :none  ; case sensitive

; Include Windows Structures
include c:\lang\MASM\INCLUDE\windows.inc
include c:\lang\MASM\INCLUDE\masm32.inc
include c:\lang\MASM\INCLUDE\gdi32.inc
include c:\lang\MASM\INCLUDE\user32.inc
include c:\lang\MASM\INCLUDE\shell32.inc
include c:\lang\MASM\INCLUDE\kernel32.inc

includelib c:\lang\MASM\LIB\gdi32.lib
includelib c:\lang\MASM\LIB\user32.lib
includelib c:\lang\MASM\LIB\kernel32.lib
includelib c:\lang\MASM\LIB\shell32.lib

PUBLIC WndProc

L EQU <DWORD>

.data
;-=-=-=-=-=-=-=-=-=-=-=-;
; Data Structures       ;
;-=-=-=-=-=-=-=-=-=-=-=-;
 Desk    STRUC
     ThisHwnd     DWORD ?
     BeforeWindow DWORD ?
     AfterWindow  DWORD ?
 Desk    ENDS

 DeskTop STRUC
     CurWindow  DWORD   ?               ; Current Window
     DeskTops   DWORD   4 DUP (?)       ; Desk Tops
 DeskTop ENDS

;-=-=-=-=-=-=-=-=-=-=-=-;
; Data Area             ;
;-=-=-=-=-=-=-=-=-=-=-=-;
 MAX_DESKTOPS  DWORD  4
 IDC_TRAYICON  EQU <1000>
 IDC_HOTKEY1   EQU <1001>
 IDC_HOTKEY2   EQU <1002>

 IDC_POP1      EQU <0>
 IDC_POP2      EQU <1>
 IDC_POP3      EQU <2>
 IDC_POP4      EQU <3>
 IDC_POP5      EQU <4>
 IDC_POP6      EQU <5>
 IDC_POP7      EQU <6>

 CursorPosition POINT <?>
 TheClassName  db         'MultiDesk', 0
 TrayName      db         'MultiDesk v1.2 Alpha', 0
 SystemTray    db         'Shell_TrayWnd', 0
 ProgMan       db         'Progman', 0
 HelpString    db         'MULTIDESK.HLP',0
 MenuString1   db         'DeskTop One', 0
 MenuString2   db         'DeskTop Two', 0
 MenuString3   db         'DeskTop Three', 0
 MenuString4   db         'DeskTop Four', 0
 MenuString5   db         'About', 0
 MenuString6   db         'Help', 0
 MenuString7   db         'Exit', 0

 ExitMsgCap    db         'EXIT MULTIDESK 1.2 Alpha', 0
 ExitMsg       db         'Exit Multidesk 1.2?',0

 DeskWindow    DWORD      ?
 ThisClassName db         26 DUP(?)
 MultiDesk     DeskTop    <?>
 WinhInstance  DWORD       ?
 WC            WNDCLASSEX <?>
 HwND          DWORD       ?
 DeskIcon1     DWORD      101
 DeskIcon2     DWORD      102
 DeskIcon3     DWORD      103
 DeskIcon4     DWORD      104
 AboutText     db         'AboutBox', 0
 Msg           MSG        <?>
 TempHwnd      DWORD       ?
 NoteData      NOTIFYICONDATAA  <?>
 PopupMenuHandle DWORD ?
 
;-=-=-=-=-=-=-=-=-=-=-=-=-;
; Main Windows Procedure  ;
;-=-=-=-=-=-=-=-=-=-=-=-=-;
.CODE

WinMain:
 PUSH  0
 CALL GetModuleHandle
 MOV [WinhInstance], EAX                ; Get HINSTANCE

 PUSH  0
 PUSH  OFFSET TheClassName
 CALL FindWindow                        ; Is Window Running?
 
 TEST EAX, EAX
 JZ SHORT NOT_RUNNING                   ; Is program already running?
 
 PUSH  0
 CALL ExitProcess                       ; Exit If Program is already running
 
NOT_RUNNING: 
 CALL SetDeskTop                        ; Setup DeskTop 

 CALL InitWindow                        ; Intialize The Window
 TEST EAX, EAX
 JZ QUIT

 CALL DisplayWindow                     ; Display The Window
 TEST EAX, EAX
 JZ QUIT
 
 PUSH  SW_HIDE
 PUSH  EAX
 CALL ShowWindow                        ; Show Window
 
 PUSH  [HwND]
 CALL UpdateWindow                      ; Update Window
 
GetMessageLoop:
   PUSH  0
   PUSH  0
   PUSH  0
   PUSH OFFSET Msg
   CALL GetMessage
 
   TEST EAX, EAX                        ; When GetMessage Returns a 0, Window Destoryed
   JZ SHORT QUIT
   
   PUSH OFFSET Msg
   CALL TranslateMessage        
   
   PUSH OFFSET Msg
   CALL DispatchMessage
 
 JMP SHORT GetMessageLoop
 
QUIT:

 PUSH  0
 CALL ExitProcess                       ; Exit
; END WinMain


;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Set Up DeskTop                ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 SetDeskTop PROC
  PUSHA
   
   MOV ECX, [MAX_DESKTOPS]
   XOR EAX, EAX
   
ZERO_DESKTOPS:
      PUSH EAX
       
       SHL EAX, 2
       MOV [MultiDesk.DeskTops + EAX], 0                ; Loop and set Default Desktops to 0
       
      POP EAX
      INC EAX
     LOOP SHORT ZERO_DESKTOPS
 
  POPA
  RET
SetDeskTop ENDP

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Restore Desktop               ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 RestoreDesktop PROC
  PUSHA
  
  MOV ECX, [MAX_DESKTOPS]
  XOR EAX, EAX
  
STARTHERE:
  PUSH EAX
  PUSH ECX
    
  SHL EAX, 2

  MOV EAX, [MultiDesk.DeskTops + EAX]  ; Get All Desktops
  
 
 INNER_LOOP:
   TEST EAX, EAX                        ; Test For Zero
   JZ SHORT ALLDONE

     PUSH EAX
     PUSH SW_SHOW                     ; Show Window
     PUSH DWORD PTR [EAX]
     CALL ShowWindow
     POP EAX           
     
     MOV EBX, [EAX + 8]                 ; Next Window
     
     PUSH EAX
     CALL LocalFree                     ; Free Old Window
     
     MOV EAX, EBX
   JMP SHORT INNER_LOOP
   
 ALLDONE:
   POP ECX
   POP EAX
    
   INC EAX
  LOOP SHORT STARTHERE
  
  POPA
  RET
RestoreDesktop ENDP 
 


;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Register Window               ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
  InitWindow PROC
    MOV [WC.hInstance], EAX
    MOV [WC.style], 0
    MOV [WC.lpfnWndProc], OFFSET WndProc
    MOV [WC.cbClsExtra], 0
    MOV [WC.cbWndExtra], 0
    MOV [WC.cbSize], SIZE WNDCLASSEX

    PUSH [DeskIcon1]
    PUSH EAX
    CALL LoadIcon

    MOV [WC.hIcon], EAX
    PUSH  IDC_ARROW
    MOV [WC.hIconSm], EAX
    PUSH  0
    CALL LoadCursor

    MOV [WC.hCursor], EAX

    
    MOV [WC.hbrBackground], 0
    MOV [WC.lpszMenuName],  0
    MOV [WC.lpszClassName], OFFSET TheClassName

    PUSH OFFSET WC
    CALL RegisterClassEx                     

  RET
InitWindow  ENDP 

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Create Window                 ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
  DisplayWindow  PROC
    PUSH  0
    PUSH [WinhInstance]
    PUSH  0
    PUSH  0
    PUSH  CW_USEDEFAULT
    PUSH  CW_USEDEFAULT
    PUSH  CW_USEDEFAULT
    PUSH  CW_USEDEFAULT
    PUSH  0
    PUSH  0
    PUSH OFFSET TheClassName
    PUSH  0
    CALL CreateWindowEx
    MOV [HwND], EAX
  RET
DisplayWindow  ENDP 

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Windows CallBack Procedure    ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
WndProc PROC USES EBX EDI ESI, hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
  
  CMP wmsg, WM_USER             ; Check For WM_USER Message
  JE WUSER

  CMP wmsg, WM_HOTKEY           ; Check For WM_HOTKEY Message
  JE WHOTKEY
  
  CMP wmsg, WM_CREATE           ; Check For WM_CREATE Message
  JE SHORT WCREATE
  
  CMP wmsg, WM_COMMAND          ; Check For WM_COMMAND Message
  JE WCOMMAND
  
  CMP wmsg, WM_DESTROY          ; Kill Program
  JE WDESTROY
  
THEDEFAULT:
  PUSH [lparam]
  PUSH  [wparam]
  PUSH  [wmsg]
  PUSH  [hwnd]
  CALL DefWindowProc  
  RET
  
WCREATE:                        ; WM_CREATE Message
  
  PUSH  0
  PUSH OFFSET AboutProc 
  PUSH [hwnd]
  PUSH OFFSET AboutText
  PUSH [WinhInstance]
  CALL DialogBoxParam           ; Call About Box 
                        
                                ; Setup SystemTray
                                
  MOV EAX, [hwnd]
  MOV [NoteData.cbSize], SIZE NOTIFYICONDATAA
  
  MOV [NoteData.hwnd], EAX
  MOV [NoteData.uID], IDC_TRAYICON
  MOV [NoteData.uFlags], NIF_MESSAGE + NIF_ICON + NIF_TIP
  MOV [NoteData.uCallbackMessage], WM_USER
  
  PUSH [DeskIcon1]
  PUSH  [WinhInstance]
  CALL LoadIcon
  
  MOV [NoteData.hIcon], EAX  
  
  
  MOV EDI, OFFSET NoteData.szTip
  MOV ECX, 21
  MOV ESI, OFFSET TrayName
  REP MOVSB
  
  PUSH  OFFSET NoteData
  PUSH  NIM_ADD
  CALL Shell_NotifyIcon                        ; Set up Systray ICON
  
  PUSH  VK_RIGHT
  PUSH  MOD_ALT + MOD_CONTROL
  PUSH  IDC_HOTKEY1
  PUSH  [hwnd]
  CALL RegisterHotKey                          ; Register HOTKEYS
  
  PUSH  VK_LEFT
  PUSH  MOD_ALT + MOD_CONTROL
  PUSH IDC_HOTKEY2
  PUSH  [hwnd]
  CALL RegisterHotKey                          ; Register HOTKEYS  
  
  CALL CreatePopupMenu


  TEST EAX, EAX 
  JZ  EXIT_CREATE                         ; Test Window handle
  
  MOV [PopupMenuHandle], EAX
  
  PUSH  OFFSET MenuString1                    ; Create Menu Popup
  PUSH  IDC_POP1
  PUSH  MF_STRING
  PUSH EAX
  CALL AppendMenu
  
  PUSH MF_CHECKED
  PUSH  IDC_POP1
  PUSH  [PopupMenuHandle]
  CALL CheckMenuItem                           ; Check Menu Item
  
  PUSH  OFFSET MenuString2                    ; Create Menu Popup
  PUSH  IDC_POP2
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu

  PUSH  OFFSET MenuString3                    ; Create Menu Popup
  PUSH  IDC_POP3
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu

  PUSH  OFFSET MenuString4                    ; Create Menu Popup
  PUSH  IDC_POP4
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu
   
  PUSH  OFFSET MenuString5                    ; Create Menu Popup
  PUSH  IDC_POP5
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu

  PUSH  OFFSET MenuString6                    ; Create Menu Popup
  PUSH  IDC_POP6
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu

  PUSH  OFFSET MenuString7                    ; Create Menu Popup
  PUSH  IDC_POP7
  PUSH  MF_STRING
  PUSH  [PopupMenuHandle]
  CALL AppendMenu


    
EXIT_CREATE:
  XOR EAX, EAX
  RET

WCOMMAND:

  
 MOV EAX, [wparam]
 AND EAX, 0FFFFh
 
 CMP EAX, IDC_POP1              ; Dialog Popup Menu
 JE  P1
 
 CMP EAX, IDC_POP2
 JE  P2
 
 CMP EAX, IDC_POP3
 JE  P3
 
 CMP EAX, IDC_POP4
 JE P4
 
 CMP EAX, IDC_POP5
 JE P5

 CMP EAX, IDC_POP6
 JE P6

 CMP EAX, IDC_POP7
 JE P7


 JMP THEDEFAULT

DO_SWITCH:

  PUSH EAX
  PUSH  MF_CHECKED
  PUSH EAX
  PUSH  [PopupMenuHandle]
  CALL CheckMenuItem                           ; Check Menu Item

  PUSH  MF_UNCHECKED
  PUSH EBX
  PUSH  [PopupMenuHandle]
  CALL CheckMenuItem                           ; UnCheck Menu Item

  POP EAX
  PUSH EAX

  INC EAX
  ADD EAX, 100

  PUSH EAX
  PUSH  [WinhInstance]
  CALL LoadIcon
  
  MOV [NoteData.hIcon], EAX  
  
  PUSH  OFFSET NoteData
  PUSH  NIM_MODIFY 
  CALL  Shell_NotifyIcon                        ; Set up Systray ICON

  PUSH [hwnd]                                  ; Set Focus
  CALL SetFocus


  POP EBX

 CALL SwitchDeskTop
 
 XOR EAX, EAX
 RET
 
P1:
 
 MOV EBX, [MultiDesk.CurWindow]
 XOR EAX, EAX
 
 TEST EBX, EBX
 JNZ SHORT DO_SWITCH

 XOR EAX, EAX
 RET

P2:
 MOV EBX, [MultiDesk.CurWindow]
 MOV EAX, 1
 
 CMP EBX, EAX
 JNE SHORT DO_SWITCH

 XOR EAX, EAX
 RET

P3:
 MOV EBX, [MultiDesk.CurWindow]
 MOV EAX, 2
 
 CMP EBX, EAX
 JNE DO_SWITCH

 XOR EAX, EAX
 RET

P4:
 MOV EBX, [MultiDesk.CurWindow]
 MOV EAX, 3
 
 CMP EBX, EAX
 JNE DO_SWITCH

 XOR EAX, EAX
 RET

P5:
  PUSH  0
  PUSH OFFSET AboutProc 
  PUSH [hwnd]
  PUSH OFFSET AboutText
  PUSH [WinhInstance]
  CALL DialogBoxParam           ; Call About Box 

 XOR EAX, EAX
 RET

P6:
 
 PUSH  0
 PUSH  HELP_CONTENTS   
 PUSH  OFFSET HelpString
 PUSH  [hwnd]
 CALL WinHelp
 
 XOR EAX, EAX
 RET

P7:
 PUSH MB_YESNO + MB_ICONQUESTION
 PUSH OFFSET ExitMsgCap
 PUSH OFFSET ExitMsg
 PUSH [hwnd]
 CALL MessageBox

 CMP EAX, IDYES
 JNE SHORT NO_QUIT

 CALL RestoreDesktop
 
 PUSH  0
 CALL PostQuitMessage

NO_QUIT:
 XOR EAX, EAX
 RET


WHOTKEY:                                ; Hot Key
 
 CMP [wparam], IDC_HOTKEY1              ; Check Hot Key 1
 JE SHORT HOT1
 
 CMP [wparam], IDC_HOTKEY2              ; Check Hot Key 2
 JE SHORT HOT2


 XOR EAX, EAX
 RET
 
HOT1:
 MOV EAX, [MultiDesk.CurWindow]
 MOV EBX, EAX
 
 INC EAX
 CMP EAX, [MAX_DESKTOPS]                  ; Switch To Next Window
 JB DO_SWITCH
 
 XOR EAX, EAX
 JMP DO_SWITCH
  
 
HOT2:
 MOV EAX, [MultiDesk.CurWindow]
 MOV EBX, EAX
 DEC EAX
 
 CMP EAX, 0                      ; Switch To Next Window 
 JGE DO_SWITCH
 
 MOV EAX, [MAX_DESKTOPS] 
 DEC EAX
 JMP DO_SWITCH


  
WUSER:                                  ; System Icon Notify

 CMP [wparam], IDC_TRAYICON
 JNE THEDEFAULT
 
 CMP [lparam], WM_LBUTTONDOWN
 JE SHORT LeftButton


 JMP THEDEFAULT
 
LeftButton:                             ; Left Button

  PUSH  OFFSET CursorPosition
  CALL GetCursorPos
  
  PUSH  [hwnd]
  CALL SetForegroundWindow
  
  PUSH 0
  PUSH  [hwnd]
  PUSH  0
  PUSH  [CursorPosition.y]
  PUSH  [CursorPosition.x]
  PUSH  TPM_LEFTBUTTON + TPM_CENTERALIGN
  PUSH  [PopupMenuHandle]
  CALL TrackPopupMenu                           ; Track Popup Menu
  
 XOR EAX, EAX
 RET

 MOV EAX, [MultiDesk.CurWindow]

 INC EAX
 CMP EAX, [MAX_DESKTOPS]                  ; Switch To Next Window
 JB SHORT OK
 
 XOR EAX, EAX
 
OK: 
 MOV EBX, EAX
 CALL SwitchDeskTop
 
 XOR EAX, EAX
 RET


 
WDESTROY:
 
 PUSH  [PopupMenuHandle]               ; Clean up Menu
 CALL DestroyMenu
 
 PUSH  OFFSET NoteData
 PUSH  NIM_DELETE
 CALL Shell_NotifyIcon
 
 PUSH  IDC_HOTKEY1
 PUSH  [hwnd]                          ; Clean UP
 CALL UnregisterHotKey
 
 PUSH  IDC_HOTKEY2
 PUSH  [hwnd]
 CALL UnregisterHotKey
 
 XOR EAX, EAX
 RET
 WndProc   ENDP


;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Switch DeskTop                ;
; On Entry, EAX = New Window    ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 SwitchDeskTop PROC
   PUSHA
   
    CALL GetDesktopWindow
    MOV [DeskWindow], EAX
    
    PUSH  [DeskWindow]
    CALL LockWindowUpdate                       ; Lock Window Update
    
    
    MOV [TempHwnd], 0
    
    PUSH  0
    PUSH  OFFSET EnumWindowsProc               ; Get All Currently Open Windows
    CALL EnumWindows
    
    MOV EAX, [MultiDesk.CurWindow]
    SHL EAX, 2
    MOV EDX, [TempHwnd]
    MOV [MultiDesk.DeskTops + EAX], EDX         ; Save The Linked List
  
 
   MOV [MultiDesk.CurWindow], EBX
   SHL EBX, 2
   MOV EAX, EBX
   MOV EAX, [MultiDesk.DeskTops + EAX]          ; Get Linked List & Reset
   MOV [MultiDesk.DeskTops + EBX], 0
   
SHOW_ALL_WINDOWS:                               ; Show All Current Desktop Windows
   TEST EAX, EAX                                ; Test For Zero
   JZ SHORT SWITCH_DONE

     PUSH EAX 
     PUSH  SW_SHOW                             ; Show Window
     PUSH  DWORD PTR [EAX]
     CALL ShowWindow
     POP EAX           
     
     MOV EBX, [EAX + 8]                         ; Next Window
     
     PUSH  EAX
     CALL LocalFree                             ; Free Old Window
     
     MOV EAX, EBX
   JMP SHORT SHOW_ALL_WINDOWS
        
SWITCH_DONE:   

  PUSH  0
  CALL LockWindowUpdate                         ; Re-enable Window Update
  
  POPA
  RET
SwitchDeskTop ENDP 
 
 
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; Dialog Box Procedure
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AboutProc PROC hDlg:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
 MOV EAX, wmsg

 CMP EAX, WM_INITDIALOG
 JE SHORT WDINIT

 CMP EAX, WM_CREATE
 JE SHORT WDINIT

 CMP EAX, WM_COMMAND            ; Command Issued?
 JE SHORT WCOMMAND2

 CMP EAX, WM_TIMER
 JE SHORT WTIMER

 MOV EAX, FALSE                 ; No, Then Exit
 RET
 WDINIT:

 PUSH  0
 PUSH  9000
 PUSH  1
 PUSH  [hDlg]
 CALL SetTimer

 MOV EAX, TRUE
 RET
 WCOMMAND2:
    MOV EAX, wparam             ; Check Command

    CMP AX, IDOK                ; OK Pressed?
    JE SHORT Process

    CMP AX, IDCANCEL            ; Cancel Pressed?
    JNE SHORT Done

 WTIMER:
   Process:
    PUSH 1
    PUSH  [hDlg]
    call KillTimer

    PUSH  0
    PUSH [hDlg]
    Call EndDialog              ; Close Dialog
   Done:
    
   MOV EAX, TRUE                ; Return TRUE
   RET 

AboutProc ENDP
 
 ;-=-=-=-=-=-=-=-=-=-;
 ; Enum Windows      ;
 ;-=-=-=-=-=-=-=-=-=-;
 EnumWindowsProc  PROC USES EBX EDI ESI, hDlg:DWORD, lparam:DWORD

  PUSH  [hDlg]
  CALL IsWindowVisible
  
  CMP EAX, TRUE                 ; Only Process Visible Windows
  JE SHORT XHERE
  
  MOV EAX, TRUE
  RET
  
XHERE:
                                ; Check Make Sure It's not the System Tray Window
  PUSH  25
  PUSH  OFFSET ThisClassName
  PUSH  [hDlg]
  CALL GetClassName
  
  MOV ESI, OFFSET SystemTray     
  MOV EDI, OFFSET ThisClassName ; Scan it!
  
  MOV ECX, 14
  
TEST_TRAY:
    CMPSB
    JNZ SHORT NOT_TRAY          ; Test For System Tray
  LOOP SHORT TEST_TRAY
  
  MOV EAX, TRUE
  RET
  
NOT_TRAY:                       ; Not Tray, Test for ProgMan

  MOV ESI, OFFSET ProgMan
  MOV EDI, OFFSET ThisClassName ; Scan it!
  
  MOV ECX, 8
  
TEST_PROG:
    CMPSB
    JNZ SHORT DONE_ALL          ; Test For System Tray
  LOOP SHORT TEST_PROG
  
  MOV EAX, TRUE
  RET
  
DONE_ALL:
  
  CMP [TempHwnd], 0
  JNE SHORT ADDON
  
  PUSH  SIZE Desk
  PUSH  LMEM_FIXED
  CALL LocalAlloc               ; Allcoate Memory
  
  TEST EAX, EAX                 ; Test Memory Allocation
  JZ ERROR
 
  MOV [TempHwnd], EAX
  MOV EBX, [hDlg]
  MOV [EAX], EBX
  MOV DWORD PTR [EAX + 4], 0
  MOV DWORD PTR [EAX + 8], 0

  PUSH SW_HIDE
  PUSH  [hDlg]
  CALL ShowWindow        


  MOV EAX, TRUE
  RET
ADDON:
 
  
  MOV EAX, [TempHwnd]

  
LOOPIE:  
  
  CMP DWORD PTR [EAX + 8], 0
  JZ SHORT ADDIT


  MOV EAX, [EAX + 8]
  JMP SHORT LOOPIE
  
ADDIT: 
  MOV EBX, EAX                  ; Save Handle
    
  
  PUSH  SIZE Desk
  PUSH  LMEM_FIXED
  CALL LocalAlloc               ; Allocate Memory
  
  TEST EAX, EAX
  JZ SHORT ERROR                ; Test Memory Allocation
  
  MOV [EBX + 8], EAX            ; Set Next Pointer
  MOV [EAX + 4], EBX            ; Set Before Pointer
  MOV DWORD PTR [EAX + 8], 0    ; Set Next Pointer
  MOV EBX, [hDlg]
  MOV [EAX], EBX                ; Set Window
  
  PUSH  SW_HIDE
  PUSH  [hDlg]
  CALL ShowWindow       

  MOV EAX, TRUE
  RET
  
ERROR:
  MOV EAX, FALSE
  RET
EnumWindowsProc ENDP 
  
 END WinMain


