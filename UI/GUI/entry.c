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
 

/*******************************************************************************
 * WinMain                                                                     *
 *                                                                             *
 * DESCRIPTION: App Entry Point                                                *
 *                                                                             *
 * INPUT                                                                       *
 *   Standard Win32 Entry                                                      *
 *                                                                             *
 * OUTPUT                                                                      * 
 *   0                                                                         *
 *                                                                             *
 *******************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevhInstance, PCHAR lpCmd, int nShow)
{
    int RetValue = 0;
    HWND hWnd;
    ATOM WndClass;
    
    
    if(WndClass = MDUI_CreateMdeskClass(hInstance))
    {
        if(hWnd = MDUI_CreateMdeskWindow(hInstance, WndClass))
        {
                RetValue = MDUI_MessageLoop(hWnd);
        }
        
        MDUI_UnRegisterUIClass(hInstance, WndClass);
    }
    
    return RetValue;
}


 
