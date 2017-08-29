;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Toby Opferman                                               ;
;                   M U L T I  -  D E S K                     ;
;                            2.0                              ;
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
; Data Area             ;
;-=-=-=-=-=-=-=-=-=-=-=-;
 MAX_DESKTOPS  DWORD  8                 ; Goes by Power of 2 + 1 (2^3 is 4 desktops)
 CurWindow     DWORD  1
 
 IDC_TRAYICON  EQU <1000>
 IDC_HOTKEY1   EQU <1001>
 IDC_HOTKEY2   EQU <1002>
 
 IDC_POP1      EQU <1>
 IDC_POP2      EQU <2>
 IDC_POP3      EQU <4>
 IDC_POP4      EQU <8>
 IDC_POP5      EQU <16>
 IDC_POP6      EQU <32>
 IDC_POP7      EQU <64>
 
 TheClassName  db         'MultiDesk', 0
 TrayName      db         'MultiDesk v1.01 Alpha', 0
 HelpString    db         'MULTIDESK.HLP',0
 MenuString1   db         'DeskTop One', 0
 MenuString2   db         'DeskTop Two', 0
 MenuString3   db         'DeskTop Three', 0
 MenuString4   db         'DeskTop Four', 0
 MenuString5   db         'Configure Desktops', 0
 MenuString6   db         'About', 0
 MenuString7   db         'Help', 0

 ExitTitleMsg  db         'Quit MDesk?', 0
 ExitWinMsg    db         'Do You Want to Quit Multi Desk?', 0

 DeskIcon      DWORD      101
 AboutText     db         'AboutBox', 0

 CursorPosition POINT <?>
  
 WinhInstance  DWORD       ?
 WC            WNDCLASS   <?>
 HWND          DWORD       ?
 MSG           MSGSTRUCT  <?>
 
 NoteData        NOTIFYICONDATAA  <?>
 PopupMenuHandle DWORD ?

 ; Functions in the DLL
 
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

;*******************;
; WM_CREATE         ;
;*******************;
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
  JZ  EXIT_CREATE                              ; Test Window handle
  
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

  PUSH L OFFSET MenuString7                    ; Create Menu Popup
  PUSH L IDC_POP7
  PUSH L MF_STRING
  PUSH L [PopupMenuHandle]
  CALL AppendMenu
  
    
EXIT_CREATE:
  XOR EAX, EAX                                 
  RET
  
  
;*******************;
; WM_COMMAND        ;
;*******************;

WCOMMAND:                                      

  
 MOV EAX, [wparam]
 AND EAX, 0FFFFh                ; Check For Menu Options
 
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

 JMP THEDEFAULT                 ; None Chosen



DO_SWITCH:                                     ; Switch Desktop

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
  
  MOV [CurWindow], EBX                         ; Set New Window
   
  CALL [SwitchDeskTop]                         ; Switch To Desktop
 
  XOR EAX, EAX                                 ; Exit
  RET
 
 
; Option 1

P1:
 
 MOV EBX, [CurWindow]
 MOV EAX, 1
 
 TEST EBX, EAX
 JZ SHORT DO_SWITCH

 XOR EAX, EAX
 RET


; Option 2
P2:
 MOV EBX, [CurWindow]
 MOV EAX, 2
 
 TEST EBX, EAX
 JZ SHORT DO_SWITCH

 XOR EAX, EAX
 RET



; Option 3
P3:
 MOV EBX, [CurWindow]
 MOV EAX, 4
 
 TEST EBX, EAX
 JZ SHORT DO_SWITCH

 XOR EAX, EAX
 RET

 
; Option 4
P4:
 MOV EBX, [CurWindow]
 MOV EAX, 8
 
 TEST EBX, EAX
 JZ DO_SWITCH

 XOR EAX, EAX
 RET

; Option 5
P5:
 
 CALL [ConfigureDesktops]
 
 XOR EAX, EAX
 RET

; Option 6
P6:
  PUSH L 0
  PUSH OFFSET AboutProc 
  PUSH [hwnd]
  PUSH OFFSET AboutText
  PUSH [WinhInstance]
  CALL DialogBoxParam           ; Call About Box 

 XOR EAX, EAX
 RET
 
 
; Option 7
P7:
 
 PUSH L 0
 PUSH L HELP_CONTENTS   
 PUSH L OFFSET HelpString
 PUSH L [hwnd]
 CALL WinHelp
 
 XOR EAX, EAX
 RET

;*******************;
; WM_HOTKEY         ;
;*******************;

WHOTKEY:                                ; Hot Key
 
 CMP [wparam], IDC_HOTKEY1              ; Check Hot Key 1
 JE SHORT HOT1
 
 CMP [wparam], IDC_HOTKEY2              ; Check Hot Key 2
 JE SHORT HOT2
 
 XOR EAX, EAX
 RET
 
HOT1:
 MOV EAX, [CurWindow]
 MOV EBX, EAX
 
 SHL EAX, 1
 CMP EAX, [MAX_DESKTOPS]                  ; Switch To Next Window
 JBE DO_SWITCH
 
 MOV EAX, 1
 JMP DO_SWITCH
  
 
HOT2:
 MOV EAX, [CurWindow]
 MOV EBX, EAX
 SHR EAX, 1
 
 TEST EAX, EAX                            ; Switch To Next Window 
 JNZ DO_SWITCH
 
 MOV EAX, [MAX_DESKTOPS] 
 
 JMP DO_SWITCH
 
 
 
;*******************;
; WM_USER           ;
;*******************;

WUSER:                                         ; System Icon Notify

 CMP [wparam], IDC_TRAYICON
 JNE THEDEFAULT
 
 CMP [lparam], WM_LBUTTONDOWN
 JE SHORT LeftButton
 
 CMP [lparam], WM_RBUTTONDOWN
 JE SHORT RightButton
 JMP THEDEFAULT
 
LeftButton:                                     ; Left Button     

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



RightButton:                                    ; Right Button Kill Window
 PUSH L MB_OK + MB_CANCEL
 PUSH L OFFSET ExitTitleMsg
 PUSH L OFFSET ExitWinMsg
 PUSH L [HWND]
 CALL MessageBox
 
 CMP EAX, FALSE                                 ; Check No Exit
 JE SHORT Exit_Button

 CALL [RestoreDesktop]                          ; Exit
 
 PUSH L 0
 CALL PostQuitMessage

Exit_Button:

 XOR EAX, EAX
 RET


;*******************;
; WM_DESTROY        ;
;*******************;
 
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
 
 
 

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; About Dialog Box Procedure
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AboutProc PROC hDlg:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD
 MOV EAX, wmsg
 
 CMP EAX, WM_COMMAND            ; Command Issued?
 JE SHORT WCOMMAND2
 
 MOV EAX, FALSE                 ; No, Then Exit
 RET
 

 WCOMMAND2:
    MOV EAX, wparam             ; Check Command

    CMP AX, IDOK                ; OK Pressed?
    JE SHORT Process

    CMP AX, IDCANCEL            ; Cancel Pressed?
    JNE SHORT Done

   Process:
    PUSH L 0
    PUSH [hDlg]
    Call EndDialog              ; Close Dialog
   Done:
    
   MOV EAX, TRUE                ; Return TRUE
   RET 

ENDP AboutProc



;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
; Load DLL Procedure
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LoadDLL PROC
 
 PUSH L OFFSET DllName
 CALL LoadLibrary
 TEST EAX, EAX
 JZ SHORT 
 RET
ENDP LoadDLL
 
 
 END WinMain
