#include <stddef.h>

#include "RTCS.h"

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif

#ifndef TEST
#define FOREVER() (1)
#else
extern int NumLoops;
#define FOREVER() (NumLoops--)
#endif

#define ARRAY_LEN(a) ( sizeof(a) / sizeof(a[0]) )

volatile STATIC RTCS_Task_t TaskTable[RTCS_NUM_TASKS];

void RTCS_Init(void)
{
   for (uint8_t i = 0; i < RTCS_NUM_TASKS; i++)
   {
      TaskTable[i].Task = (void *)0;
      TaskTable[i].Period = 0;
      TaskTable[i].TicksRemaining = 0;
      TaskTable[i].RunRequestsPending = 0;
   }
}

RTCS_Status_t RTCS_AddTask(void task(void), uint8_t priority, uint32_t period)
{
   if (task == NULL || priority >= ARRAY_LEN(TaskTable))
   {
      return RTCS_Status_Failure;
   }

   TaskTable[priority].Task = task;
   TaskTable[priority].Period = period;
   TaskTable[priority].TicksRemaining = period;
   TaskTable[priority].RunRequestsPending = 0;

   return RTCS_Status_Success;
}

void RTCS_Run(void)
{
   while (FOREVER())
   {
      // start at top of table to find highest priority ready task
      for (uint8_t i = 0; i < RTCS_NUM_TASKS; i++)
      {
         if ((TaskTable[i].Task != 0) && (TaskTable[i].RunRequestsPending > 0))
         {
            TaskTable[i].RunRequestsPending--;
            TaskTable[i].Task();
            break; // restarts at top of table
         }
      }
   }
}

void RTCS_Tick(void)
{
   for (uint8_t i = 0; i < RTCS_NUM_TASKS; i++)
   {
      if ((TaskTable[i].Task != 0) && (TaskTable[i].TicksRemaining > 0))
      {
         // decrement TicksRemaining, if 0 then time to run
         if (--TaskTable[i].TicksRemaining == 0)
         {
            TaskTable[i].RunRequestsPending++;
            TaskTable[i].TicksRemaining = TaskTable[i].Period;
         }
      }
   }
}
