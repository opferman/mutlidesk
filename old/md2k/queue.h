/*--------------------------------------------------*
   MultiDesk 2000

   Toby Opferman

   Decemeber 2000

 *--------------------------------------------------*/

#ifndef __QUEUE_H__
#define __QUEUE_H__

/* Constants */
#define QUEUE_PUSH    0
#define QUEUE_POP     1
#define QUEUE_INIT    2
#define QUEUE_UNINIT  3

/* Structure */
typedef struct __queue_type__ {

  DWORD Value;
  struct __queue_type__ *Next, *Before;

} QUEUE, *PQUEUE;

/* Prototypes */
 DWORD UseQueue(DWORD, DWORD); 

#endif

