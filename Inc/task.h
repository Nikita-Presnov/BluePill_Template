#ifndef __TASK_H__
#define __TASK_H__
// #include <stddef.h>
#include <stdint.h>
#define TSK 5 // count of task

typedef enum
{
    TOP = ((uint8_t)0x00),
    BOTTOM = ((uint8_t)0x01)
} Task_Priority_TypeDef;

typedef enum
{
    NOLOOP = ((uint8_t)0x00),
    LOOP = ((uint8_t)0x01)
} Task_Loop_TypeDef;

uint32_t GetLoadCpu();
void TaskDelay(int ms);
void StartLoop(void);
void RemoveTask(uint8_t num);
void ClearTask(void);
void AddTask(void (*callback)(void), Task_Priority_TypeDef rank, Task_Loop_TypeDef loop, uint32_t period_ms);

#endif /* __TASK_H__ */
