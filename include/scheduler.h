#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "tcb.h"

#define MAX_TASKS 8

void    scheduler_add_task(TCB_t *tcb);
TCB_t  *scheduler_get_current(void);
TCB_t  *scheduler_next(void);
void    scheduler_tick(void);

#endif