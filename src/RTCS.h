#ifndef RTCS_H
#define RTCS_H

#include <stdint.h>

#define RTCS_NUM_TASKS (10)

typedef struct _RTCS_Task_t
{
   void (*Task)(void);
   uint32_t Period;
   uint32_t TicksRemaining;
   char RunRequestsPending;
} RTCS_Task_t;

typedef enum
{
   RTCS_Status_Success = 1,
   RTCS_Status_Failure = 0,
} RTCS_Status_t;

void RTCS_Init(void);

RTCS_Status_t RTCS_AddTask(void task(void), uint8_t priority, uint32_t period);

void RTCS_Run(void);

void RTCS_Tick(void);

#endif