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
 
 
#ifndef __MDUI_H__
#define __MDUI_H__


#ifdef __cplusplus
extern "C" {
#endif




typedef struct __mdeskui__struct__ {
        
        HINSTANCE hInstance;
        HMDESK hMdesk;
        HMENU  hPopupMenu;
        DWORD  dwCurrentCheck;
        
} MDUISTRUC, *PMDUISTRUC;

void WINAPI MDUI_UnRegisterUIClass(HINSTANCE hInstance, ATOM ClassAtom);
HWND WINAPI MDUI_CreateMdeskWindow(HINSTANCE hInstance, ATOM WinClass);
int WINAPI MDUI_MessageLoop(HWND hWnd);
ATOM WINAPI MDUI_CreateMdeskClass(HINSTANCE hInstance);




#ifdef __cplusplus
}
#endif





#endif

