;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Toby Opferman                                               ;
;                   M U L T I  -  D E S K                     ;
;                            2.0                              ;
; Virtual Desktop                                             ;
; DLL                                                         ;
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

EXPORT 
.data
;-=-=-=-=-=-=-=-=-=-=-=-;
; Data Structures       ;
;-=-=-=-=-=-=-=-=-=-=-=-;
 Desk    STRUC
     ThisHwnd     DWORD ?
     BeforeWindow DWORD ?
     AfterWindow  DWORD ?
     DeskTopNum   DWORD ?
 Desk    ENDS

 DeskTop STRUC
     CurWindow  DWORD   ?               ; Current Window
     Windows    DWORD   ?               ; Window Handles
 DeskTop ENDS

;-=-=-=-=-=-=-=-=-=-=-=-=-;
;      Constants          ;
;-=-=-=-=-=-=-=-=-=-=-=-=-;

 SystemTray    db         'Shell_TrayWnd', 0
 ProgMan       db         'Progman', 0
 MAX_DESKTOPS  DWORD  8                 ; Goes by Power of 2 + 1 (2^3 is 4 desktops)
 
 
;-=-=-=-=-=-=-=-=-=-=-=-=-;
;       The Data          ;
;-=-=-=-=-=-=-=-=-=-=-=-=-;
 DeskWindow    DWORD      ?
 ThisClassName db         26 DUP(?)
 HookID        DWORD      ?
 HookID        DWORD      ?
 Parent        DWORD       ?
 TempHwnd      DWORD       ?
 MultiDesk     DeskTop    <?>

 
; The Program Code
.CODE

 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
 ;  DLL Entry Point        ;
 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
WinDLL:
DllMain PROC hInstDLL:HINSTANCE, reason:DWORD, reserved1:DWORD 
        MOV  EAX,TRUE 
        RET 
DllMain ENDP

 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
 ;  System Message Hook    ;
 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
 MdeskGetMsgProc PROC nCode:DWORD, wparam:DWORD, lparam:DWORD
   
 ENDP MdeskGetMsgProc
 
 
 

 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
 ;   System SHell Hook     ;
 ;-=-=-=-=-=-=-=-=-=-=-=-=-;
 MdeskShellProc PROC nCode:DWORD, wparam:DWORD, lparam:DWORD
  
  CMP [nCode], HSHELL_WINDOWCREATED              ; Window Created?
  JE SHORT Create_Window
  
  

;  CMP [nCode], 0 
;  JGE SHORT Return_Zero
  
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx
  RET

Return_Zero:

  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx
  

  XOR EAX, EAX
  RET

Create_Window:

  
  PUSH L [wparam]
  CALL GetParent                        ; Get Parent Window
  
  MOV [Parent], EAX
  MOV EAX, [MultiDesk.Windows]
 
  
  TEST EAX, EAX                         ; Test No Windows In List
  JZ SHORT Add_It_New
  
FIND_IT_LOOP:

  MOV EBX, [Parent]  

  CMP [EAX], EBX                        ; It's Parent in the list?
  JE Add_It_Old
  
  MOV EBX, [wparam]
  
  CMP [EAX], EBX                        ; It in the list?
  JE Just_Hide_It
  
  CMP DWORD PTR [EAX + 8], 0                      ; No More Windows?
  JE SHORT Add_It_New
  
  MOV EAX, [EAX + 8]                    ; Next Window
  JMP SHORT FIND_IT_LOOP

Add_It_New:
  TEST EAX, EAX                         ; Are there any windows?
  JZ SHORT NO_WINDOWS
  MOV EBX, EAX
  PUSH L SIZE Desk
  PUSH L LMEM_FIXED
  CALL LocalAlloc                        ; Allocate Memory
  
  TEST EAX, EAX
  JZ  ERROR                              ; Test Memory Allocation
  
  MOV [EBX + 8], EAX
  MOV DWORD PTR [EAX + 8], 0            ; Set Nodes Up
  MOV [EAX + 4], EBX
  
  MOV EBX, [wparam]                     ; Set Window Handle
  MOV [EAX], EBX
  
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx  
 XOR EAX, EAX

  RET
 
NO_WINDOWS:

  PUSH L SIZE Desk
  PUSH L LMEM_FIXED
  CALL LocalAlloc                        ; Allocate Memory
  
  TEST EAX, EAX
  JZ  ERROR                              ; Test Memory Allocation
  
  MOV DWORD PTR [EAX + 8], 0            ; Set Nodes Up
  MOV DWORD PTR [EAX + 4], 0
 
  MOV EBX, [wparam]                     ; Set Window Handle
  MOV [EAX], EBX

  MOV [MultiDesk.Windows], EAX          ; Set First Node

  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx  
 XOR EAX, EAX

  RET

Add_It_Old:
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx  
 XOR EAX, EAX
 RET


Just_Hide_It:
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx
 XOR EAX, EAX
 RET

  
Destroy_Window:

   MOV EAX, [MultiDesk.Windows]                ; Get Windows Address
   MOV ECX, [wparam]
  
Test_For_Window:

    TEST EAX, EAX                             ; No More Windows?
    JZ DONE_SEARCHING

    CMP [EAX], ECX                            ; Check if Window is in list
    JE SHORT Erase_it

    MOV EAX, [EAX + 8]
    JMP SHORT  Test_For_Window                   ; Next Window

Erase_it:

    CMP EAX, [MultiDesk.Windows]              ; Check if Front Node
    JNE SHORT Not_Front
    
Front_Node:
    
    MOV EBX, EAX
    MOV ECX, [EBX + 8]
    MOV [MultiDesk.Windows], ECX              ; Set Front Node To Next Node in List
    
    XOR EDX, EDX
    JMP SHORT Set_Back
    
Not_Front:
    MOV ECX, [EAX + 8]
    MOV EDX, [EAX + 4]                        ; Elimate Window From List
    
    MOV [EDX + 8], ECX

Set_Back:
    TEST ECX, ECX                             ; Is Rear Node?
    JZ SHORT SKIP_THIS

    MOV [ECX + 4], EDX
    
SKIP_THIS:    
    PUSH EAX
    CALL LocalFree                            ; Free Window
    
  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx

 XOR EAX, EAX
 RET
    

DONE_SEARCHING:

  PUSH L [lparam]
  PUSH L [wparam]
  PUSH L [nCode]
  PUSH L [HookID]
  CALL CallNextHookEx

 XOR EAX, EAX
 RET

 ENDP MdeskShellProc





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
  MOV DWORD PTR [EAX + 12], 1
  

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
  MOV DWORD PTR [EAX + 12], 1   ; Set Window Number
  
  MOV EBX, [hDlg]
  MOV [EAX], EBX                ; Set Window
  
  MOV EAX, TRUE
  RET
  
ERROR:
  
  MOV EAX, FALSE
  RET
 ENDP EnumWindowsProc
 
 
 
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Switch DeskTop                ;
; On Entry, EBX = New Window    ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 SwitchDeskTop PROC
   PUSHA
   
   CALL GetDesktopWindow
    MOV [DeskWindow], EAX
    
    PUSH L [DeskWindow]
    CALL LockWindowUpdate                       ; Lock Window Update
    
    MOV EAX, [MultiDesk.Windows]                ; Get Windows Address
    MOV ECX, [MultiDesk.CurWindow]              ; Get Current Window
    
DO_THE_SWITCH:        

    TEST EAX, EAX                               ; No More Windows?
    JZ SHORT SWITCH_DONE

    TEST [EAX + 12], ECX                        ; Check Current Window Active
    JZ SHORT Check_Appearance
  

    TEST [EAX + 12], EBX                        ; Check if Remains Active
    JNZ SHORT Next_Window

    PUSHA
    PUSH L SW_HIDE
    PUSH L DWORD PTR [EAX]
    CALL ShowWindow                             ; Hide Window
    POPA
    
    JMP SHORT Next_Window
    
Check_Appearance:
    TEST DWORD PTR [EAX + 12], EBX              ; Check if Becomes Active
    JZ SHORT Next_Window
    
    PUSHA
    PUSH L SW_SHOW
    PUSH L DWORD PTR [EAX]
    CALL ShowWindow                             ; Show Window
    POPA
        
Next_Window:

    MOV EAX, [EAX + 8]
    JMP SHORT DO_THE_SWITCH
    
SWITCH_DONE:   
  
  MOV [MultiDesk.CurWindow], EBX                ; Switch Next Window
  
  PUSH L 0
  CALL LockWindowUpdate                         ; Re-enable Window Update
  
  POPA
  RET
 ENDP SwitchDeskTop




;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Set Up DeskTop                ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 SetDeskTop PROC
  PUSHA

  
  MOV [TempHwnd], 0
   
  PUSH L 0
  PUSH L OFFSET EnumWindowsProc               ; Get All Currently Open Windows
  CALL EnumWindows
  
  MOV [MultiDesk.CurWindow], 1                ; Set Current Window
  
  MOV EAX, [TempHwnd]
  MOV [MultiDesk.Windows], EAX                ; Set Pointers

  
  PUSH L 0 
  PUSH L [WinhInstance]
  PUSH L OFFSET MdeskShellProc
  PUSH L WH_SHELL
  CALL SetWindowsHookEx         ; Install System Hook
  MOV [HookIDSh], EAX
  
  PUSH L 0 
  PUSH L [WinhInstance]
  PUSH L OFFSET MdeskGetMsgProc
  PUSH L WH_GETMESSAGE
  CALL SetWindowsHookEx
  MOV [HookIDMsg], EAX
  
  POPA
  RET
 ENDP SetDeskTop

;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
; Restore Desktop               ;
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-;
 RestoreDesktop PROC
  PUSHA
  
  PUSH L [HookID]
  CALL UnhookWindowsHookEx              ; UnInstall System Hook

  
  MOV ECX, [MultiDesk.CurWindow]
  MOV EAX, [MultiDesk.Windows]          ; Get All Desktops
  
 
 RESTORE_LOOP:
   TEST EAX, EAX                        ; Test For Zero
   JZ SHORT ALLDONE
   
   TEST DWORD PTR [EAX + 12], ECX
   JNZ SHORT Next_Window_Loop

    
     PUSH EAX
     PUSH L SW_SHOW                     ; Show Window
     PUSH L DWORD PTR [EAX]
     CALL ShowWindow
     POP EAX      
          
 Next_Window_Loop:    
     MOV EBX, [EAX + 8]                 ; Next Window
 
     PUSH L EAX
     CALL LocalFree                     ; Free Old Window
     
     MOV EAX, EBX
   JMP SHORT RESTORE_LOOP
   
 ALLDONE:
    

  POPA
  RET
 ENDP RestoreDesktop
 
 
 END WinDLL
