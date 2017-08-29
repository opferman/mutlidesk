;*************************************************************************
;
;       WIN32.INC - Win32 assembly language structures & constants
;
;
;*************************************************************************
;
;       C/C++ Run Time Library - Version 6.0
;
;       Copyright (c) 1985, 1993 by Borland International
;       All Rights Reserved.
;
;
; Updated & Maintained by Toby Opferman
;   http://www.nauticom.net/www/secret
;   secret@pgh.nauticom.net
;*************************************************************************

UCHAR   EQU <db>
USHORT  EQU <dw>  ; used only if we really need 16 bits
UINT    EQU <dd>  ; 32 bits for WIN32
ULONG   EQU <dd>
HDC     EQU <dd>
HBITMAP EQU <dd>
HICON   EQU <dd>

FALSE   EQU <0>
TRUE    EQU <1>


;*******************************************************************
;
;       Window's externals
;
;*******************************************************************
extrn    AppendMenuA:PROC
extrn    BeginPaint:PROC
extrn    BitBlt:PROC
extrn    CloseHandle:PROC
extrn    CreateCompatibleDC:PROC
extrn    CreateCompatibleBitmap:PROC
extrn    CreateToolbar:PROC
extrn    CreateToolbarEx:PROC
extrn    CreatePopupMenu:PROC
extrn    CreateWindowExA:PROC
extrn    CheckMenuItem:PROC
extrn    DestroyMenu:PROC
extrn    DefWindowProcA:PROC
extrn    DeleteDC:PROC
extrn    DialogBoxParamA:PROC
extrn    DispatchMessageA:PROC
extrn    EndDialog:PROC
extrn    EndDoc:PROC
extrn    EndPaint:PROC
extrn    EndPage:PROC
extrn    ExitProcess:PROC
extrn    EnumWindows:PROC
extrn    FindWindowA:PROC
extrn    FillRect:PROC
extrn    GetClassNameA:PROC
extrn    GetClientRect:PROC
extrn    GetDC:PROC
extrn    GetDlgItem:PROC
extrn    GetDlgItemTextA:PROC
extrn    GetParent:PROC
extrn    GetLocalTime:PROC
extrn    GetDesktopWindow:PROC
extrn    GetMessageA:PROC
extrn    GetCursorPos:PROC
extrn    GetModuleHandleA:PROC
extrn    GetOpenFileNameA:PROC
extrn    GetSaveFileNameA:PROC
extrn    GetStockObject:PROC
extrn    GetSystemMetrics:PROC
extrn    GetTextMetricsA:PROC
extrn    GetWindowRect:PROC
extrn    GetWindow:PROC
extrn    GetWindowPlacement:PROC
extrn    GlobalAlloc:PROC
extrn    GlobalFree:PROC
extrn    InitCommonControls:PROC
extrn    IsWindowVisible:PROC
extrn    InvalidateRect:PROC
extrn    KillTimer:PROC
extrn    LocalFree:PROC
extrn    LoadAcceleratorsA:PROC
extrn    LoadBitmapA:PROC
extrn    LockWindowUpdate:PROC
extrn    LoadCursorA:PROC
extrn    LoadIconA:PROC
extrn    LoadMenuA:PROC
extrn    LoadStringA:PROC
extrn    LocalAlloc:PROC
extrn    MessageBeep:PROC
extrn    MessageBoxA:PROC
extrn    MoveWindow:PROC
extrn    OpenFile:PROC
extrn    PostQuitMessage:PROC
extrn    PrintDlgA:PROC
extrn    ReleaseCapture:PROC
extrn    ReleaseDC:PROC
extrn    ReadFile:PROC
extrn    RegisterHotKey:PROC
extrn    RegisterClassA:PROC
extrn    SelectObject:PROC
extrn    SendMessageA:PROC
extrn    SetCursor:PROC
extrn    SetCapture:PROC
extrn    SetRect:PROC
extrn    SetBkMode:PROC
extrn    SetTextColor:PROC
extrn    SetFocus:PROC
extrn    SetTimer:PROC
extrn    SetWindowPos:PROC
extrn    SetForegroundWindow:PROC
extrn    ShowWindow:PROC
extrn    StartDocA:PROC
extrn    StartPage:PROC
extrn    StretchBlt:PROC
extrn    Shell_NotifyIcon:PROC
extrn    timeGetTime:PROC
extrn    TextOutA:PROC
extrn    TrackPopupMenu:PROC
extrn    TranslateAcceleratorA:PROC
extrn    TranslateMessage:PROC
extrn    UpdateWindow:PROC
extrn    UnregisterHotKey:PROC
extrn    WinHelpA:PROC
extrn    WriteFile:PROC

AppendMenu       equ <AppendMenuA>
GetClassName     equ <GetClassNameA>
CreateWindowEx   equ <CreateWindowExA>
DefWindowProc    equ <DefWindowProcA>
DialogBoxParam   equ <DialogBoxParamA>
DispatchMessage  equ <DispatchMessageA>
FindWindow       equ <FindWindowA>
GetDlgItemText   equ <GetDlgItemTextA>
GetMessage       equ <GetMessageA>
GetModuleHandle  equ <GetModuleHandleA>
GetOpenFileName  equ <GetOpenFileNameA>
GetSaveFileName  equ <GetSaveFileNameA>
GetTextMetrics   equ <GetTextMetricsA>
LoadAccelerators equ <LoadAcceleratorsA>
LoadBitmap       equ <LoadBitmapA>
LoadCursor       equ <LoadCursorA>
LoadIcon         equ <LoadIconA>
LoadMenu         equ <LoadMenuA>
LoadString       equ <LoadStringA>
MessageBox       equ <MessageBoxA>
PrintDlg         equ <PrintDlgA>
RegisterClass    equ <RegisterClassA>
SendMessage      equ <SendMessageA>
StartDoc         equ <StartDocA>
TextOut          equ <TextOutA>
TranslateAccelerator equ <TranslateAcceleratorA>
WinHelp          equ <WinHelpA>

L equ <LARGE>
;
; Commands to pass to WinHelp()
;
HELP_CONTEXT     = 0001h
HELP_QUIT        = 0002h
HELP_INDEX       = 0003h
HELP_CONTENTS    = 0003h
HELP_HELPONHELP  = 0004h
HELP_SETINDEX    = 0005h
HELP_SETCONTENTS = 0005h
HELP_CONTEXTPOPUP = 0008h
HELP_FORCEFILE   = 0009h
HELP_KEY         = 0101h
HELP_COMMAND     = 0102h
HELP_PARTIALKEY  = 0105h
HELP_MULTIKEY    = 0201h
HELP_SETWINPOS   = 0203h
HELP_CONTEXTMENU = 000ah
HELP_FINDER      = 000bh
HELP_WM_HELP     = 000ch
HELP_SETPOPUP_POS = 000dh

HELP_TCARD             = 8000h
HELP_TCARD_DATA        = 0010h
HELP_TCARD_OTHER_CALLER  = 0011h

IDH_NO_HELP                =     28440
IDH_MISSING_CONTEXT        =     28441
IDH_GENERIC_HELP_BUTTON    =     28442
IDH_OK                     =     28443
IDH_CANCEL                 =     28444
IDH_HELP                   =     28445
;*******************************************************************
;
;       OFSTRUCT
;
;*******************************************************************
OF_READ             = 00000000h
OF_WRITE            = 00000001h
OF_READWRITE        = 00000002h
OF_SHARE_COMPAT     = 00000000h
OF_SHARE_EXCLUSIVE  = 00000010h
OF_SHARE_DENY_WRITE = 00000020h
OF_SHARE_DENY_READ  = 00000030h
OF_SHARE_DENY_NONE  = 00000040h
OF_PARSE            = 00000100h
OF_DELETE           = 00000200h
OF_VERIFY           = 00000400h
OF_CANCEL           = 00000800h
OF_CREATE           = 00001000h
OF_PROMPT           = 00002000h
OF_EXIST            = 00004000h
OF_REOPEN           = 00008000h
OFS_MAXPATHNAME     = 128

OFSTRUCT struc
    cBytes UCHAR ?
    fFixedDisk UCHAR ?
    nErrCode USHORT ?
    Reserved1 USHORT ?
    Reserved2 USHORT ?
    szPathName db OFS_MAXPATHNAME dup(?)
OFSTRUCT ends


;*******************************************************************
;
;       OPENFILENAME
;
;*******************************************************************
OPENFILENAME struc
   lStructSize ULONG ?
   hwndOwner ULONG ?
   hInstance ULONG ?
   lpstrFilter ULONG ?
   lpstrCustomFilter ULONG ?
   nMaxCustFilter ULONG ?
   nFilterIndex ULONG ?
   lpstrFile ULONG ?
   nMaxFile ULONG ?
   lpstrFileTitle ULONG ?
   nMaxFileTitle ULONG ?
   lpstrInitialDir ULONG ?
   lpstrTitle ULONG ?
   Flags ULONG ?
   nFileOffset USHORT ?
   nFileExtension USHORT ?
   lpstrDefExt ULONG ?
   lCustData ULONG ?
   lpfnHook ULONG ?
   lpTemplateName ULONG ?
OPENFILENAME ends

OFN_READONLY              =   00000001h
OFN_OVERWRITEPROMPT       =   00000002h
OFN_HIDEREADONLY          =   00000004h
OFN_NOCHANGEDIR           =   00000008h
OFN_SHOWHELP              =   00000010h
OFN_ENABLEHOOK            =   00000020h
OFN_ENABLETEMPLATE        =   00000040h
OFN_ENABLETEMPLATEHANDLE  =   00000080h
OFN_NOVALIDATE            =   00000100h
OFN_ALLOWMULTISELECT      =   00000200h
OFN_EXTENSIONDIFFERENT    =   00000400h
OFN_PATHMUSTEXIST         =   00000800h
OFN_FILEMUSTEXIST         =   00001000h
OFN_CREATEPROMPT          =   00002000h
OFN_SHAREAWARE            =   00004000h
OFN_NOREADONLYRETURN      =   00008000h
OFN_NOTESTFILECREATE      =   00010000h
OFN_NONETWORKBUTTON       =   00020000h
OFN_NOLONGNAMES           =   00040000h
OFN_EXPLORER              =   00080000h
OFN_NODEREFERENCELINKS    =   00100000h
OFN_LONGNAMES             =   00200000h
OFN_SHAREFALLTHROUGH      =   2
OFN_SHARENOWARN           =   1
OFN_SHAREWARN             =   0



TPM_LEFTBUTTON  = 0
TPM_RIGHTBUTTON = 2
TPM_LEFTALIGN   = 0
TPM_CENTERALIGN = 4
TPM_RIGHTALIGN  = 8


; MOD keys 
MOD_ALT     =   1
MOD_CONTROL =   2
MOD_SHIFT   =   4

MF_UNCHECKED  =         0
MF_CHECKED    =         8

LMEM_FIXED          = 0000h
LMEM_MOVEABLE       = 0002h
LMEM_NOCOMPACT      = 0010h
LMEM_NODISCARD      = 0020h
LMEM_ZEROINIT       = 0040h
LMEM_MODIFY         = 0080h
LMEM_DISCARDABLE    = 0F00h
LMEM_VALID_FLAGS    = 0F72h
LMEM_INVALID_HANDLE = 8000h

MF_STRING     =        0
MF_BITMAP     =       4h
MF_OWNERDRAW  =      100h

;*******************************************************************
;
;       PRINTDLG
;
;*******************************************************************
PRINTDLG struc
   pdlStructSize ULONG ?
   pdhwndOwner ULONG ?
   pdhDevMode ULONG ?
   pdhDevNames ULONG ?
   pdhDC ULONG ?
   pdFlags ULONG ?
   pdnFromPage USHORT ?
   pdnToPage USHORT ?
   pdnMinPage USHORT ?
   pdnMaxPage USHORT ?
   pdnCopies USHORT ?
   pdhInstance ULONG ?
   pdlCustData ULONG  ?
   pdlpfnPrintHook ULONG ?
   pdlpfnSetupHook ULONG ?
   pdlpPrintTemplateName ULONG ?
   pdlpSetupTemplateName ULONG ?
   pdhPrintTemplate ULONG ?
   pdhSetupTemplate ULONG ?
PRINTDLG ends

PD_ALLPAGES              =    00000000h
PD_SELECTION             =    00000001h
PD_PAGENUMS              =    00000002h
PD_NOSELECTION           =    00000004h
PD_NOPAGENUMS            =    00000008h
PD_COLLATE               =    00000010h
PD_PRINTTOFILE           =    00000020h
PD_PRINTSETUP            =    00000040h
PD_NOWARNING             =    00000080h
PD_RETURNDC              =    00000100h
PD_RETURNIC              =    00000200h
PD_RETURNDEFAULT         =    00000400h
PD_SHOWHELP              =    00000800h
PD_ENABLEPRINTHOOK       =    00001000h
PD_ENABLESETUPHOOK       =    00002000h
PD_ENABLEPRINTTEMPLATE   =    00004000h
PD_ENABLESETUPTEMPLATE   =    00008000h
PD_ENABLEPRINTTEMPLATEHANDLE = 00010000h
PD_ENABLESETUPTEMPLATEHANDLE  = 00020000h
PD_USEDEVMODECOPIES         = 00040000h
PD_USEDEVMODECOPIESANDCOLLATE = 00040000h
PD_DISABLEPRINTTOFILE     =   00080000h
PD_HIDEPRINTTOFILE        =   00100000h
PD_NONETWORKBUTTON        =   00200000h

DOCINFO struc
    cbSize UINT ?
    lpszDocName ULONG ?
    lpszOutput ULONG ?
    lpszDatatype ULONG ?
    fwType ULONG ?
DOCINFO ends


GW_HWNDFIRST      =  0
GW_HWNDLAST       =  1
GW_HWNDNEXT       =  2
GW_HWNDPREV       =  3
GW_OWNER          =  4
GW_CHILD          =  5
GW_MAX            =  5

;*******************************************************************
;
;       SYSTEMTIME
;
;*******************************************************************
SYSTEMTIME struc
    wYear USHORT ?
    wMonth USHORT ?
    wDayOfWeek USHORT ?
    wDay USHORT ?
    wHour USHORT ?
    wMinute USHORT ?
    wSecond USHORT ?
    wMilliseconds USHORT ?
SYSTEMTIME ends
;*******************************************************************
;
;       TBBUTTON
;
;*******************************************************************
TBBUTTON struc
    iBitmap UINT ?
    idCommand UINT ?
    fsState UCHAR ?
    fsStyle UCHAR ?
    bReserved db 2 dup(?)
    dwData ULONG ?
    iString UINT ?
TBBUTTON ends

STD_CUT             =    0
STD_COPY            =    1
STD_PASTE           =    2
STD_UNDO            =    3
STD_REDOW           =    4
STD_DELETE          =    5
STD_FILENEW         =    6
STD_FILEOPEN        =    7
STD_FILESAVE        =    8
STD_PRINTPRE        =    9
STD_PROPERTIES      =    10
STD_HELP            =    11
STD_FIND            =    12
STD_REPLACE         =    13
STD_PRINT           =    14

TBSTATE_CHECKED       =  01h
TBSTATE_PRESSED       =  02h
TBSTATE_ENABLED       =  04h
TBSTATE_HIDDEN        =  08h
TBSTATE_INDETERMINATE =  10h
TBSTATE_WRAP          =  20h

TBSTYLE_BUTTON        =  00h
TBSTYLE_SEP           =  01h
TBSTYLE_CHECK         =  02h
TBSTYLE_GROUP         =  04h
TBSTYLE_CHECKGROUP    =  TBSTYLE_GROUP+TBSTYLE_CHECK

TBSTYLE_TOOLTIPS      =  0100h
TBSTYLE_WRAPABLE      =  0200h
TBSTYLE_ALTDRAG       =  0400h

TB_ENABLEBUTTON       =  (WM_USER + 1)
TB_CHECKBUTTON        =  (WM_USER + 2)
TB_PRESSBUTTON        =  (WM_USER + 3)
TB_HIDEBUTTON         =  (WM_USER + 4)
TB_INDETERMINATE      =  (WM_USER + 5)
TB_ISBUTTONENABLED    =  (WM_USER + 9)
TB_ISBUTTONCHECKED    =  (WM_USER + 10)
TB_ISBUTTONPRESSED    =  (WM_USER + 11)
TB_ISBUTTONHIDDEN     =  (WM_USER + 12)
TB_ISBUTTONINDETERMINATE = (WM_USER + 13)
TB_SETSTATE           =  (WM_USER + 17)
TB_GETSTATE           =  (WM_USER + 18)
TB_ADDBITMAP          =  (WM_USER + 19)
TB_SAVERESTOREA       =  (WM_USER + 26)
TB_SAVERESTOREW       =  (WM_USER + 76)
TB_CUSTOMIZE          =  (WM_USER + 27)
TB_ADDSTRINGA         =  (WM_USER + 28)
TB_ADDSTRINGW         =  (WM_USER + 77)
TB_GETITEMRECT        =  (WM_USER + 29)
TB_BUTTONSTRUCTSIZE   =  (WM_USER + 30)
TB_SETBUTTONSIZE      =  (WM_USER + 31)
TB_SETBITMAPSIZE      =  (WM_USER + 32)
TB_AUTOSIZE           =  (WM_USER + 33)
TB_GETTOOLTIPS        =  (WM_USER + 35)
TB_SETTOOLTIPS        =  (WM_USER + 36)
TB_SETPARENT          =  (WM_USER + 37)
TB_SETROWS            =  (WM_USER + 39)
TB_GETROWS            =  (WM_USER + 40)
TB_SETCMDID           =  (WM_USER + 42)
TB_CHANGEBITMAP       =  (WM_USER + 43)
TB_GETBITMAP          =  (WM_USER + 44)
TB_GETBUTTONTEXTA     =  (WM_USER + 45)
TB_GETBUTTONTEXTW     =  (WM_USER + 75)
TB_REPLACEBITMAP      =  (WM_USER + 46)

;====== COMMON CONTROL STYLES =====

CCS_TOP            =     00000001h
CCS_NOMOVEY        =     00000002h
CCS_BOTTOM         =     00000003h
CCS_NORESIZE       =     00000004h
CCS_NOPARENTALIGN  =     00000008h
CCS_ADJUSTABLE     =     00000020h
CCS_NODIVIDER      =     00000040h


; Define Transparent

TRANSPARENT   = 1


;*******************************************************************
;
;       NMHDR
;
;*******************************************************************
NMHDR struc
    hwndFrom UINT ?
    idFrom UINT ?
    code UINT ?
NMHDR ends

;*******************************************************************
;
;       TOOLTIPTEXT
;
;*******************************************************************
TOOLTIPTEXT struc
    hdr NMHDR <?>
    lpszText ULONG ?
    szText db 80 dup(?)
    hinst ULONG ?
    uFlags UINT ?
TOOLTIPTEXT ends

TTN_NEEDTEXT equ 0FFFFFDF8h
;*******************************************************************
;
;       TEXTMETRIC
;
;*******************************************************************
TEXTMETRIC struc
    tmHeight ULONG ?
    tmAscent ULONG ?
    tmDescent ULONG ?
    tmInternalLeading ULONG ?
    tmExternalLeading ULONG ?
    tmAveCharWidth ULONG ?
    tmMaxCharWidth ULONG ?
    tmWeight ULONG ?
    tmOverhang ULONG ?
    tmDigitizedAspectX ULONG ?
    tmDigitizedAspectY ULONG ?
    tmFirstChar UCHAR ?
    tmLastChar UCHAR ?
    tmDefaultChar UCHAR ?
    tmBreakChar UCHAR ?
    tmItalic UCHAR ?
    tmUnderlined UCHAR ?
    tmStruckOut UCHAR ?
    tmPitchAndFamily UCHAR ?
    tmCharSet UCHAR ?
TEXTMETRIC ends
;*******************************************************************
;
;       Rectangle
;
;*******************************************************************

RECT    struc
        rcLeft          UINT ?
        rcTop           UINT ?
        rcRight         UINT ?
        rcBottom        UINT ?
RECT    ends

;*******************************************************************
;
;  Window Class structure
;
;*******************************************************************

WNDCLASS struc
        clsStyle          UINT     ?
        clsLpfnWndProc    ULONG    ?
        clsCbClsExtra     UINT     ?
        clsCbWndExtra     UINT     ?
        clsHInstance      UINT     ?
        clsHIcon          UINT     ?
        clsHCursor        UINT     ?
        clsHbrBackground  UINT     ?
        clsLpszMenuName   ULONG    ?
        clsLpszClassName  ULONG    ?
        hIconSm           UINT     ?
WNDCLASS ends

 POINT STRUC
    x  DWORD ?
    y  DWORD ?
 POINT ENDS

 WINDOWPLACEMENT STRUC
    length           UINT    ?
    flags            UINT    ?
    showCmd          UINT    ?
    ptMinPosition    POINT  <?>
    pMaxPosition     POINT  <?>
    rcNormalPosition RECT   <?>
 WINDOWPLACEMENT ENDS


PAINTSTRUCT STRUC
    PShdc         UINT             ?
    PSfErase      UINT             ?
    PSrcPaint     UCHAR            size RECT dup(?)
    PSfRestore    UINT             ?
    PSfIncUpdate  UINT             ?
    PSrgbReserved UCHAR            32 dup(?)
PAINTSTRUCT ENDS

MSGSTRUCT struc
    msHWND          UINT    ?
    msMESSAGE       UINT    ?
    msWPARAM        UINT    ?
    msLPARAM        ULONG   ?
    msTIME          ULONG   ?
    msPT            ULONG   2 dup(?)
MSGSTRUCT ends

; Shell Data
NOTIFYICONDATAA STRUC
    cbSize            DWORD ?
    hWnd              DWORD ?
    uID               UINT  ?
    uFlags2           UINT  ? 
    uCallbackMessage  UINT  ? 
    hIcon             HICON ?
    szTip             BYTE  64 DUP(?)
NOTIFYICONDATAA ENDS






MINMAXINFO struc
  res_x               dd ?
  res_y               dd ?
  maxsize_x           dd ?
  maxsize_y           dd ?
  maxposition_x       dd ?
  maxposition_y       dd ?
  mintrackposition_x  dd ?
  mintrackposition_y  dd ?
  maxtrackposition_x  dd ?
  maxtrackposition_y  dd ?
MINMAXINFO ends



; Shell Stuff
NIM_ADD       =  0
NIM_MODIFY    =  1
NIM_DELETE    =  2

NIF_MESSAGE   =  1
NIF_ICON      =  2
NIF_TIP       =  4



;
;   System Metrics
;
SM_CXSCREEN         =    0
SM_CYSCREEN         =    1
SM_CXVSCROLL        =    2
SM_CYHSCROLL        =    3
SM_CYCAPTION        =    4
SM_CXBORDER         =    5
SM_CYBORDER         =    6
SM_CXDLGFRAME       =    7
SM_CYDLGFRAME       =    8
SM_CYVTHUMB         =    9
SM_CXHTHUMB         =    10
SM_CXICON           =    11
SM_CYICON           =    12
SM_CXCURSOR         =    13
SM_CYCURSOR         =    14
SM_CYMENU           =    15
SM_CXFULLSCREEN     =    16
SM_CYFULLSCREEN     =    17
SM_CYKANJIWINDOW    =    18
SM_MOUSEPRESENT     =    19
SM_CYVSCROLL        =    20
SM_CXHSCROLL        =    21
SM_DEBUG            =    22
SM_SWAPBUTTON       =    23
SM_RESERVED1        =    24
SM_RESERVED2        =    25
SM_RESERVED3        =    26
SM_RESERVED4        =    27
SM_CXMIN            =    28
SM_CYMIN            =    29
SM_CXSIZE           =    30
SM_CYSIZE           =    31
SM_CXFRAME          =    32
SM_CYFRAME          =    33
SM_CXMINTRACK       =    34
SM_CYMINTRACK       =    35
SM_CXDOUBLECLK      =    36
SM_CYDOUBLECLK      =    37
SM_CXICONSPACING    =    38
SM_CYICONSPACING    =    39
SM_MENUDROPALIGNMENT =    40
SM_PENWINDOWS       =    41
SM_DBCSENABLED      =    42
SM_CMOUSEBUTTONS    =    43
SM_CXFIXEDFRAME     equ      <SM_CXDLGFRAME>
SM_CYFIXEDFRAME     equ      <SM_CYDLGFRAME>
SM_CXSIZEFRAME      equ      <SM_CXFRAME>
SM_CYSIZEFRAME      equ      <SM_CYFRAME>
SM_SECURE           =    44
SM_CXEDGE           =    45
SM_CYEDGE           =    46
SM_CXMINSPACING     =    47
SM_CYMINSPACING     =    48
SM_CXSMICON         =    49
SM_CYSMICON         =    50
SM_CYSMCAPTION      =    51
SM_CXSMSIZE         =    52
SM_CYSMSIZE         =    53
SM_CXMENUSIZE       =    54
SM_CYMENUSIZE       =    55
SM_ARRANGE          =    56
SM_CXMINIMIZED      =    57
SM_CYMINIMIZED      =    58
SM_CXMAXTRACK       =    59
SM_CYMAXTRACK       =    60
SM_CXMAXIMIZED      =    61
SM_CYMAXIMIZED      =    62
SM_NETWORK          =    63
SM_CLEANBOOT        =    67
SM_CXDRAG           =    68
SM_CYDRAG           =    69
SM_SHOWSOUNDS       =    70
SM_CXMENUCHECK      =    71
SM_CYMENUCHECK      =    72
SM_SLOWMACHINE      =    73
SM_MIDEASTENABLED   =    74
SM_CMETRICS         =    75
;
;  Stock Logical Objects
;
WHITE_BRUSH         =  0
LTGRAY_BRUSH        =  1
GRAY_BRUSH          =  2
DKGRAY_BRUSH        =  3
BLACK_BRUSH         =  4
NULL_BRUSH          =  5
HOLLOW_BRUSH        =  5
WHITE_PEN           =  6
BLACK_PEN           =  7
NULL_PEN            =  8
DOT_MARKER          =  9
OEM_FIXED_FONT      = 10
ANSI_FIXED_FONT     = 11
ANSI_VAR_FONT       = 12
SYSTEM_FONT         = 13
DEVICE_DEFAULT_FONT = 14
DEFAULT_PALETTE     = 15
SYSTEM_FIXED_FONT   = 16

;
; Brush Styles
;
BS_SOLID        =   0
BS_NULL         =   1
BS_HOLLOW       =   BS_NULL
BS_HATCHED      =   2
BS_PATTERN      =   3
BS_INDEXED      =   4
BS_DIBPATTERN   =   5
;
; Hatch Styles
;
HS_HORIZONTAL   =   0       ; -----
HS_VERTICAL     =   1       ; |||||
HS_FDIAGONAL    =   2       ; \\\\\
HS_BDIAGONAL    =   3       ; /////
HS_CROSS        =   4       ; +++++
HS_DIAGCROSS    =   5       ; xxxxx
;
; Pen Styles
;
PS_SOLID        =   0
PS_DASH         =   1       ; -------
PS_DOT          =   2       ; .......
PS_DASHDOT      =   3       ; _._._._
PS_DASHDOTDOT   =   4       ; _.._.._
PS_NULL         =   5
PS_INSIDEFRAME  =   6

;
;  Window State Messages
;
IFNDEF  NOWM
WM_STATE            = 0000H

WM_NULL             = 0000h
WM_CREATE           = 0001h
WM_DESTROY          = 0002h
WM_MOVE             = 0003h
WM_SIZE             = 0005h
WM_ACTIVATE         = 0006h
WM_SETFOCUS         = 0007h
WM_KILLFOCUS        = 0008h
WM_ENABLE           = 000Ah
WM_SETREDRAW        = 000Bh
WM_SETTEXT          = 000Ch
WM_GETTEXT          = 000Dh
WM_GETTEXTLENGTH    = 000Eh
WM_PAINT            = 000Fh
WM_CLOSE            = 0010h
WM_QUERYENDSESSION  = 0011h
WM_QUIT             = 0012h
WM_QUERYOPEN        = 0013h
WM_ERASEBKGND       = 0014h
WM_SYSCOLORCHANGE   = 0015h
WM_ENDSESSION       = 0016h
WM_SYSTEMERROR      = 0017h
WM_SHOWWINDOW       = 0018h
WM_CTLCOLOR         = 0019h
WM_WININICHANGE     = 001Ah
WM_DEVMODECHANGE    = 001Bh
WM_ACTIVATEAPP      = 001Ch
WM_FONTCHANGE       = 001Dh
WM_TIMECHANGE       = 001Eh
WM_CANCELMODE       = 001Fh
WM_SETCURSOR        = 0020h
WM_MOUSEACTIVATE    = 0021h
WM_CHILDACTIVATE    = 0022h
WM_QUEUESYNC        = 0023h
WM_GETMINMAXINFO    = 0024h
WM_PAINTICON        = 0026h
WM_ICONERASEBKGND   = 0027h
WM_NEXTDLGCTL       = 0028h
WM_SPOOLERSTATUS    = 002Ah
WM_DRAWITEM         = 002Bh
WM_MEASUREITEM      = 002Ch
WM_DELETEITEM       = 002Dh
WM_VKEYTOITEM       = 002Eh
WM_CHARTOITEM       = 002Fh
WM_SETFONT          = 0030h
WM_GETFONT          = 0031h
WM_QUERYDRAGICON    = 0037h
WM_COMPAREITEM      = 0039h
WM_COMPACTING       = 0041h
WM_COMMNOTIFY       = 0044h
WM_WINDOWPOSCHANGING= 0046h
WM_WINDOWPOSCHANGED = 0047h
WM_POWER            = 0048h
WM_NOTIFY           = 004eh

WM_NCCREATE         = 0081h
WM_NCDESTROY        = 0082h
WM_NCCALCSIZE       = 0083h
WM_NCHITTEST        = 0084h
WM_NCPAINT          = 0085h
WM_NCACTIVATE       = 0086h
WM_GETDLGCODE       = 0087h
WM_NCMOUSEMOVE      = 00A0h
WM_NCLBUTTONDOWN    = 00A1h
WM_NCLBUTTONUP      = 00A2h
WM_NCLBUTTONDBLCLK  = 00A3h
WM_NCRBUTTONDOWN    = 00A4h
WM_NCRBUTTONUP      = 00A5h
WM_NCRBUTTONDBLCLK  = 00A6h
WM_NCMBUTTONDOWN    = 00A7h
WM_NCMBUTTONUP      = 00A8h
WM_NCMBUTTONDBLCLK  = 00A9h

WM_KEYFIRST         = 0100h
WM_KEYDOWN          = 0100h
WM_KEYUP            = 0101h
WM_CHAR             = 0102h
WM_DEADCHAR         = 0103h
WM_SYSKEYDOWN       = 0104h
WM_SYSKEYUP         = 0105h
WM_SYSCHAR          = 0106h
WM_SYSDEADCHAR      = 0107h
WM_KEYLAST          = 0108h
WM_HOTKEY           = 0312h
WM_INITDIALOG       = 0110h
WM_COMMAND          = 0111h
WM_SYSCOMMAND       = 0112h
WM_TIMER            = 0113h
WM_HSCROLL          = 0114h
WM_VSCROLL          = 0115h
WM_INITMENU         = 0116h
WM_INITMENUPOPUP    = 0117h
WM_MENUSELECT       = 011Fh
WM_MENUCHAR         = 0120h
WM_ENTERIDLE        = 0121h


WM_MOUSEFIRST       = 0200h
WM_MOUSEMOVE        = 0200h
WM_LBUTTONDOWN      = 0201h
WM_LBUTTONUP        = 0202h
WM_LBUTTONDBLCLK    = 0203h
WM_RBUTTONDOWN      = 0204h
WM_RBUTTONUP        = 0205h
WM_RBUTTONDBLCLK    = 0206h
WM_MBUTTONDOWN      = 0207h
WM_MBUTTONUP        = 0208h
WM_MBUTTONDBLCLK    = 0209h
WM_MOUSELAST        = 0209h

WM_PARENTNOTIFY     = 0210h
WM_MDICREATE        = 0220h
WM_MDIDESTROY       = 0221h
WM_MDIACTIVATE      = 0222h
WM_MDIRESTORE       = 0223h
WM_MDINEXT          = 0224h
WM_MDIMAXIMIZE      = 0225h
WM_MDITILE          = 0226h
WM_MDICASCADE       = 0227h
WM_MDIICONARRANGE   = 0228h
WM_MDIGETACTIVE     = 0229h
WM_MDISETMENU       = 0230h
WM_DROPFILES        = 0233h


WM_CUT              = 0300h
WM_COPY             = 0301h
WM_PASTE            = 0302h
WM_CLEAR            = 0303h
WM_UNDO             = 0304h
WM_RENDERFORMAT     = 0305h
WM_RENDERALLFORMATS = 0306h
WM_DESTROYCLIPBOARD = 0307h
WM_DRAWCLIPBOARD    = 0308h
WM_PAINTCLIPBOARD   = 0309h
WM_VSCROLLCLIPBOARD = 030Ah
WM_SIZECLIPBOARD    = 030Bh
WM_ASKCBFORMATNAME  = 030Ch
WM_CHANGECBCHAIN    = 030Dh
WM_HSCROLLCLIPBOARD = 030Eh
WM_QUERYNEWPALETTE  = 030Fh
WM_PALETTEISCHANGING = 0310h
WM_PALETTECHANGED   = 0311h

WM_PENWINFIRST      equ 0380h
WM_PENWINLAST       equ 038Fh

WM_COALESCE_FIRST  equ 0390h
WM_COALESCE_LAST   equ 039Fh

;  private window messages start here
WM_USER             = 0400H
ENDIF           ; NOWM

; WM_MOUSEACTIVATE Return Codes
MA_ACTIVATE       =  1
MA_ACTIVATEANDEAT =  2
MA_NOACTIVATE     =  3

; Size message commands
SIZENORMAL       = 0
SIZEICONIC       = 1
SIZEFULLSCREEN   = 2
SIZEZOOMSHOW     = 3
SIZEZOOMHIDE     = 4

; ShowWindow() Commands
SW_HIDE            = 0
SW_SHOWNORMAL      = 1
SW_NORMAL          = 1
SW_SHOWMINIMIZED   = 2
SW_SHOWMAXIMIZED   = 3
SW_MAXIMIZE        = 3
SW_SHOWNOACTIVATE  = 4
SW_SHOW            = 5
SW_MINIMIZE        = 6
SW_SHOWMINNOACTIVE = 7
SW_SHOWNA          = 8
SW_RESTORE         = 9

; Old ShowWindow() Commands
HIDE_WINDOW        = 0
SHOW_OPENWINDOW    = 1
SHOW_ICONWINDOW    = 2
SHOW_FULLSCREEN    = 3
SHOW_OPENNOACTIVATE= 4

;  identifiers for the WM_SHOWWINDOW message
SW_PARENTCLOSING =  1
SW_OTHERZOOM     =  2
SW_PARENTOPENING =  3
SW_OTHERUNZOOM   =  4
;
; Key state masks for mouse messages
;
MK_LBUTTON       = 0001h
MK_RBUTTON       = 0002h
MK_SHIFT         = 0004h
MK_CONTROL       = 0008h
MK_MBUTTON       = 0010h
;
; Virutal Keys
;
VK_RIGHT    =   027h    
VK_LEFT     =   025h


;
; Class styles
;
CS_VREDRAW         = 0001h
CS_HREDRAW         = 0002h
CS_KEYCVTWINDOW    = 0004H
CS_DBLCLKS         = 0008h
SBS_SIZEGRIP       = 0010h
CS_OWNDC           = 0020h
CS_CLASSDC         = 0040h
CS_PARENTDC        = 0080h
CS_NOKEYCVT        = 0100h
CS_SAVEBITS        = 0800h
CS_NOCLOSE         = 0200h
CS_BYTEALIGNCLIENT = 1000h
CS_BYTEALIGNWINDOW = 2000h
CS_GLOBALCLASS     = 4000h    ; Global window class

;
; Special CreateWindow position value
;
CW_USEDEFAULT   EQU    8000h

;
; Windows styles
;
WS_OVERLAPPED   = 000000000h
WS_ICONICPOPUP  = 0C0000000h
WS_POPUP        = 080000000h
WS_CHILD        = 040000000h
WS_MINIMIZE     = 020000000h
WS_VISIBLE      = 010000000h
WS_DISABLED     = 008000000h
WS_CLIPSIBLINGS = 004000000h
WS_CLIPCHILDREN = 002000000h
WS_MAXIMIZE     = 001000000h
WS_CAPTION      = 000C00000h     ; WS_BORDER | WS_DLGFRAME
WS_BORDER       = 000800000h
WS_DLGFRAME     = 000400000h
WS_VSCROLL      = 000200000h
WS_HSCROLL      = 000100000h
WS_SYSMENU      = 000080000h
WS_THICKFRAME   = 000040000h
WS_HREDRAW      = 000020000h
WS_VREDRAW      = 000010000h
WS_GROUP        = 000020000h
WS_TABSTOP      = 000010000h
WS_MINIMIZEBOX  = 000020000h
WS_MAXIMIZEBOX  = 000010000h

; Common Window Styles

WS_OVERLAPPEDWINDOW = WS_OVERLAPPED OR WS_CAPTION OR WS_SYSMENU OR WS_THICKFRAME OR WS_MINIMIZEBOX OR WS_MAXIMIZEBOX
WS_POPUPWINDOW  = WS_POPUP OR WS_BORDER OR WS_SYSMENU
WS_CHILDWINDOW  = WS_CHILD
WS_TILEDWINDOW  = WS_OVERLAPPEDWINDOW

WS_TILED        = WS_OVERLAPPED
WS_ICONIC       = WS_MINIMIZE
WS_SIZEBOX      = WS_THICKFRAME

; Extended Window Styles (low words)
WS_EX_DLGMODALFRAME  = 0001
WS_EX_DRAGOBJECT     = 0002
WS_EX_NOPARENTNOTIFY = 0004
WS_EX_TOPMOST        = 0008


; PeekMessage() Options
PM_NOREMOVE    = 0000h
PM_REMOVE      = 0001h
PM_NOYIELD     = 0002h


; StretchBlt
SRCCOPY       = 0CC0020h


; SetWindowPos Flags
SWP_NOSIZE       =  0001h
SWP_NOMOVE       =  0002h
SWP_NOZORDER     =  0004h
SWP_NOREDRAW     =  0008h
SWP_NOACTIVATE   =  0010h
SWP_DRAWFRAME    =  0020h
SWP_SHOWWINDOW   =  0040h
SWP_HIDEWINDOW   =  0080h
SWP_NOCOPYBITS   =  0100h
SWP_NOREPOSITION =  0200h
;
;  Predefined cursor & icon IDs
;
IDC_ARROW       = 32512
IDC_IBEAM       = 32513
IDC_WAIT        = 32514
IDC_CROSS       = 32515
IDC_UPARROW     = 32516
IDC_SIZE        = 32640
IDC_ICON        = 32641
IDC_SIZENWSE    = 32642
IDC_SIZENESW    = 32643
IDC_SIZEWE      = 32644
IDC_SIZENS      = 32645

IDI_APPLICATION = 32512
IDI_HAND        = 32513
IDI_QUESTION    = 32514
IDI_EXCLAMATION = 32515
IDI_ASTERISK    = 32516

COLOR_SCROLLBAR           = 0
COLOR_BACKGROUND          = 1
COLOR_ACTIVECAPTION       = 2
COLOR_INACTIVECAPTION     = 3
COLOR_MENU                = 4
COLOR_WINDOW              = 5
COLOR_WINDOWFRAME         = 6
COLOR_MENUTEXT            = 7
COLOR_WINDOWTEXT          = 8
COLOR_CAPTIONTEXT         = 9
COLOR_ACTIVEBORDER        = 10
COLOR_INACTIVEBORDER      = 11
COLOR_APPWORKSPACE        = 12
COLOR_HIGHLIGHT           = 13
COLOR_HIGHLIGHTTEXT       = 14
COLOR_BTNFACE             = 15
COLOR_BTNSHADOW           = 16
COLOR_GRAYTEXT            = 17
COLOR_BTNTEXT             = 18

;
;  MessageBox type flags
;
MB_OK                   = 0000H
MB_OKCANCEL             = 0001H
MB_ABORTRETRYIGNORE     = 0002H
MB_YESNOCANCEL          = 0003H
MB_YESNO                = 0004H
MB_RETRYCANCEL          = 0005H

MB_ICONHAND             = 0010H
MB_ICONQUESTION         = 0020H
MB_ICONEXCLAMATION      = 0030H
MB_ICONASTERISK         = 0040H

MB_DEFBUTTON1           = 0000H
MB_DEFBUTTON2           = 0100H
MB_DEFBUTTON3           = 0200H

MB_APPLMODAL            = 0000H
MB_SYSTEMMODAL          = 1000H
MB_TASKMODAL            = 2000H

MB_NOFOCUS              = 8000H

;
;  Conventional dialog box and message box command IDs
;
IDOK     =   1
IDCANCEL =   2
IDABORT  =   3
IDRETRY  =   4
IDIGNORE =   5
IDYES    =   6
IDNO     =   7


;
;  Edit Control Messages
;
EM_GETSEL             =  00B0h
EM_SETSEL             =  00B1h
EM_GETRECT            =  00B2h
EM_SETRECT            =  00B3h
EM_SETRECTNP          =  00B4h
EM_SCROLL             =  00B5h
EM_LINESCROLL         =  00B6h
EM_SCROLLCARET        =  00B7h
EM_GETMODIFY          =  00B8h
EM_SETMODIFY          =  00B9h
EM_GETLINECOUNT       =  00BAh
EM_LINEINDEX          =  00BBh
EM_SETHANDLE          =  00BCh
EM_GETHANDLE          =  00BDh
EM_GETTHUMB           =  00BEh
EM_LINELENGTH         =  00C1h
EM_REPLACESEL         =  00C2h
EM_GETLINE            =  00C4h
EM_LIMITTEXT          =  00C5h
EM_CANUNDO            =  00C6h
EM_UNDO               =  00C7h
EM_FMTLINES           =  00C8h
EM_LINEFROMCHAR       =  00C9h
EM_SETTABSTOPS        =  00CBh
EM_SETPASSWORDCHAR    =  00CCh
EM_EMPTYUNDOBUFFER    =  00CDh
EM_GETFIRSTVISIBLELINE=  00CEh
EM_SETREADONLY        =  00CFh
EM_SETWORDBREAKPROC   =  00D0h
EM_GETWORDBREAKPROC   =  00D1h
EM_GETPASSWORDCHAR    =  00D2h
EM_SETMARGINS         =  00D3h
EM_GETMARGINS         =  00D4h
EM_SETLIMITTEXT       equ  <EM_LIMITTEXT>
EM_GETLIMITTEXT       =  00D5h
EM_POSFROMCHAR        =  00D6h
EM_CHARFROMPOS        =  00D7h
