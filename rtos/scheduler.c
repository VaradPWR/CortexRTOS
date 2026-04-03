#include "scheduler.h"

static TCB_t *tasks[MAX_TASKS];
static int task_count = 0;
static int current_task_idx = 0;

void scheduler_add_task(TCB_t *tcb) {
    if (task_count < MAX_TASKS) {
        tasks[task_count++] = tcb;
    }
}

TCB_t *scheduler_get_current(void) {
    return tasks[current_task_idx];
}

TCB_t *scheduler_next(void) {
    int checked = 0;
 
    while (checked < task_count) {
        // Advance to next task (wrap around)
        current_task_idx = (current_task_idx + 1) % task_count;
 
        if (tasks[current_task_idx]->state == TASK_READY) {
            return tasks[current_task_idx];
        }
 
        checked++;
    }
 
    // No ready task found
    return NULL;
}

void scheduler_tick(void) {
    for (int i = 0; i < task_count; i++) {
        if (tasks[i]->state == TASK_BLOCKED) {
            if (tasks[i]->delay_ticks > 0) {
                tasks[i]->delay_ticks--;
 
                if (tasks[i]->delay_ticks == 0) {
                    tasks[i]->state = TASK_READY;
                }
            }
        }
    }
}