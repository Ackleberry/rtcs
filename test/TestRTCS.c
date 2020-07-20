#include <stdio.h>
#include <stddef.h>

#include "unity.h"
#include "RTCS.h"

extern RTCS_Task_t TaskTable[];

int NumLoops;
static int TaskACalls;
static int TaskBCalls;
static int TaskCCalls;

void setUp(void)
{
   RTCS_Init();
   NumLoops = 0;
   TaskACalls = 0;
   TaskBCalls = 0;
   TaskCCalls = 0;
}

void tearDown(void)
{
}

void TaskA(void)
{
   TaskACalls++;
}

void TaskB(void)
{
   TaskBCalls++;
}

void TaskC(void)
{
   TaskCCalls++;
}

void IncrementTicksBy(int amount)
{
   for(int i = 0; i < amount; i++)
   {
      RTCS_Tick();
   }
}

/**
 * Begin implementation specific tests.
 *
 * These tests were used to initially develop code. Delete any test if it
 * begins to fail during a refactor.
 */
void test_RTCS_Init_should_SetAllTaskDataToZero(void)
{
   RTCS_Task_t ExpectedTable[RTCS_NUM_TASKS] = {0};
   RTCS_Init();

   TEST_ASSERT_EQUAL_MEMORY_ARRAY(ExpectedTable, TaskTable, sizeof(RTCS_Task_t), RTCS_NUM_TASKS);
}

void test_RTCS_AddTask_should_BeAbleToAddBasicTask(void)
{
   int priority = 1;
   uint32_t tickPeriod = 5;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, priority, tickPeriod);

   TEST_ASSERT_EQUAL(RTCS_Status_Success, retVal);
   TEST_ASSERT_EQUAL(TaskA, TaskTable[priority].Task);
   TEST_ASSERT_EQUAL(tickPeriod, TaskTable[priority].Period);
   TEST_ASSERT_EQUAL(tickPeriod, TaskTable[priority].TicksRemaining);
   TEST_ASSERT_EQUAL(0, TaskTable[priority].RunRequestsPending);
}

void test_RTCS_AddTask_should_RejectNullFunctionPtr(void)
{
   int priority = 1;
   uint32_t tickPeriod = 6;

   RTCS_Status_t retVal = RTCS_AddTask(NULL, priority, tickPeriod);

   TEST_ASSERT_EQUAL(RTCS_Status_Failure, retVal);

   // Ensure nothing was touched.
   TEST_ASSERT_NULL(TaskTable[priority].Task);
   TEST_ASSERT_EQUAL(0, TaskTable[priority].Period);
   TEST_ASSERT_EQUAL(0, TaskTable[priority].TicksRemaining);
   TEST_ASSERT_EQUAL(0, TaskTable[priority].RunRequestsPending);
}

void test_RTCS_AddTask_should_RejectOutOfBoundsTaskPriority(void)
{
   int priority = RTCS_NUM_TASKS;
   uint32_t tickPeriod = 3;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, priority, tickPeriod);

   TEST_ASSERT_EQUAL(RTCS_Status_Failure, retVal);
   TEST_ASSERT_NOT_EQUAL(TaskA, TaskTable[priority].Task);
}

void test_RTCS_AddTask_should_AcceptHighPeriodValue(void)
{
   int priority = 1;
   uint32_t tickPeriod = 4294967295;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, 1, tickPeriod);

   TEST_ASSERT_EQUAL(RTCS_Status_Success, retVal);
   TEST_ASSERT_EQUAL(TaskA, TaskTable[priority].Task);
   TEST_ASSERT_EQUAL(4294967295, TaskTable[priority].Period);
}

void test_RTCS_Run_should_RunPendingTask(void)
{
   NumLoops = 1;
   int priority = 1;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, priority, 0);
   TEST_ASSERT_EQUAL(RTCS_Status_Success, retVal);

   // Request task to run
   TaskTable[priority].RunRequestsPending = 1;

   RTCS_Run();

   TEST_ASSERT_EQUAL(0, TaskTable[priority].RunRequestsPending);
   TEST_ASSERT_EQUAL(1, TaskACalls);
}

void test_RTCS_Run_should_RunMultipleRequestsForOneTask(void)
{
   int priority = 1;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, priority, 0);
   TEST_ASSERT_EQUAL(RTCS_Status_Success, retVal);

   // Request task to run X number of times
   TaskTable[priority].RunRequestsPending = 10;
   for (int i = 10; i > 0; i--)
   {
      NumLoops = 1;
      RTCS_Run();
      TEST_ASSERT_EQUAL(i-1, TaskTable[priority].RunRequestsPending);
   }

   TEST_ASSERT_EQUAL(10, TaskACalls);
}

void test_RTCS_Run_should_RunMutiplePendingTasksInOrder(void)
{

   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskA, 0, 0));
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskB, 1, 0));
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskC, 2, 0));

   // Request tasks to run
   TaskTable[0].RunRequestsPending = 1;
   TaskTable[1].RunRequestsPending = 1;
   TaskTable[2].RunRequestsPending = 1;

   for (int i = 0; i < 3; i++)
   {
      NumLoops = 1;
      RTCS_Run();
      TEST_ASSERT_EQUAL(0, TaskTable[i].RunRequestsPending);
   }

   TEST_ASSERT_EQUAL(1, TaskACalls);
   TEST_ASSERT_EQUAL(1, TaskBCalls);
   TEST_ASSERT_EQUAL(1, TaskCCalls);
}

void test_RTCS_Run_should_RunPeriodicTasks(void)
{
   int priority = 1;
   uint32_t period = 100;

   RTCS_Status_t retVal = RTCS_AddTask(TaskA, priority, period);
   TEST_ASSERT_EQUAL(RTCS_Status_Success, retVal);

   IncrementTicksBy(100);
   TEST_ASSERT_EQUAL(1, TaskTable[priority].RunRequestsPending);
   TEST_ASSERT_EQUAL(period, TaskTable[priority].TicksRemaining);

   NumLoops = 1;
   RTCS_Run();

   TEST_ASSERT_EQUAL(1, TaskACalls);
   TEST_ASSERT_EQUAL(0, TaskTable[priority].RunRequestsPending);
   TEST_ASSERT_EQUAL(period, TaskTable[priority].TicksRemaining);
}

void test_RTCS_Run_should_RunMultiplePeriodicTasks(void)
{
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskA, 1, 10));
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskB, 0, 100));
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskC, 2, 1000));

   IncrementTicksBy(10);
   TEST_ASSERT_EQUAL(1, TaskTable[1].RunRequestsPending);
   TEST_ASSERT_EQUAL(10, TaskTable[1].TicksRemaining);
   NumLoops = 10;
   RTCS_Run();
   TEST_ASSERT_EQUAL(1, TaskACalls);
   TEST_ASSERT_EQUAL(0, TaskTable[1].RunRequestsPending);
   TEST_ASSERT_EQUAL(10, TaskTable[1].TicksRemaining);


   IncrementTicksBy(90);
   TEST_ASSERT_EQUAL(1, TaskTable[0].RunRequestsPending);
   TEST_ASSERT_EQUAL(100, TaskTable[0].TicksRemaining);
   NumLoops = 100;
   RTCS_Run();
   TEST_ASSERT_EQUAL(1, TaskBCalls);
   TEST_ASSERT_EQUAL(10, TaskACalls);
   TEST_ASSERT_EQUAL(0, TaskTable[0].RunRequestsPending);
   TEST_ASSERT_EQUAL(100, TaskTable[0].TicksRemaining);


   IncrementTicksBy(900);
   TEST_ASSERT_EQUAL(1, TaskTable[2].RunRequestsPending);
   TEST_ASSERT_EQUAL(1000, TaskTable[2].TicksRemaining);
   NumLoops = 1000;
   RTCS_Run();
   TEST_ASSERT_EQUAL(1, TaskCCalls);
   TEST_ASSERT_EQUAL(10, TaskBCalls);
   TEST_ASSERT_EQUAL(100, TaskACalls);
   TEST_ASSERT_EQUAL(0, TaskTable[2].RunRequestsPending);
   TEST_ASSERT_EQUAL(1000, TaskTable[2].TicksRemaining);
}

void test_RTCS_Tick_should_DecrementTicksUntilNextRelease(void)
{
   int ticks = 10;
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskA, 0, ticks));

   for (int i = 1; i < ticks; i++)
   {
      RTCS_Tick();
      TEST_ASSERT_EQUAL(ticks - i, TaskTable[0].TicksRemaining);
      TEST_ASSERT_EQUAL(0, TaskTable[i].RunRequestsPending);
   }
}

void test_RTCS_Tick_should_ResetTicksRemaining_when_TaskBecomesPending(void)
{
   int ticks = 100;
   TEST_ASSERT_EQUAL(RTCS_Status_Success, RTCS_AddTask(TaskA, 0, ticks));

   IncrementTicksBy(100);

   TEST_ASSERT_EQUAL(ticks, TaskTable[0].TicksRemaining);
   TEST_ASSERT_EQUAL(1, TaskTable[0].RunRequestsPending);
}
/**
 * End implementation specific tests.
 */

