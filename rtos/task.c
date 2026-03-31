#include <stdint.h>
#include "tcb.h"

void init_task_stack(TCB_t *tcb, void (*task_func)(void)) {

    // Start at top of stack (full descending stack)
    uint32_t *sp = tcb->stack_base + tcb->stack_size;

    // Ensure 8-byte alignment
    sp = (uint32_t *)((uint32_t)sp & ~0x7);

    // -------------------------
    // Hardware stack frame
    // -------------------------
    *(--sp) = 0x01000000;                        // xPSR
    *(--sp) = ((uint32_t)task_func | 1U);        // PC (Thumb bit set)
    *(--sp) = 0x00000000;                        // LR 
    *(--sp) = 0x00000000;                        // R12
    *(--sp) = 0x00000000;                        // R3
    *(--sp) = 0x00000000;                        // R2
    *(--sp) = 0x00000000;                        // R1
    *(--sp) = 0x00000000;                        // R0

    // -------------------------
    // Software stack frame
    // -------------------------
    *(--sp) = 0x00000000; // R11
    *(--sp) = 0x00000000; // R10
    *(--sp) = 0x00000000; // R9
    *(--sp) = 0x00000000; // R8
    *(--sp) = 0x00000000; // R7
    *(--sp) = 0x00000000; // R6
    *(--sp) = 0x00000000; // R5
    *(--sp) = 0x00000000; // R4

    // Save stack pointer
    tcb->sp = sp;
}
