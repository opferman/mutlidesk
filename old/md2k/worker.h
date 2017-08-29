/*--------------------------------------------------*
   Toby Opferman

   http://www.opferman.com
   http://www.opferman.net
   toby@opferman.com

   Worker Threads
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=;
;    D I S C L A I M E R  AND  L I S E N C E       ;
;                                                  ;    
;                                                  ;
;I license out this code for educational use only. ;
;  There are no warrenties implied or here in.     ;
;  Use at your own risk.  I am not responsible for ; 
;  any catastropic ressults occuring from use or   ;
;  replication of this code in any way or form.    ;
;  Free to use code as a reference or educational  ;
;  tool only.  If you use any of the subroutines   ;
;  give credit where credit is due.  No using      ;
;  any part of this code to make commerical        ;
;  products unless notifing the author for         ;
;  permission.                                     ;
;                                                  ;    
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=; 
 *--------------------------------------------------*/

#ifndef __WORKER_H__
#define __WORKER_H__

/* Constants */
#define IGNORE_WORK_COMPLEGE_FLAG  0
#define SEND_WORK_COMPLETE_FLAG    1
#define SEQ_WORK_BEG_FLAG          2
#define SEQ_WORK_END_FLAG          4

#define INVALID_WORK_ID                    (WORK_ID)-1

#define NO_WORK_DELAY_TIME                  400
#define WORK_DELAY_TIME                      10
#define WORKER_CLOSE_WAIT                  1000
#define WORK_SEQ_DELAY_TIME                   0


/* Types */
typedef DWORD WORK_ID;
typedef PVOID WORKHANDLE;
typedef PVOID WORKITEMHANDLE;

/* Structures */
typedef struct __work_structure_type__ {

 DWORD (WINAPI *WorkItem)(PVOID);
 PVOID WorkItemContext;

 DWORD WorkItemFlags;

 DWORD (WINAPI *WorkItemComplete)(PVOID, WORK_ID, DWORD);
 PVOID WorkItemCompleteContext;
 WORK_ID WID;

 struct __work_structure_type__ *NextWorkItem;

} WORKSTRUC, *PWORKSTRUC;



typedef struct __work_context_type__ {
 
 HANDLE WorkMutex;
 HANDLE KillAllThreadEvent;
 HANDLE KillOneThreadEvent;
 DWORD NumberOfThreads;

 PWORKSTRUC pWorkStruc;


} WORKCONTEXT, *PWORKCONTEXT;

/* Function Prototypes */
void WINAPI CloseWorkerThreads(WORKHANDLE);
void WINAPI RemoveWorkerThreads(WORKHANDLE, DWORD);
void WINAPI ClearAllWork(WORKHANDLE);
BOOL WINAPI ClearWorkItem(WORKHANDLE, WORK_ID);
DWORD WINAPI GetNumberOfWorkerThreads(WORKHANDLE);
WORKHANDLE WINAPI CreateNewWorkerThreads(DWORD, DWORD);
DWORD WINAPI AddWorkerThreads(WORKHANDLE, DWORD, DWORD);
WORK_ID WINAPI AddWorkItem(WORKHANDLE, PWORKSTRUC);

#endif


