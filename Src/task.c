#include "task.h"
#define NULL ((void *)0)

TASK task[TSK];

__IO uint32_t load_cpu;
__IO uint32_t current_load;

void task_stat();

void SysTick_Handler(void) {
    for(char i=0; i<TSK; i++)
    {
        if (task[i].handler == NULL) break;

        if (!task[i].counter)
        {
            task[i].counter=task[i].period;
            task[i].handler();
            if (!task[i].loop)
                remove_task(i);
            break;
        } else
            task[i].counter--;
    }
    current_load +=(72000-SysTick->VAL);
}

void remove_task(uint8_t num) {
    char i;
    for(i=num; i<(TSK-2);i++) {
        task[i]=task[i+1];
    }
    task[TSK-1].counter=0;
    task[TSK-1].period=0;
    task[TSK-1].loop=NOLOOP;
    task[TSK-1].handler=NULL;
}

void add_task(  void (*callback)(void),
                Task_Priority_TypeDef rank,
                Task_Loop_TypeDef loop,
                uint32_t period_ms)
{
    char i=TSK-1;
    do {
        if (task[i-1].handler != NULL) {
            task[i]=task[i-1];
        }
        i--;
    } while (i);
    task[i].loop=loop;
    task[i].period=period_ms;
    task[i].handler=callback;
    task[i].counter=period_ms;
}

void clear_task(void) {
    for(char i=0;i<TSK;i++) {
        task[i].loop=ENABLE;
        task[i].period=0;
        task[i].counter=0;
        task[i].handler=NULL;
    }
    load_cpu=0;
    current_load=0;
    add_task(task_stat,TOP,LOOP,1000);
}


uint32_t get_load_cpu() {
    return load_cpu;
}

void task_stat() {
    load_cpu=current_load;
    current_load=0;
}