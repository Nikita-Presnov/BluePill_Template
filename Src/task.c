#include "stm32f1xx.h"
#include "task.h"
#define NULL ((void *)0)

TASK_TypeDef task[TSK];

__IO uint32_t load_cpu;
__IO uint32_t current_load;

void TaskStat();

void SysTick_Handler(void)
{
    for (uint8_t i = 0; i < TSK; i++)
    {
        if (task[i].handler == NULL)
            break;

        if (!task[i].counter)
        {
            task[i].counter = task[i].period;
            task[i].handler();
            if (!task[i].loop)
                RemoveTask(i);
            break;
        }
        else
            task[i].counter--;
    }
    current_load += (72000 - SysTick->VAL);
}

void RemoveTask(uint8_t num)
{
    for (uint8_t i = num; i < (TSK - 2); i++)
    {
        task[i] = task[i + 1];
    }
    task[TSK - 1].counter = 0;
    task[TSK - 1].period = 0;
    task[TSK - 1].loop = NOLOOP;
    task[TSK - 1].handler = NULL;
}

void AddTask(void (*callback)(void),
             Task_Priority_TypeDef rank,
             Task_Loop_TypeDef loop,
             uint32_t period_ms)
{
    uint8_t i = TSK - 1;
    do
    {
        if (task[i - 1].handler != NULL)
        {
            task[i] = task[i - 1];
        }
        i--;
    } while (i);
    task[i].loop = loop;
    task[i].period = period_ms;
    task[i].handler = callback;
    task[i].counter = period_ms;
}

void ClearTask(void)
{
    for (uint8_t i = 0; i < TSK; i++)
    {
        task[i].loop = ENABLE;
        task[i].period = 0;
        task[i].counter = 0;
        task[i].handler = NULL;
    }
    load_cpu = 0;
    current_load = 0;
    AddTask(TaskStat, TOP, LOOP, 1000);
}

uint32_t GetLoadCpu()
{
    return load_cpu;
}

void TaskStat()
{
    load_cpu = current_load;
    current_load = 0;
}