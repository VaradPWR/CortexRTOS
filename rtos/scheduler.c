#include "scheduler.h"
#include <stddef.h>

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

//round robin to preemptive scheduler change
TCB_t *scheduler_next(void) {
    // 1. Find the highest priority level (lowest number) among all READY tasks
    uint32_t highest_prio = 0xFFFFFFFF;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i]->state == TASK_READY && tasks[i]->priority < highest_prio) {
            highest_prio = tasks[i]->priority;
        }
    }

    if (highest_prio == 0xFFFFFFFF) {
        return NULL; // Should never happen if idle task is always READY
    }

    // 2. Find the next READY task with this priority, starting from current + 1
    // This ensures fair time-slicing (Round-Robin) for tasks of the SAME priority
    int start_idx = (current_task_idx + 1) % task_count;
    int checked = 0;
    
    while (checked < task_count) {
        if (tasks[start_idx]->state == TASK_READY && tasks[start_idx]->priority == highest_prio) {
            current_task_idx = start_idx; // Update the global index
            return tasks[start_idx];
        }
        start_idx = (start_idx + 1) % task_count;
        checked++;
    }

    return NULL;
}

int scheduler_tick(void) {
    int switch_needed = 0;
    TCB_t *current = scheduler_get_current();

    for (int i = 0; i < task_count; i++) {
        if (tasks[i]->state == TASK_BLOCKED) {
            if (tasks[i]->delay_ticks > 0) {
                tasks[i]->delay_ticks--;
                
                if (tasks[i]->delay_ticks == 0) {
                    tasks[i]->state = TASK_READY;
                    
                    // UPGRADED PREEMPTION LOGIC:
                    // If the newly unblocked task has a HIGHER priority than the current task, we trigger a context switch
                    if (tasks[i]->priority <= current->priority) {
                        switch_needed = 1;
                    }
                }
            }
        }
    }
    return switch_needed;
}