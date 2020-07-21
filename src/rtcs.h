/**
 * @file RTCS.h
 * @author Brooks Anderson
 * @date 16 July 2020
 * @brief A run-to-completion-scheduler.
 */

#ifndef _RTCS_H
#define _RTCS_H

#include <stdint.h>

/**
 * Defines the number of tasks needed for the application.
 *
 * @note This value directly affects the amount of RAM used by the scheduler.
 */
#define RTCS_NUM_TASKS (10)

/**
 * Structure represents a task entry. None of the members of this structure
 * are intended to be accessed outside of the RTCS functions.
 */
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

/**
 * Initializes the scheduler and removes all existing tasks.
 */
void RTCS_Init(void);

/**
 * Increments the scheduler tick.
 *
 * This function should be called periodically in some timer ISR such as
 * SysTick_Handler(). The accuracy of the caller determines the time accuracy
 * of the scheduler. The period in which this function is called defines the
 * tick unit. For example if this function is called in an ISR every
 * millisecond then 1 tick is equal to 1 millisecond.
 */
void RTCS_Tick(void);

/**
 * Adds a new task entry to the scheduler.
 *
 * @param task The function to be executed
 * @param priority Task importance relative to all other tasks. 0 = highest
 * @param period How often the task should run. unit: ticks, defined by RTCS_Tick()
 *
 * @note Two or more tasks cannot have the same priorty value.
 */
RTCS_Status_t RTCS_AddTask(void task(void), uint8_t priority, uint32_t period);

/**
 * Runs the scheduler.
 *
 * This function will never yield control back to the caller. It is intended
 * to be placed at the end of initialization code eg. main().
 */
void RTCS_Run(void);

#endif /* _RTCS_H */