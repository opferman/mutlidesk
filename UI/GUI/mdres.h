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
 
 
#ifndef __MDRES_H__
#define __MDRES_H__


#ifdef __cplusplus
extern "C" {
#endif




#define MAX_MSG_SIZE                256
#define NUMBER_OF_DESKTOPS          8

#define MDUI_CLASS_NAME             "Multidesk 2001 Class"
#define MD_HELPSTRING               "md2001.hlp"

#define WM_MD_SYSTEM_TRAY           WM_USER

/* Registry Keys */
#define MDUI_REG_KEY                "SOFTWARE\\OPFERMAN\\MDESK"
#define MDUI_HOTKEYS_REG_KEY        "SOFTWARE\\OPFERMAN\\MDESK\\HOTKEYS"
#define MDUI_UNI_REG_KEY            "SOFTWARE\\OPFERMAN\\MDESK\\HOTKEYS\\UNIVERSAL"
#define MDUI_SD_REG_KEY             "SOFTWARE\\OPFERMAN\\MDESK\\HOTKEYS\\SWITCHDESKTOP"

/* Register Values */
#define MDUI_ADD_UNI_MOD_VALUE          "UniversalWindowAdditionModifier"
#define MDUI_SUB_UNI_MOD_VALUE          "UniversalWindowSubtractModifier"
#define MDUI_ADD_UNI_KEY_VALUE          "UniversalWindowAdditionKey"
#define MDUI_SUB_UNI_KEY_VALUE          "UniversalWindowSubtractKey"
#define MDUI_UP_SD_MOD_VALUE            "SwitchDesktopUpModifier"
#define MDUI_DWN_SD_MOD_VALUE           "SwitchDesktopDownModifier"
#define MDUI_UP_SD_KEY_VALUE            "SwitchDesktopUpKey"
#define MDUI_DWN_SD_KEY_VALUE           "SwitchDesktopDownKey"
#define MDUI_SAFEMODE_VALUE             "SafeModeEnable"

#define DEFAULT_MODIFIER                MOD_ALT | MOD_CONTROL
#define DEFAULT_KEY_SD_UP               VK_RIGHT
#define DEFAULT_KEY_SD_DWN              VK_LEFT
#define DEFAULT_KEY_UNI_UP              VK_UP 
#define DEFAULT_KEY_UNI_DWN             VK_DOWN
#define DEFAULT_SAFEMODE                0

/* ICONS */
#define IDC_MAIN_ICON               70

#define IDC_DESKTOPONE_ICON         90
#define IDC_DESKTOPTWO_ICON         91
#define IDC_DESKTOPTHREE_ICON       92 
#define IDC_DESKTOPFOUR_ICON        93 
#define IDC_PICTURE_ICON            94 
#define IDC_DESKTOPFIVE_ICON        95
#define IDC_DESKTOPSIX_ICON         96
#define IDC_DESKTOPSEVEN_ICON       97 
#define IDC_DESKTOPEIGHT_ICON       98 

#define IDC_SYSTEM_TRAY             99

/* Resource IDs */
#define MDUI_HK_ERROR              100 
#define HK_UNI_ADD_ERROR           101 
#define HK_UNI_SUB_ERROR           102
#define HK_SDDOWN_ERROR            103
#define HK_SDUP_ERROR              104
#define SYSTEM_TRAY_TOOLTIP        105

#define IDC_HOTKEY_DPSWITCH_FWD    200
#define IDC_HOTKEY_DPSWITCH_BCK    201
#define IDC_HOTKEY_UNIVERSAL_ON    202
#define IDC_HOTKEY_UNIVERSAL_DOWN  203

#define IDC_SYSTRAY_MENU_DUMMY     300
#define IDC_SYSTRAY_MENU           301

#define IDC_SAFE_MODE              400
#define IDC_DT_ONE                 401
#define IDC_DT_TWO                 402
#define IDC_DT_THREE               403
#define IDC_DT_FOUR                404
#define IDC_ABOUT                  405
#define IDC_EXIT                   406
#define IDC_UNI_CLEAR              407
#define IDC_MDHELP                 408
#define IDC_DT_FIVE                409
#define IDC_DT_SIX                 410
#define IDC_DT_SEVEN               411
#define IDC_DT_EIGHT               412

#define IDD_ABOUTBOX               500
#define IDS_EXIT_TITLE             501
#define IDS_EXIT_QUESTION          502

#ifdef __cplusplus
}
#endif


#endif

