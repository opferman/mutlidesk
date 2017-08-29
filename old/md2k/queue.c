/*--------------------------------------------------*
   MultiDesk 2000

   Toby Opferman

   Decemeber 2000

 *--------------------------------------------------*/
#include <windows.h>
#include "queue.h"

/*-------------------------------------------------------*
    USE QUEUE
 *-------------------------------------------------------*/
 DWORD UseQueue(DWORD QueueOp, DWORD Value)
{
   DWORD RetVal = -1;
   static PQUEUE pQueue = 0;
   PQUEUE pTemp;
   static CRITICAL_SECTION cs;

   switch(QueueOp)
   {
      case QUEUE_INIT:
        InitializeCriticalSection(&cs);
        break;
  
      case QUEUE_UNINIT:
        DeleteCriticalSection(&cs);
        break;

      case QUEUE_PUSH:
         EnterCriticalSection(&cs);      
         if(pQueue)
         {
            pTemp = pQueue;

            while(pTemp->Next)
                pTemp = pTemp->Next;

            if(!(pTemp->Next = LocalAlloc(LMEM_ZEROINIT, sizeof(QUEUE))))
               return RetVal;
         
            pTemp->Next->Value = Value;
            pTemp->Next->Before = pTemp;
         
            RetVal = 0;
           
         }
         else
         {
            if(!(pQueue = LocalAlloc(LMEM_ZEROINIT, sizeof(QUEUE))))
               return RetVal;

            pQueue->Value = Value;

            RetVal = 0;
         }

         LeaveCriticalSection(&cs);      
         break;

      case QUEUE_POP:
         EnterCriticalSection(&cs);
         if(pQueue)
         {  
            RetVal = pQueue->Value;
            pTemp = pQueue->Next;

            if(pTemp)
              pTemp->Before = NULL;

            LocalFree(pQueue);
            pQueue = pTemp;
         } 
         LeaveCriticalSection(&cs);
         break;
   }

   return RetVal;
}



