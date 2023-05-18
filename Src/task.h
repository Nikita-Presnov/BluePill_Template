#ifndef __TASK_H__
#define __TASK_H__
// #include <stddef.h>
#define TSK 5 // count of task

typedef struct TASK_TypeDef
{
    uint32_t loop;
    uint32_t period;
    uint32_t counter;
    void (*handler)(void);
} TASK_TypeDef;

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
void RemoveTask(uint8_t num);
void ClearTask(void);
void AddTask(void (*callback)(void), Task_Priority_TypeDef rank, Task_Loop_TypeDef loop, uint32_t period_ms);

#endif /* __TASK_H__ */
