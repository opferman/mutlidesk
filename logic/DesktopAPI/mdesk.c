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




/***************************************************************************** 
 * Internal Data Structure                                                   *
 *****************************************************************************/
/* Application window Structure For Each Window */
typedef struct _application_window_ {
        
        HWND   hWnd;
        DWORD  dwDesktop;
        BOOL   bFocus;
        struct _application_window_ *pNext, *pBefore;
        
} APPWINDOW, *PAPPWINDOW;

/* Structure of Desktop Handle */
typedef struct _mutliple_desktop_structure_ {
   
    DWORD  dwNumberOfDesktops;
    DWORD  dwFlags;
    DWORD  dwCurrentDesktop;
    
    HWND   hProgman;
    HWND   hTaskbar;
    
    PAPPWINDOW pAppWindows;
    PAPPWINDOW pUniversalWindows;
    
    struct _mutliple_desktop_structure_ *pNext, *pBefore;        
    
} MDESK_INTERNAL, *PMDESK_INTERNAL;


/***************************************************************************** 
 * Internal Constants                                                        *
 *****************************************************************************/
const PCHAR gc_TaskBarClassName = "Shell_TrayWnd";
const PCHAR gc_ProgMan          = "Progman";

/***************************************************************************** 
 * Internal Macros                                                           *
 *****************************************************************************/
#define MDESK_DESKTOP_INC(x) ((x->dwCurrentDesktop == (x->dwNumberOfDesktops - 1)) ? 0 : (x->dwCurrentDesktop + 1))
#define MDESK_DESKTOP_DEC(x) ((x->dwCurrentDesktop == 0) ? (x->dwNumberOfDesktops - 1) : (x->dwCurrentDesktop - 1))

/***************************************************************************** 
 * Internal Prototypes                                                       *
 *****************************************************************************/
void MDesk_SwitchDesktopsWithSafeMode(PMDESK_INTERNAL pMdesk, DWORD dwDesktop);
void MDesk_SwitchDesktopsRegularMode(PMDESK_INTERNAL pMdesk, DWORD dwDesktop);
BOOL MDesk_SaveWindow(PMDESK_INTERNAL pMdesk, HWND hWnd);
BOOL MDesk_IsUniversalWindow(PMDESK_INTERNAL pMdesk, HWND hWnd);
BOOL CALLBACK MDesk_SaveDesktopRegularMode(HWND hWnd, LPARAM lParam);
BOOL CALLBACK MDesk_SaveDesktopSafeMode(HWND hWnd, LPARAM lParam);
void MDesk_RestoreDesktopRegularMode(PMDESK_INTERNAL pMdesk);
void MDesk_RestoreDesktopSafeMode(PMDESK_INTERNAL pMdesk);
void MDesk_RestoreAllDesktopsSafeMode(PMDESK_INTERNAL pMdesk);
void MDesk_RestoreAllDesktopsRegularMode(PMDESK_INTERNAL pMdesk);





/*******************************************************************************
 * DllMain                                                                     *
 *                                                                             *
 * DESCRIPTION: DLL Entry Point                                                *
 *                                                                             *
 * INPUT                                                                       *
 *   Standard Win32 DLL Entry                                                  *
 *                                                                             *
 * OUTPUT                                                                      * 
 *   TRUE = Initialized, FALSE = Failure                                       *
 *                                                                             *
 *******************************************************************************/
BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    BOOL bRetValue = TRUE;

    switch(dwReason)
    {
       case DLL_PROCESS_ATTACH:
            break;

       case DLL_PROCESS_DETACH:
            break;

    }

    return bRetValue;
}

/*****************************************************************************
 * MDesk_CreateDesktops
 *
 *   Initialize Desktop API
 *
 * INPUT
 *   Number of Desktops, Flags
 * OUTPUT
 *   Handle to Desktop
 *****************************************************************************/
HMDESK WINAPI MDesk_CreateDesktops(DWORD dwNumber, DWORD dwFlags)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)INVALID_MDESK_HANDLE;
   
   if(dwNumber > 1 && dwNumber < 256)
   {
     if(pMdesk = (PMDESK_INTERNAL)LocalAlloc(LMEM_ZEROINIT, sizeof(MDESK_INTERNAL)))
     {
          pMdesk->dwNumberOfDesktops = dwNumber;
          pMdesk->dwFlags            = dwFlags;
          pMdesk->hProgman           = FindWindow(gc_ProgMan, NULL);
          pMdesk->hTaskbar           = FindWindow(gc_TaskBarClassName, NULL);
     }
     else
       SetLastError(ERROR_NOT_ENOUGH_MEMORY);
   }
   else
     SetLastError(ERROR_INVALID_PARAMETER);
   
   return (HMDESK)pMdesk;     
}



/*****************************************************************************
 * MDesk_SetFlags
 *
 *   Set the desktop Flags
 *
 * INPUT
 *   Handle to desktop, Flags
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_SetFlags(HMDESK hMdesk, DWORD dwFlags)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   pMdesk->dwFlags    = dwFlags;
}


/*****************************************************************************
 * MDesk_GetFlags
 *
 *   Get the desktop flags
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   Flags
 *****************************************************************************/
DWORD WINAPI MDesk_GetFlags(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   return pMdesk->dwFlags;
}

/*****************************************************************************
 * MDesk_GetCurrentDesktop
 *
 *   Get the current desktop
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   current desktop
 *****************************************************************************/
DWORD WINAPI MDesk_GetCurrentDesktop(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   return pMdesk->dwCurrentDesktop;
}


/*****************************************************************************
 * MDesk_GetCurrentDesktop
 *
 *   Get the number of desktops
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   Number of desktops
 *****************************************************************************/
DWORD WINAPI MDesk_GetNumberOfDesktops(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   return pMdesk->dwNumberOfDesktops;
}


/*****************************************************************************
 * MDesk_GoToDesktop
 *
 *   Switches To A particular Desktop
 *
 * INPUT
 *   Handle to desktop, Desktop Number
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_GoToDesktop(HMDESK hMdesk, DWORD dwDesktop)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   if(dwDesktop != pMdesk->dwCurrentDesktop && dwDesktop < pMdesk->dwNumberOfDesktops)
   {
      if(pMdesk->dwFlags & MDESK_SAFEMODE_FLAG)
         MDesk_SwitchDesktopsWithSafeMode(pMdesk, dwDesktop);
      else
         MDesk_SwitchDesktopsRegularMode(pMdesk, dwDesktop);
   }
}


/*****************************************************************************
 * MDesk_SwitchDesktopForward
 *
 *   Rotates the desktops forward
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_SwitchDesktopForward(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   DWORD dwNextDesktop;
   
   dwNextDesktop = MDESK_DESKTOP_INC(pMdesk);
   
   if(pMdesk->dwFlags & MDESK_SAFEMODE_FLAG)
      MDesk_SwitchDesktopsWithSafeMode(pMdesk, dwNextDesktop);
   else
      MDesk_SwitchDesktopsRegularMode(pMdesk, dwNextDesktop);   
   
}



/*****************************************************************************
 * MDesk_SwitchDesktopBackward
 *
 *   Rotates the desktops in reverse
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_SwitchDesktopBackward(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   DWORD dwNextDesktop;
   
   dwNextDesktop = MDESK_DESKTOP_DEC(pMdesk);
   
   if(pMdesk->dwFlags & MDESK_SAFEMODE_FLAG)
      MDesk_SwitchDesktopsWithSafeMode(pMdesk, dwNextDesktop);
   else
      MDesk_SwitchDesktopsRegularMode(pMdesk, dwNextDesktop);
   
}




/*****************************************************************************
 * MDesk_DestroyDesktop
 *
 *   Destroys the Desktops
 *
 * INPUT
 *   Handle to desktop
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_DestroyDesktop(HMDESK hMdesk)
{
   PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
   
   if(pMdesk->dwFlags & MDESK_SAFEMODE_FLAG)
      MDesk_RestoreAllDesktopsSafeMode(pMdesk);
   else
      MDesk_RestoreAllDesktopsRegularMode(pMdesk);

   MDesk_DestroyAllUniversalWindows((HMDESK)pMdesk);
   
   LocalFree(pMdesk);
}





/*****************************************************************************
 * MDesk_SwitchDesktopsWithSafeMode
 *
 *   Switches To A particular Desktop using safe mode
 *
 * INPUT
 *   Desktop Structure, Desktop Number
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_SwitchDesktopsWithSafeMode(PMDESK_INTERNAL pMdesk, DWORD dwDesktop)
{
  pMdesk->hProgman = FindWindow(gc_ProgMan, NULL);
  pMdesk->hTaskbar = FindWindow(gc_TaskBarClassName, NULL);

  LockWindowUpdate(GetDesktopWindow());
  
  EnumWindows(MDesk_SaveDesktopSafeMode, (LPARAM)pMdesk);     
  pMdesk->dwCurrentDesktop = dwDesktop;
  MDesk_RestoreDesktopSafeMode(pMdesk);
  
  LockWindowUpdate(NULL);
}


/*****************************************************************************
 * MDesk_SwitchDesktopsRegularMode
 *
 *   Switches To A particular Desktop Without Safe mode
 *
 * INPUT
 *   Desktop Structure, Desktop Number
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_SwitchDesktopsRegularMode(PMDESK_INTERNAL pMdesk, DWORD dwDesktop)
{
  pMdesk->hProgman = FindWindow(gc_ProgMan, NULL);
  pMdesk->hTaskbar = FindWindow(gc_TaskBarClassName, NULL);
        
  LockWindowUpdate(GetDesktopWindow());
  
  EnumWindows(MDesk_SaveDesktopRegularMode, (LPARAM)pMdesk);     
  pMdesk->dwCurrentDesktop = dwDesktop;
  MDesk_RestoreDesktopRegularMode(pMdesk);
  
  LockWindowUpdate(NULL);
}



/*****************************************************************************
 * MDesk_SaveDesktopSafeMode
 *
 *   Saves off the current desktop using safe mode
 *
 * INPUT
 *   Window Handle, Context
 * OUTPUT
 *   TRUE
 *****************************************************************************/
BOOL CALLBACK MDesk_SaveDesktopSafeMode(HWND hWnd, LPARAM lParam)
{
    PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)lParam;
    
    if(IsWindowVisible(hWnd) && pMdesk->hProgman != hWnd && pMdesk->hTaskbar != hWnd && GetParent(hWnd) == HWND_DESKTOP)
    {
       if(!MDesk_IsUniversalWindow(pMdesk, hWnd))
       {
         if(MDesk_SaveWindow(pMdesk, hWnd))
            ShowWindowAsync(hWnd, SW_HIDE); 
       }
    }
    
    return TRUE;
}


/*****************************************************************************
 * MDesk_SaveDesktopRegularMode
 *
 *   Saves off the current desktop
 *
 * INPUT
 *   Window Handle, Context
 * OUTPUT
 *   TRUE
 *****************************************************************************/
BOOL CALLBACK MDesk_SaveDesktopRegularMode(HWND hWnd, LPARAM lParam)
{
    PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)lParam;
    
    if(IsWindowVisible(hWnd) && pMdesk->hProgman != hWnd && pMdesk->hTaskbar != hWnd && GetParent(hWnd) == HWND_DESKTOP)
    {
       if(!MDesk_IsUniversalWindow(pMdesk, hWnd))
       {
         if(MDesk_SaveWindow(pMdesk, hWnd))
            ShowWindow(hWnd, SW_HIDE); 
       }
    }
    
    return TRUE;
}


/*****************************************************************************
 * MDesk_IsUniversalWindow
 *
 *   Determins if window is a universal window
 *
 * INPUT
 *   Multidesk Structure, Window Handle
 * OUTPUT
 *   TRUE = Universal Window, FALSE = Not Universal Window
 *****************************************************************************/
BOOL MDesk_IsUniversalWindow(PMDESK_INTERNAL pMdesk, HWND hWnd)
{
    BOOL bRetValue = FALSE;
    PAPPWINDOW pUniversalWindowWalker = pMdesk->pUniversalWindows;
    
    while(pUniversalWindowWalker)
    {
        if(pUniversalWindowWalker->hWnd == hWnd)
           bRetValue = TRUE;
           
        pUniversalWindowWalker = pUniversalWindowWalker->pNext;
    }
    
    return bRetValue;
}


/*****************************************************************************
 * MDesk_SaveWindow
 *
 *   Saves window
 *
 * INPUT
 *   Multidesk Structure, Window Handle
 * OUTPUT
 *   TRUE = Window Saved, FALSE = Not Saved
 *****************************************************************************/
BOOL MDesk_SaveWindow(PMDESK_INTERNAL pMdesk, HWND hWnd)
{
    BOOL bRetValue = FALSE;
    PAPPWINDOW pWindowWalker = pMdesk->pAppWindows, pFocusFinder;
    BOOL bFound      = FALSE;
    BOOL bFocusValue = TRUE;
    
    if(pWindowWalker)
    {
       pFocusFinder = pWindowWalker;
       
       while(pFocusFinder)
       {
          if(pFocusFinder->dwDesktop == pMdesk->dwCurrentDesktop && pFocusFinder->bFocus)
             bFocusValue = FALSE;
          
          pFocusFinder = pFocusFinder->pNext;
       }
       
       while(!bFound)
       {
          if(pWindowWalker->hWnd == hWnd && pWindowWalker->dwDesktop == pMdesk->dwCurrentDesktop)
             bFound = TRUE;
          else
          {
                    
            if(!pWindowWalker->pNext)
               bFound = TRUE;
            else
               pWindowWalker = pWindowWalker->pNext;
          }
       }
       
       if(!(pWindowWalker->hWnd == hWnd && pWindowWalker->dwDesktop == pMdesk->dwCurrentDesktop))
       {
         if(pWindowWalker->pNext = (PAPPWINDOW)LocalAlloc(LMEM_ZEROINIT, sizeof(APPWINDOW)))
         {
             pWindowWalker->pNext->pBefore = pWindowWalker;
             pWindowWalker                 = pWindowWalker->pNext;
             pWindowWalker->hWnd           = hWnd;
             pWindowWalker->dwDesktop      = pMdesk->dwCurrentDesktop;
             pWindowWalker->bFocus         = bFocusValue;
             bRetValue = TRUE;
         }
       }
       else
       {
         pWindowWalker->bFocus = bFocusValue;
         bRetValue = TRUE;
       }
    }
    else
    {
       if(pMdesk->pAppWindows = (PAPPWINDOW)LocalAlloc(LMEM_ZEROINIT, sizeof(APPWINDOW)))
       {
           pMdesk->pAppWindows->hWnd      = hWnd;
           pMdesk->pAppWindows->dwDesktop = pMdesk->dwCurrentDesktop;
           bRetValue = TRUE;
       }
    }
    
    return bRetValue;
}






/*****************************************************************************
 * MDesk_RestoreDesktopRegularMode
 *
 *   Restores the Desktops using regular mode
 *
 * INPUT
 *   Desktop Structure
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_RestoreDesktopRegularMode(PMDESK_INTERNAL pMdesk)
{
  PAPPWINDOW pWindowWalker = pMdesk->pAppWindows;
  HWND hWnd = HWND_DESKTOP;
  
  while(pWindowWalker)
  {
     if(pWindowWalker->dwDesktop == pMdesk->dwCurrentDesktop)
     {
        if(IsWindow(pWindowWalker->hWnd) && (!MDesk_IsUniversalWindow(pMdesk, pWindowWalker->hWnd)))
        {
           ShowWindow(pWindowWalker->hWnd, SW_SHOW);
           
           if(pWindowWalker->bFocus)
           {
              pWindowWalker->bFocus = FALSE;
              hWnd = pWindowWalker->hWnd;
           }
           
           pWindowWalker = pWindowWalker->pNext;
        }
        else
        {
           if(pWindowWalker->pBefore)
           {
              PAPPWINDOW pWindowWalkerTemp = pWindowWalker->pNext;
           
              pWindowWalker->pBefore->pNext = pWindowWalker->pNext;
           
              if(pWindowWalker->pNext)
                pWindowWalker->pNext->pBefore = pWindowWalker->pBefore;
              
              LocalFree(pWindowWalker);
           
              pWindowWalker = pWindowWalkerTemp;
           
           }
           else
           {
             if(pWindowWalker->pNext)
               pWindowWalker->pNext->pBefore = NULL;
             
             pMdesk->pAppWindows = pWindowWalker->pNext;
           
             LocalFree(pWindowWalker);
           
             pWindowWalker = pMdesk->pAppWindows;
           }
        }
     }
     else
       pWindowWalker = pWindowWalker->pNext;
     
       
  }  
  
  if(!pMdesk->pUniversalWindows)
  {
     SetFocus(hWnd);
     SetActiveWindow(hWnd);
     BringWindowToTop(hWnd);
     SetForegroundWindow(hWnd);
  }
}


/*****************************************************************************
 * MDesk_RestoreDesktopSafeMode
 *
 *   Restores the Desktops using safe mode
 *
 * INPUT
 *   Desktop Structure
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_RestoreDesktopSafeMode(PMDESK_INTERNAL pMdesk)
{
  PAPPWINDOW pWindowWalker = pMdesk->pAppWindows;
  HWND hWnd = HWND_DESKTOP;
  
  while(pWindowWalker)
  {
     if(pWindowWalker->dwDesktop == pMdesk->dwCurrentDesktop)
     {
        if(IsWindow(pWindowWalker->hWnd) && (!MDesk_IsUniversalWindow(pMdesk, pWindowWalker->hWnd)))
        {
           ShowWindowAsync(pWindowWalker->hWnd, SW_SHOW);
           
           if(pWindowWalker->bFocus)
           {
              pWindowWalker->bFocus = FALSE;
              hWnd = pWindowWalker->hWnd;
           }
           
           pWindowWalker = pWindowWalker->pNext;
        }
        else
        {
          if(pWindowWalker->pBefore)
          {
             PAPPWINDOW pWindowWalkerTemp = pWindowWalker->pNext;
           
             pWindowWalker->pBefore->pNext = pWindowWalker->pNext;
           
             if(pWindowWalker->pNext)
                pWindowWalker->pNext->pBefore = pWindowWalker->pBefore;
              
             LocalFree(pWindowWalker);
           
             pWindowWalker = pWindowWalkerTemp;
           
          }
          else
          {
             if(pWindowWalker->pNext)
               pWindowWalker->pNext->pBefore = NULL;
             
             pMdesk->pAppWindows = pWindowWalker->pNext;
           
             LocalFree(pWindowWalker);
           
             pWindowWalker = pMdesk->pAppWindows;
          }
        }
        
     }
     else
        pWindowWalker = pWindowWalker->pNext;
     
       
  }  
  
  if(!pMdesk->pUniversalWindows)
  {
     SetFocus(hWnd);
     SetActiveWindow(hWnd);
     BringWindowToTop(hWnd);
     SetForegroundWindow(hWnd);
  }
}






/*****************************************************************************
 * MDesk_RestoreAllDesktopsSafeMode
 *
 *   Restores All the Desktops using safe mode
 *
 * INPUT
 *   Desktop Structure
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_RestoreAllDesktopsSafeMode(PMDESK_INTERNAL pMdesk)
{
  PAPPWINDOW pWindowWalker = pMdesk->pAppWindows;
  
  if(pWindowWalker)
  {
     while(pWindowWalker->pNext)
     {
       if(IsWindow(pWindowWalker->hWnd))
          ShowWindowAsync(pWindowWalker->hWnd, SW_SHOW);
    
       pWindowWalker = pWindowWalker->pNext;
       LocalFree(pWindowWalker->pBefore);
     }
  
     ShowWindowAsync(pWindowWalker->hWnd, SW_SHOW);
     LocalFree(pWindowWalker);
  }
  
  pMdesk->pAppWindows = NULL;
}




/*****************************************************************************
 * MDesk_RestoreAllDesktopsRegularMode
 *
 *   Restores All the Desktops using regular mode
 *
 * INPUT
 *   Desktop Structure
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void MDesk_RestoreAllDesktopsRegularMode(PMDESK_INTERNAL pMdesk)
{
  PAPPWINDOW pWindowWalker = pMdesk->pAppWindows;
  
  if(pWindowWalker)
  {
     while(pWindowWalker->pNext)
     {
       if(IsWindow(pWindowWalker->hWnd))
         ShowWindow(pWindowWalker->hWnd, SW_SHOW);
    
       pWindowWalker = pWindowWalker->pNext;
       LocalFree(pWindowWalker->pBefore);
     }
  
     ShowWindowAsync(pWindowWalker->hWnd, SW_SHOW);
     LocalFree(pWindowWalker);
  }
  
  pMdesk->pAppWindows = NULL;
}






/*****************************************************************************
 * MDesk_DestroyAllUniversalWindows
 *
 *   Destroys all universal windows
 *
 * INPUT
 *   Desktop Structure
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_DestroyAllUniversalWindows(HMDESK hMdesk)
{
  PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
  PAPPWINDOW pWindowWalker = pMdesk->pUniversalWindows;
  
  if(pWindowWalker)
  {
     while(pWindowWalker->pNext)
     {
       pWindowWalker = pWindowWalker->pNext;
       LocalFree(pWindowWalker->pBefore);
     } 
 
     LocalFree(pWindowWalker);
  }
  
  pMdesk->pUniversalWindows = NULL;
}



/*****************************************************************************
 * MDesk_AddUniversalWindow
 *
 *   Adds a universal window
 *
 * INPUT
 *   Multidesk handle, Window Handle
 * OUTPUT
 *   TRUE = Universal Window Added, FALSE = Not Added
 *****************************************************************************/
BOOL WINAPI MDesk_AddUniversalWindow(HMDESK hMdesk, HWND hWnd)
{
    BOOL bRetValue = FALSE;
    PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
    PAPPWINDOW pUniversalWindowWalker = pMdesk->pUniversalWindows;

    if(!MDesk_IsUniversalWindow(pMdesk, hWnd))
    {
       if(pUniversalWindowWalker)
       {
          while(pUniversalWindowWalker->pNext)
              pUniversalWindowWalker = pUniversalWindowWalker->pNext;
              
          if(pUniversalWindowWalker->pNext = (PAPPWINDOW)LocalAlloc(LMEM_ZEROINIT, sizeof(APPWINDOW)))
          {
                bRetValue = TRUE;
                pUniversalWindowWalker->pNext->pBefore = pUniversalWindowWalker;
                pUniversalWindowWalker->pNext->hWnd    = hWnd;
          }
       }
       else
       {
          if(pMdesk->pUniversalWindows = (PAPPWINDOW)LocalAlloc(LMEM_ZEROINIT, sizeof(APPWINDOW)))
          {
                bRetValue = TRUE;
                pMdesk->pUniversalWindows->hWnd    = hWnd;
          }
       }
    }
    
    return bRetValue;
}




/*****************************************************************************
 * MDesk_RemoveUniversalWindow
 *
 *   Removes a universal window
 *
 * INPUT
 *   Multidesk Structure, Window Handle
 * OUTPUT
 *   Nothing
 *****************************************************************************/
void WINAPI MDesk_RemoveUniversalWindow(HMDESK hMdesk, HWND hWnd)
{
    PMDESK_INTERNAL pMdesk = (PMDESK_INTERNAL)hMdesk;
    PAPPWINDOW pUniversalWindowWalker = pMdesk->pUniversalWindows;

    while(pUniversalWindowWalker)
    {
        if(pUniversalWindowWalker->hWnd == hWnd)
        {
          if(pUniversalWindowWalker->pBefore)
          {
               PAPPWINDOW pUniversalWindowTemp = pUniversalWindowWalker->pNext;

               pUniversalWindowWalker->pBefore->pNext = pUniversalWindowWalker->pNext;
               
               if(pUniversalWindowWalker->pNext)
                  pUniversalWindowWalker->pNext->pBefore = pUniversalWindowWalker->pBefore;
               
               LocalFree(pUniversalWindowWalker);
               pUniversalWindowWalker = pUniversalWindowTemp;
          }
          else
          {
             if(pUniversalWindowWalker->pNext)
                pUniversalWindowWalker->pNext->pBefore = NULL;
                
              pMdesk->pUniversalWindows = pUniversalWindowWalker->pNext;
              
              LocalFree(pUniversalWindowWalker);
              
              pUniversalWindowWalker = pMdesk->pUniversalWindows;
          }
        
        }

        if(pUniversalWindowWalker)
           pUniversalWindowWalker = pUniversalWindowWalker->pNext;
    }
    
}

 