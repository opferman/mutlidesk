;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Toby Opferman                                               ;
;                   M U L T I  -  D E S K                     ;
;                                                             ;
; Virtual Desktop                                             ;
;                                                             ;
; http://www.nauticom.net/www/secret                          ;
;                                                             ;
; _Secret/Scrat                                               ;
; secret@pgh.nauticom.net                                     ;
; Copyright 1999                                              ;
;                                                             ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;

; Set Up Processor & Model
.486p
.MODEL FLAT, STDCALL

; Include Windows Structures 
INCLUDE WIN32.INC

PUBLIC WndProc

.data
;-=-=-=-=-=-=-=-=-=-=-=-;
; Data Structures       ;
;-=-=-=-=-=-=-=-=-=-=-=-;
 Desk    STRUC
     ThisHwnd     DWORD ?
     BeforeWindow DWORD ?
     AfterWindow  DWORD ?
;     DeskTopNum   DWORD ?
;     Position     DWORD ?
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
 
 CursorPosition POINT <?>
 TheClassName  db         'MultiDesk', 0
 TrayName      db         'MultiDesk v1.0 Alpha', 0
 SystemTray    db         'Shell_TrayWnd', 0
 ProgMan       db         'Progman', 0
 HelpString    db         'MULTIDESK.HLP',0
 MenuString1   db         'DeskTop One', 0
 MenuString2   db         'DeskTop Two', 0
 MenuString3   db         'DeskTop Three', 0
 MenuString4   db         'DeskTop Four', 0
 MenuString5   db         'About', 0
 MenuString6   db         'Help', 0
   
 DeskWindow    DWORD      ?
 ThisClassName db         26 DUP(?)
 MultiDesk     DeskTop    <?>
 WinhInstance  DWORD       ?
 WC            WNDCLASS   <?>
 HWND          DWORD       ?
 DeskIcon      DWORD      101
 AboutText     db         'AboutBox', 0
 MSG           MSGSTRUCT  <?>
 TempHwnd      DWORD       ?
 NoteData      NOTIFYICONDATAA  <?>
 PopupMenuHandle DWORD ?
 
;-=-=-=-=-=-=-=-=-=-=-=-=-;
; Main Windows Procedure  ;
;-=-=-=-=-=-=-=-=-=-=-=-=-;
.CODE

WinMain:
 
 PUSH L 0
 CALL GetModuleHandle
 MOV [WinhInstance], EAX                ; Get HINSTANCE

 PUSH L 0
 PUSH L OFFSET TheClassName
 CALL FindWindow                        ; Is Window Running?
 
 TEST EAX, EAX
 JZ SHORT NOT_RUNNING                   ; Is program already running?
 
 PUSH L 0
 CALL ExitProcess                       ; Exit If Program is already running
 
NOT_RUNNING: 
 CALL SetDeskTop                        ; Setup DeskTop 

 CALL InitWindow                        ; Intialize The Window
 TEST EAX, EAX

 CALL DisplayWindow                     ; Display The Window
 TEST EAX, EAX
 
 PUSH L SW_HIDE
 PUSH L EAX
 CALL ShowWindow                        ; Show Window
 
 PUSH L [HWND]
 CALL UpdateWindow                      ; Update Window
 
GetMessageLoop:
   PUSH L 0
   PUSH L 0
   PUSH L 0
   PUSH OFFSET MSG
   CALL GetMessage
 
   TEST EAX, EAX                        ; When GetMessage Returns a 0, Window Destoryed
   JZ SHORT QUIT
   
   PUSH OFFSET MSG
   CALL TranslateMessage        
   
   PUSH OFFSET MSG
   CALL DispatchMessage
 
 JMP SHORT GetMessageLoop
 
QUIT:

 PUSH L 0
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
 ENDP SetDeskTop

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
     PUSH L SW_SHOW                     ; Show Window
     PUSH L DWORD PTR [EAX]
     CALL ShowWindow
     POP EAX           
     
     MOV EBX, [EAX + 8]                 ; Next Window
     
     PUSH L EAX
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
 ENDP RestoreDesktop
 


;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Register Window               ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
  InitWindow PROC
    
    MOV [WC.clsHInstance], EAX
    MOV [WC.clsStyle], 0
    MOV [WC.clsLpfnWndProc], OFFSET WndProc
    MOV [WC.clsCbClsExtra], 0
    MOV [WC.clsCbWndExtra], 0

    PUSH L [DeskIcon]
    PUSH EAX
    CALL LoadIcon

    MOV [WC.clsHIcon], EAX
    MOV [WC.hIconSm], EAX

    PUSH L IDC_ARROW
    PUSH L 0
    CALL LoadCursor

    MOV [WC.clsHCursor], EAX

    
    MOV [WC.clsHbrBackground], 0
    MOV [WC.clsLpszMenuName],  0
    MOV [WC.clsLpszClassName], OFFSET TheClassName

    PUSH OFFSET WC
    CALL RegisterClass                     

  RET
  ENDP InitWindow

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Create Window                 ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
  DisplayWindow  PROC
    PUSH L 0
    PUSH [WinhInstance]
    PUSH L 0
    PUSH L 0
    PUSH L CW_USEDEFAULT
    PUSH L CW_USEDEFAULT
    PUSH L CW_USEDEFAULT
    PUSH L CW_USEDEFAULT
    PUSH L 0
    PUSH L 0
    PUSH OFFSET TheClassName
    PUSH L 0
    CALL CreateWindowEx
    MOV [HWND], EAX
  RET
  ENDP DisplayWindow

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
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [wmsg]
  PUSH L [hwnd]
  CALL DefWindowProc  
  RET
  
WCREATE:                        ; WM_CREATE Message
  
  PUSH L 0
  PUSH OFFSET AboutProc 
  PUSH [hwnd]
  PUSH OFFSET AboutText
  PUSH [WinhInstance]
  CALL DialogBoxParam           ; Call About Box 
                        
                                ; Setup SystemTray
                                
  MOV EAX, [hwnd]
  MOV [NoteData.cbSize], SIZE NOTIFYICONDATAA
  
  MOV [NoteData.hWnd], EAX
  MOV [NoteData.uID], IDC_TRAYICON
  MOV [NoteData.uFlags2], NIF_MESSAGE + NIF_ICON + NIF_TIP
  MOV [NoteData.uCallbackMessage], WM_USER
  
  PUSH L [DeskIcon]
  PUSH L [WinhInstance]
  CALL LoadIcon
  
  MOV [NoteData.hIcon], EAX  
  
  
  MOV EDI, OFFSET NoteData.szTip
  MOV ECX, 21
  MOV ESI, OFFSET TrayName
  REP MOVSB
  
  PUSH L OFFSET NoteData
  PUSH L NIM_ADD
  CALL Shell_NotifyIcon                        ; Set up Systray ICON
  
  PUSH L VK_RIGHT
  PUSH L MOD_ALT + MOD_CONTROL
  PUSH L IDC_HOTKEY1
  PUSH L [hwnd]
  CALL RegisterHotKey                          ; Register HOTKEYS
  
  PUSH L VK_LEFT
  PUSH L MOD_ALT + MOD_CONTROL
  PUSH L IDC_HOTKEY2
  PUSH L [hwnd]
  CALL RegisterHotKey                          ; Register HOTKEYS  
  
  CALL CreatePopupMenu
  
  TEST EAX, EAX 
  JZ  EXIT_CREATE                         ; Test Window handle
  
  MOV [PopupMenuHandle], EAX
  
  PUSH L OFFSET MenuString1                    ; Create Menu Popup
  PUSH L IDC_POP1
  PUSH L MF_STRING
  PUSH EAX
  CALL AppendMenu
  
  PUSH L MF_CHECKED
  PUSH L IDC_POP1
  PUSH L [PopupMenuHandle]
  CALL CheckMenuItem                           ; Check Menu Item
  
  PUSH L OFFSET MenuString2                    ; Create Menu Popup
  PUSH L IDC_POP2
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
  CALL AppendMenu

  PUSH L OFFSET MenuString3                    ; Create Menu Popup
  PUSH L IDC_POP3
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
  CALL AppendMenu

  PUSH L OFFSET MenuString4                    ; Create Menu Popup
  PUSH L IDC_POP4
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
  CALL AppendMenu
   
  PUSH L OFFSET MenuString5                    ; Create Menu Popup
  PUSH L IDC_POP5
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
  CALL AppendMenu

  PUSH L OFFSET MenuString6                    ; Create Menu Popup
  PUSH L IDC_POP6
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
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

 JMP THEDEFAULT

DO_SWITCH:

  PUSH EAX
  PUSH L MF_CHECKED
  PUSH EAX
  PUSH L [PopupMenuHandle]
  CALL CheckMenuItem                           ; Check Menu Item

  PUSH L MF_UNCHECKED
  PUSH EBX
  PUSH L [PopupMenuHandle]
  CALL CheckMenuItem                           ; UnCheck Menu Item
  
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
 JNE SHORT DO_SWITCH

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
  PUSH L 0
  PUSH OFFSET AboutProc 
  PUSH [hwnd]
  PUSH OFFSET AboutText
  PUSH [WinhInstance]
  CALL DialogBoxParam           ; Call About Box 

 XOR EAX, EAX
 RET

P6:
 
 PUSH L 0
 PUSH L HELP_CONTENTS   
 PUSH L OFFSET HelpString
 PUSH L [hwnd]
 CALL WinHelp
 
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
 
 CMP [lparam], WM_RBUTTONDOWN
 JE SHORT RightButton
 JMP THEDEFAULT
 
LeftButton:                             ; Left Button

  PUSH L OFFSET CursorPosition
  CALL GetCursorPos
  
  PUSH L [hwnd]
  CALL SetForegroundWindow
  
  PUSH L 0
  PUSH L [hwnd]
  PUSH L 0
  PUSH L [CursorPosition.y]
  PUSH L [CursorPosition.x]
  PUSH L TPM_LEFTBUTTON + TPM_CENTERALIGN
  PUSH L [PopupMenuHandle]
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

RightButton:                            ; Right Button Kill Window

 CALL RestoreDesktop
 
 PUSH L 0
 CALL PostQuitMessage

 XOR EAX, EAX
 RET
 
WDESTROY:
 
 PUSH L [PopupMenuHandle]               ; Clean up Menu
 CALL DestroyMenu
 
 PUSH L OFFSET NoteData
 PUSH L NIM_DELETE
 CALL Shell_NotifyIcon
 
 PUSH L IDC_HOTKEY1
 PUSH L [hwnd]                          ; Clean UP
 CALL UnregisterHotKey
 
 PUSH L IDC_HOTKEY2
 PUSH L [hwnd]
 CALL UnregisterHotKey
 
 XOR EAX, EAX
 RET
 ENDP WndProc  


;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Switch DeskTop                ;
; On Entry, EAX = New Window    ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 SwitchDeskTop PROC
   PUSHA
   
    CALL GetDesktopWindow
    MOV [DeskWindow], EAX
    
    PUSH L [DeskWindow]
    CALL LockWindowUpdate                       ; Lock Window Update
    
    
    MOV [TempHwnd], 0
    
    PUSH L 0
    PUSH L OFFSET EnumWindowsProc               ; Get All Currently Open Windows
    CALL EnumWindows
    
    MOV EAX, [MultiDesk.CurWindow]
    SHL EAX, 2
    MOV EDX, [TempHwnd]
    MOV [MultiDesk.DeskTops + EAX], EDX         ; Save The Linked List
  
;    MOV EAX, EDX                                ; Start The Loop
;LOOP_HIDE_WINDOWS:

;   TEST EAX, EAX
;   JZ SHORT ALL_HIDEN                           ; Hide All The Windows
   
 
;   PUSH EAX
   
;   PUSH L SW_HIDE
;   PUSH L DWORD PTR [EAX]
;   CALL ShowWindow                              ; Hide Window

;   POP EAX
   
;   MOV EAX, [EAX + 8]                           ; Next Window

;  JMP SHORT LOOP_HIDE_WINDOWS
  
;ALL_HIDEN:
 
   MOV [MultiDesk.CurWindow], EBX
   SHL EBX, 2
   MOV EAX, EBX
   MOV EAX, [MultiDesk.DeskTops + EAX]          ; Get Linked List & Reset
   MOV [MultiDesk.DeskTops + EBX], 0
   
SHOW_ALL_WINDOWS:                               ; Show All Current Desktop Windows
   TEST EAX, EAX                                ; Test For Zero
   JZ SHORT SWITCH_DONE

     PUSH EAX 
     PUSH L SW_SHOW                             ; Show Window
     PUSH L DWORD PTR [EAX]
     CALL ShowWindow
     POP EAX           
     
     MOV EBX, [EAX + 8]                         ; Next Window
     
     PUSH L EAX
     CALL LocalFree                             ; Free Old Window
     
     MOV EAX, EBX
   JMP SHORT SHOW_ALL_WINDOWS
        
SWITCH_DONE:   

  PUSH L 0
  CALL LockWindowUpdate                         ; Re-enable Window Update
  
  POPA
  RET
 ENDP SwitchDeskTop
 
 
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

 PUSH L 0
 PUSH L 9000
 PUSH L 1
 PUSH L [hDlg]
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
    PUSH L 1
    PUSH L [hDlg]
    call KillTimer

    PUSH L 0
    PUSH [hDlg]
    Call EndDialog              ; Close Dialog
   Done:
    
   MOV EAX, TRUE                ; Return TRUE
   RET 

ENDP
 
 ;-=-=-=-=-=-=-=-=-=-;
 ; Enum Windows      ;
 ;-=-=-=-=-=-=-=-=-=-;
 EnumWindowsProc  PROC USES EBX EDI ESI, hDlg:DWORD, lparam:DWORD

  PUSH L [hDlg]
  CALL IsWindowVisible
  
  CMP EAX, TRUE                 ; Only Process Visible Windows
  JE SHORT XHERE
  
  MOV EAX, TRUE
  RET
  
XHERE:
                                ; Check Make Sure It's not the System Tray Window
  PUSH L 25
  PUSH L OFFSET ThisClassName
  PUSH L [hDlg]
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
  
  PUSH L SIZE Desk
  PUSH L LMEM_FIXED
  CALL LocalAlloc               ; Allcoate Memory
  
  TEST EAX, EAX                 ; Test Memory Allocation
  JZ ERROR
 
  MOV [TempHwnd], EAX
  MOV EBX, [hDlg]
  MOV [EAX], EBX
  MOV DWORD PTR [EAX + 4], 0
  MOV DWORD PTR [EAX + 8], 0

  PUSH L SW_HIDE
  PUSH L [hDlg]
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
    
  
  PUSH L SIZE Desk
  PUSH L LMEM_FIXED
  CALL LocalAlloc               ; Allocate Memory
  
  TEST EAX, EAX
  JZ SHORT ERROR                ; Test Memory Allocation
  
  MOV [EBX + 8], EAX            ; Set Next Pointer
  MOV [EAX + 4], EBX            ; Set Before Pointer
  MOV DWORD PTR [EAX + 8], 0    ; Set Next Pointer
  MOV EBX, [hDlg]
  MOV [EAX], EBX                ; Set Window
  
  PUSH L SW_HIDE
  PUSH L [hDlg]
  CALL ShowWindow       

  MOV EAX, TRUE
  RET
  
ERROR:
  MOV EAX, FALSE
  RET
 ENDP EnumWindowsProc
  
 END WinMain
  