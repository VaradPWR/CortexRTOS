#include <stdint.h>
#include "device.h"
#include "core_cm3.h"
#include "scheduler.h"

#define GPIOC_ODR (*(volatile uint32_t*)0x4001100C)

volatile uint32_t tick_count = 0;

void systick_init(uint32_t tick_hz){
    #define SYSCLK_HZ 8000000UL  //replace this thing with 72 later when rcc is configured.
    SysTick->LOAD = (SYSCLK_HZ/tick_hz) -1;
    SysTick->VAL = 0;
    SysTick->CTRL =  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    NVIC_SetPriority(PendSV_IRQn, 0xFF); // setting pendsv to lowest priority , shouldnt interfere with any other interrupts
}

void SysTick_Handler(){
    tick_count++;
    scheduler_tick();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

}
