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

#include <windows.h>
#include "worker.h"

/* Internal Function Prototypes */
 void WINAPI PerformWorkItem(WORKITEMHANDLE);
 DWORD WINAPI WorkerThread(WORKHANDLE);
 WORKITEMHANDLE WINAPI GetWorkItem(WORKHANDLE);
 BOOL WINAPI WorkerThreadAlive(WORKHANDLE);

/*-------------------------------------------------------*
     WORKER THREAD
 *-------------------------------------------------------*/
 DWORD WINAPI WorkerThread(WORKHANDLE WorkerContext)
{
   WORKITEMHANDLE WorkItem = NULL;
   
   while(WorkerThreadAlive(WorkerContext))
   {

      WorkItem = GetWorkItem(WorkerContext);

      if(WorkItem)
      {
        PerformWorkItem(WorkItem);
        Sleep(WORK_DELAY_TIME);
      }
      else
        Sleep(NO_WORK_DELAY_TIME);
   }

   /* Exit The Thread */
   ExitThread(0);

   return 0;
}


/*-------------------------------------------------------*
     GET NEXT WORK ITEM
 *-------------------------------------------------------*/
 WORKITEMHANDLE WINAPI GetWorkItem(WORKHANDLE Context)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   PWORKSTRUC WorkItem, WorkWalker;
   BOOL StillWork = TRUE;
   
   WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
   
   /* Get Work Item */
   if(WorkItem = pWorkContext->pWorkStruc)
   {
      pWorkContext->pWorkStruc = pWorkContext->pWorkStruc->NextWorkItem;

      /* Check If Work Has To Be Done Sequentially & Commit to thread */
      if(WorkItem->WorkItemFlags & SEQ_WORK_BEG_FLAG)
      {
         WorkWalker = WorkItem;

         while(pWorkContext->pWorkStruc && StillWork)
         {
             if(pWorkContext->pWorkStruc->WorkItemFlags & SEQ_WORK_END_FLAG)
                StillWork = FALSE;

             WorkWalker = WorkWalker->NextWorkItem;
             pWorkContext->pWorkStruc = pWorkContext->pWorkStruc->NextWorkItem;
         }
         
         WorkWalker->NextWorkItem = NULL;
      }
      else
          WorkItem->NextWorkItem = NULL;

   }

   ReleaseMutex(pWorkContext->WorkMutex);
   
   return (WORKITEMHANDLE)WorkItem;
}


/*-------------------------------------------------------*
    CHECK IF WORKER THREAD LIVES
 *-------------------------------------------------------*/
 BOOL WINAPI WorkerThreadAlive(WORKHANDLE Context)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   BOOL ThreadAlive = TRUE;
  
   WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);



   /* Check if Thread Is Killed */
   if(WaitForSingleObject(pWorkContext->KillAllThreadEvent, 1) == WAIT_OBJECT_0)
   {
      pWorkContext->NumberOfThreads--; 
      ThreadAlive = FALSE;      
   }
   else if(WaitForSingleObject(pWorkContext->KillOneThreadEvent, 1) == WAIT_OBJECT_0)
        {
           pWorkContext->NumberOfThreads--; 
           ThreadAlive = FALSE;      
        }

   ReleaseMutex(pWorkContext->WorkMutex);
   
   return ThreadAlive;
}


/*-------------------------------------------------------*
    KILL ALL WORKER THREADS (MUST BE CALLED)
 *-------------------------------------------------------*/
 void WINAPI CloseWorkerThreads(WORKHANDLE Context)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   BOOL ThreadsAlive = TRUE;
   PWORKSTRUC pWorkStruc;   

   WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
   SetEvent(pWorkContext->KillAllThreadEvent);
   ReleaseMutex(pWorkContext->WorkMutex);
   
   while(ThreadsAlive)
   {
      WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);

      if(pWorkContext->NumberOfThreads == 0)
         ThreadsAlive = FALSE;

      ReleaseMutex(pWorkContext->WorkMutex);

      if(ThreadsAlive)
        Sleep(WORKER_CLOSE_WAIT);
   }

   /* Perform Clean up */
   while(pWorkContext->pWorkStruc)
   {
      pWorkStruc = pWorkContext->pWorkStruc;
      pWorkContext->pWorkStruc = pWorkContext->pWorkStruc->NextWorkItem;

      LocalFree(pWorkStruc);
   }

   CloseHandle(pWorkContext->WorkMutex);
   CloseHandle(pWorkContext->KillAllThreadEvent);
   CloseHandle(pWorkContext->KillOneThreadEvent);   

   LocalFree(pWorkContext);
}


/*-------------------------------------------------------*
    KILL A WORKER THREAD
 *-------------------------------------------------------*/
 void WINAPI RemoveWorkerThreads(WORKHANDLE Context, DWORD NumberOfThreads)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   BOOL ThreadsKilled = FALSE, ThreadKilled;
   DWORD NumThreads;

   if(!NumberOfThreads)
     return;

   while(!ThreadsKilled)
   {
       WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
       NumThreads = pWorkContext->NumberOfThreads;

       if(NumThreads)
       {
          SetEvent(pWorkContext->KillOneThreadEvent);
          ThreadKilled = FALSE;
       }
       else
          ThreadsKilled = ThreadKilled = TRUE;

       ReleaseMutex(pWorkContext->WorkMutex);
       
       while(!ThreadKilled)
       {
           WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
   
           if(pWorkContext->NumberOfThreads < NumThreads)
           {
              ThreadKilled = FALSE;

              if(!(--NumberOfThreads))
                 ThreadsKilled = TRUE;
           }

           ReleaseMutex(pWorkContext->WorkMutex);

           if(!ThreadKilled)
             Sleep(WORKER_CLOSE_WAIT);
       }
   }
}



/*-------------------------------------------------------*
    CLEAR ALL WORK
 *-------------------------------------------------------*/
 void WINAPI ClearAllWork(WORKHANDLE Context)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   PWORKSTRUC pWorkStruc;   

   WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);

   /* Perform Clean up */
   while(pWorkContext->pWorkStruc)
   {
      pWorkStruc = pWorkContext->pWorkStruc;
      pWorkContext->pWorkStruc = pWorkContext->pWorkStruc->NextWorkItem;

      LocalFree(pWorkStruc);
   }

   ReleaseMutex(pWorkContext->WorkMutex);
}



/*-------------------------------------------------------*
    CLEAR WORK ITEM
 *-------------------------------------------------------*/
 BOOL WINAPI ClearWorkItem(WORKHANDLE Context, WORK_ID WID)
{
   PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
   PWORKSTRUC pWorkStruc, pBefore;
   BOOL Found = FALSE;

   WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);

   pBefore = pWorkStruc = pWorkContext->pWorkStruc;

   /* Find Work ID */
   while(pWorkStruc && !Found)
   {
      if(pWorkStruc->WID == WID)
         Found = TRUE;
      else
      {
           pBefore = pWorkStruc;
           pWorkStruc = pWorkStruc->NextWorkItem;
      }
   }

   if(Found)
   {
      if(pBefore == pWorkStruc)
      {
          pWorkContext->pWorkStruc = pWorkContext->pWorkStruc->NextWorkItem;
          LocalFree(pWorkStruc);
      }
      else
      {
          pBefore = pWorkStruc->NextWorkItem;
          LocalFree(pWorkStruc);
      }
   }

   ReleaseMutex(pWorkContext->WorkMutex);

   return Found;
}

/*-------------------------------------------------------*
    PERFORM WORK ITEM
 *-------------------------------------------------------*/
 void WINAPI PerformWorkItem(WORKITEMHANDLE Context)
{
   PWORKSTRUC pWorkStruc = (PWORKSTRUC)Context, pTempWalker;
   DWORD RetVal;  

   while(pWorkStruc)
   {

      RetVal = pWorkStruc->WorkItem(pWorkStruc->WorkItemContext);

      if(pWorkStruc->WorkItemFlags & SEND_WORK_COMPLETE_FLAG)
         pWorkStruc->WorkItemComplete(pWorkStruc->WorkItemCompleteContext, pWorkStruc->WID, RetVal);
   
      pTempWalker = pWorkStruc;
      pWorkStruc = pWorkStruc->NextWorkItem;
      LocalFree(pTempWalker);

      Sleep(WORK_SEQ_DELAY_TIME);
   }
}

/*-------------------------------------------------------*
    CREATE WORKER THREAD
 *-------------------------------------------------------*/
 WORKHANDLE WINAPI CreateNewWorkerThreads(DWORD Number, DWORD StackSize)
{
    PWORKCONTEXT pWorkContext = NULL;
    DWORD ThreadID;

    if(!Number)
    {
      SetLastError(ERROR_BAD_ARGUMENTS);
      return NULL;
    }

    if(!(pWorkContext = (PWORKCONTEXT)LocalAlloc(LMEM_ZEROINIT, sizeof(WORKCONTEXT))))
    {
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      return NULL;
    }

  
    if(!(pWorkContext->WorkMutex = CreateMutex(NULL, FALSE, NULL)))
    {
      SetLastError(ERROR_TOO_MANY_SEMAPHORES);
      return NULL;
    }
    
    if(!(pWorkContext->KillOneThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
    {
      CloseHandle(pWorkContext->WorkMutex);
      SetLastError(ERROR_TOO_MANY_SEMAPHORES);
      return NULL;
    }

    if(!(pWorkContext->KillAllThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
    {
      CloseHandle(pWorkContext->WorkMutex);
      CloseHandle(pWorkContext->KillOneThreadEvent);
      SetLastError(ERROR_TOO_MANY_SEMAPHORES);
      return NULL;
    }
    
    while(Number--)
    {
       if(CreateThread(NULL, StackSize, WorkerThread, (LPVOID)pWorkContext, 0, &ThreadID))
       {
           WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
           pWorkContext->NumberOfThreads++;
           ReleaseMutex(pWorkContext->WorkMutex);
       }
    }
  
    return (WORKHANDLE)pWorkContext;
}


/*-------------------------------------------------------*
    GET NUMBER OF THREADS
 *-------------------------------------------------------*/
 DWORD WINAPI GetNumberOfWorkerThreads(WORKHANDLE Context)
{
    PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
    DWORD Number;

    WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
    Number = pWorkContext->NumberOfThreads;
    ReleaseMutex(pWorkContext->WorkMutex);

    return Number;
}


/*-------------------------------------------------------*
    ADD MORE THREADS
 *-------------------------------------------------------*/
 DWORD WINAPI AddWorkerThreads(WORKHANDLE Context, DWORD Number, DWORD StackSize)
{
    PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
    DWORD ThreadID;
    DWORD NumberAdded = 0;

    while(Number--)
    {
       if(CreateThread(NULL, StackSize, WorkerThread, (LPVOID)pWorkContext, 0, &ThreadID))
       {
           WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
           NumberAdded = ++pWorkContext->NumberOfThreads;
           ReleaseMutex(pWorkContext->WorkMutex);
       }
    }

    return NumberAdded;
}


/*-------------------------------------------------------*
    ADD WORK TO THE THREADS
 *-------------------------------------------------------*/
 WORK_ID WINAPI AddWorkItem(WORKHANDLE Context, PWORKSTRUC pWorkStruc)
{
    PWORKCONTEXT pWorkContext = (PWORKCONTEXT)Context;
    PWORKSTRUC pWalker;
    static WORK_ID WID = 0;

    WaitForMultipleObjects(1, &pWorkContext->WorkMutex, TRUE, INFINITE);
    
    if(pWorkContext->pWorkStruc)
    {
         pWalker = pWorkContext->pWorkStruc; 

         while(pWalker->NextWorkItem)
             pWalker = pWalker->NextWorkItem;
  
         while(pWorkStruc)
         {
            if(!(pWalker->NextWorkItem = LocalAlloc(LMEM_ZEROINIT, sizeof(WORKSTRUC))))
            {
                 SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                 return INVALID_WORK_ID;
            }
            
            CopyMemory(pWalker->NextWorkItem, pWorkStruc, sizeof(WORKSTRUC));

            pWalker = pWalker->NextWorkItem;
            pWalker->NextWorkItem= 0;

            WID++;

            if(WID == INVALID_WORK_ID)
              WID++;       

            pWalker->WID = pWorkStruc->WID = WID;
            pWorkStruc = pWorkStruc->NextWorkItem;
         }

    }
    else
    {

         if(!(pWorkContext->pWorkStruc = LocalAlloc(LMEM_ZEROINIT, sizeof(WORKSTRUC))))
         {
              SetLastError(ERROR_NOT_ENOUGH_MEMORY);
              return INVALID_WORK_ID;
         }
         
         CopyMemory(pWorkContext->pWorkStruc, pWorkStruc, sizeof(WORKSTRUC));

         pWorkContext->pWorkStruc->NextWorkItem = 0;
         pWalker = pWorkContext->pWorkStruc;

         WID++;

         if(WID == INVALID_WORK_ID)
           WID++;       

         pWorkContext->pWorkStruc->WID = pWorkStruc->WID = WID;
         pWorkStruc = pWorkStruc->NextWorkItem;

         while(pWorkStruc)
         {
            if(!(pWalker->NextWorkItem = LocalAlloc(LMEM_ZEROINIT, sizeof(WORKSTRUC))))
            {
                 SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                 return INVALID_WORK_ID;
            }
            
            CopyMemory(pWalker->NextWorkItem, pWorkStruc, sizeof(WORKSTRUC));

            pWalker = pWalker->NextWorkItem;
            pWalker->NextWorkItem= 0;

            WID++;

            if(WID == INVALID_WORK_ID)
              WID++;       

            pWalker->WID = pWorkStruc->WID = WID;
            pWorkStruc = pWorkStruc->NextWorkItem;
         }
    }

    ReleaseMutex(pWorkContext->WorkMutex);

    return WID;  
}


  