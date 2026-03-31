#ifndef TCB_H
#define TCB_H

#include <stdint.h>

typedef enum {
    TASK_READY = 0,
    TASK_BLOCKED = 1,
    //no real need for TASK_Running as only 1 task will be running at a time
} task_state_t;

typedef struct {
    uint32_t *sp;           // Stack pointer (PSP)
    uint32_t *stack_base;   // Base of stack
    uint32_t stack_size;    // Stack size
    uint32_t priority;      // Priority
    task_state_t state;     // READY or BLOCKED
    uint32_t delay_ticks;   // Delay counter
} TCB_t;

#endif