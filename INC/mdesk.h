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


#ifndef __MDESK_H__
#define __MDESK_H__


#ifdef __cplusplus
extern "C" {
#endif



typedef PVOID HMDESK;

#define INVALID_MDESK_HANDLE (HMDESK)NULL

#define MDESK_SAFEMODE_FLAG   0x00000001

HMDESK WINAPI MDesk_CreateDesktops(DWORD dwNumber, DWORD dwFlags);
void WINAPI MDesk_SetFlags(HMDESK hMdesk, DWORD dwFlags);
DWORD WINAPI MDesk_GetFlags(HMDESK hMdesk);
DWORD WINAPI MDesk_GetCurrentDesktop(HMDESK hMdesk);
DWORD WINAPI MDesk_GetNumberOfDesktops(HMDESK hMdesk);
void WINAPI MDesk_GoToDesktop(HMDESK hMdesk, DWORD dwDesktop);
void WINAPI MDesk_SwitchDesktopForward(HMDESK hMdesk);
void WINAPI MDesk_SwitchDesktopBackward(HMDESK hMdesk);
void WINAPI MDesk_DestroyDesktop(HMDESK hMdesk);
void WINAPI MDesk_DestroyAllUniversalWindows(HMDESK hMdesk);
BOOL WINAPI MDesk_AddUniversalWindow(HMDESK hMdesk, HWND hWnd);
void WINAPI MDesk_RemoveUniversalWindow(HMDESK hMdesk, HWND hWnd);


#ifdef __cplusplus 
}
#endif


#endif

