.syntax unified
.cpu cortex-m3
.thumb

.extern scheduler_get_current
.extern scheduler_next

.global PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:

    MRS R0, PSP              // Get current task's stack pointer
    STMDB R0!, {R4-R11}      // Push software registers onto task stack
    MOV R4, R0               // Save updated SP in R4 (safe across C calls)

    PUSH {R4, LR}                // Protect LR before C calls
    BL scheduler_get_current // R0 = pointer to current TCB
    STR R4, [R0, #0]         // Save SP into TCB's sp field (offset 0)

    BL scheduler_next        // R0 = pointer to next TCB
    LDR R0, [R0, #0]         // Load next task's SP from its sp field
    POP {R4, LR}             // Restore LR

    LDMIA R0!, {R4-R11}      // Pop software registers from next task's stack
    MSR PSP, R0              // Set PSP to next task's stack pointer

    BX LR                    // Return (LR = 0xFFFFFFFD, hardware restores the rest)