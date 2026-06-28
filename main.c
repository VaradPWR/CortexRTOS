#include <stdint.h>
#include "device.h"
#include "core_cm3.h"
#include "tcb.h"
#include "scheduler.h"

void systick_init(void);
void init_task_stack(TCB_t *tcb, void (*task_func)(void));
void task_delay(uint32_t ticks);

#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021018)
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_ODR     (*(volatile uint32_t*)0x4001100C)

#define GPIOB_CRH     (*(volatile uint32_t*)0x40010C04)
#define GPIOB_ODR     (*(volatile uint32_t*)0x40010C0C)


void gpio_init(void) {
    RCC_APB2ENR |= (1 << 4);
    
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);
    
    RCC_APB2ENR |= (1 << 3);
    
    GPIOB_CRH &= ~(0xF << 20);
	GPIOB_CRH |=  (0x2 << 20); 
	
	GPIOB_CRH &= ~(0xF << 16);
	GPIOB_CRH |=  (0x2 << 16);
    
}

uint32_t task1_stack[128];
uint32_t task2_stack[128];
uint32_t task3_stack[128];
uint32_t idle_stack[64];
TCB_t tcb1, tcb2, tcb3, tcb_idle;

// Toggles LED every 1000ms
void task1(void) {
    while (1) {
        GPIOC_ODR ^= (1 << 13);
        task_delay(1000);
    }
}

// Increments a counter every 500ms
volatile uint32_t task2_count = 0;
void task2(void) {
    while (1) {
        task2_count++; //used for debugging
        GPIOB_ODR ^= (1<<13);
        task_delay(500);
    }
}

// Increments a counter every 1500ms
volatile uint32_t task3_count = 0;
void task3(void) {
    while (1) {
        task3_count++;
        GPIOB_ODR ^= (1<<12);
        task_delay(2000);
    }
}

void idle_task(void) {
    while(1);
}

int main(void) {

    systick_init();
    gpio_init();

    tcb1.stack_base = task1_stack;
    tcb1.stack_size = 128;
    tcb1.priority   = 0;
    tcb1.state      = TASK_READY;
    tcb1.delay_ticks = 0;
    init_task_stack(&tcb1, task1);
    scheduler_add_task(&tcb1);

    tcb2.stack_base = task2_stack;
    tcb2.stack_size = 128;
    tcb2.priority   = 0;
    tcb2.state      = TASK_READY;
    tcb2.delay_ticks = 0;
    init_task_stack(&tcb2, task2);
    scheduler_add_task(&tcb2);

    tcb3.stack_base = task3_stack;
    tcb3.stack_size = 128;
    tcb3.priority   = 0;
    tcb3.state      = TASK_READY;
    tcb3.delay_ticks = 0;
    init_task_stack(&tcb3, task3);
    scheduler_add_task(&tcb3);

    tcb_idle.stack_base = idle_stack;
    tcb_idle.stack_size = 64;
    tcb_idle.priority   = 255;
    tcb_idle.state      = TASK_READY;
    tcb_idle.delay_ticks = 0;
    init_task_stack(&tcb_idle, idle_task);
    scheduler_add_task(&tcb_idle);

    TCB_t *first = scheduler_get_current();

    __asm volatile(
        "MOV R0, %0       \n"
        "MSR PSP, R0      \n"
        "MOV R0, #2       \n"
        "MSR CONTROL, R0  \n"
        "ISB              \n"
        "POP {R4-R11}     \n"
        "POP {R0-R3}      \n"
        "POP {R12}        \n"
        "POP {LR}         \n"
        "POP {PC}         \n"
        :
        : "r"(first->sp)
    ); //this blocks the main thread and starts the first task. The first task will never return, so we don't need to worry about returning to main.

    while(1);
}
