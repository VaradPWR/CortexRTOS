#include <stdint.h>
#include "device.h"
#include "core_cm3.h"
#include "tcb.h"

void systick_init(uint32_t tick_hz);

void init_task_stack(TCB_t *tcb, void (*task_func)(void));

// ----------- GPIO (PC13 LED) -----------
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021018)
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_ODR     (*(volatile uint32_t*)0x4001100C)

void gpio_init(void) {
    // Enable GPIOC clock
    RCC_APB2ENR |= (1 << 4);

    // Configure PC13 as output (2 MHz push-pull)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);
}

// ----------- Delay -----------
void delay(volatile uint32_t count) {
    while (count--) {
        __asm volatile("nop");
    }
}

// ----------- Globals -----------
uint32_t task1_stack[128];
TCB_t tcb1;

// ----------- Task -----------
void task1(void) {
    while (1) {
        GPIOC_ODR ^= (1 << 13); 
        delay(500000);
    }
}

// ----------- Main -----------
int main(void) {

    systick_init(1000);
    gpio_init();   // init LED

    // Setup TCB
    tcb1.stack_base = task1_stack;
    tcb1.stack_size = 128;
    tcb1.priority = 0;
    tcb1.state = TASK_READY;
    tcb1.delay_ticks = 0;

    // Initialize task stack
    init_task_stack(&tcb1, task1);

    // Start first task (manual context restore)
    __asm volatile(
        "MOV R0, %0       \n"   // R0 = tcb1.sp
        "MSR PSP, R0      \n"

        "MOV R0, #2       \n"
        "MSR CONTROL, R0  \n"
        "ISB              \n"

        // Restore full context
        "POP {R4-R11}     \n"
        "POP {R0-R3}      \n"
        "POP {R12}        \n"
        "POP {LR}         \n"
        "POP {PC}         \n"

        :
        : "r"(tcb1.sp)
    );

    // Should never reach here
    while (1);
}
