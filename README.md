# CortexRTOS

A minimal preemptive RTOS built from scratch for the STM32F103 (Cortex-M3), using bare-metal C and CMSIS.

## What this is

This project is a learning-focused implementation of a basic RTOS kernel.
The goal is to understand how preemptive multitasking actually works on Cortex-M microcontrollers — not just use a library like FreeRTOS.

Everything is written from scratch:

* No HAL
* No external RTOS
* Direct register-level control

---

## Planned Features

* SysTick-based system tick
* PendSV-based context switching
* Separate stack per task (PSP)
* Basic task control block (TCB)
* Task delay (blocking with tick count)
* Preemptive Sccheduling

---

## What is NOT implemented (yet)

* Dynamic memory allocation
* Queues / semaphores
* Dynamic task creation
* Advanced scheduling



---

## How it works (brief)

* SysTick fires periodically and updates the system tick
* It triggers PendSV when a context switch is needed
* PendSV saves the current task context and restores the next one
* Tasks run in thread mode using PSP, interrupts run in handler mode using MSP

---

## Hardware

* STM32F103C8T6 (Blue Pill)
* Cortex-M3 @ 72 MHz

---

## Build

Requires:

* `arm-none-eabi-gcc`
* Make
```bash
make
```
This generates:

```
build/main.elf
```
---

## Status

Built a working kernel, but with a Round robin scheduler instead on priority based.
28/6: Implemented Preemptive Scheduling

---

##  Demo

- Task 1 → LED blink @ 500 ms  
- Task 2 → LED blink @ 1000 ms  
- Task 3 → LED blink @ 2000 ms  

<p align="center">
  <img src="https://github.com/user-attachments/assets/03b664e8-ff61-4c0c-bf6b-c59763de8496" width="500"/>
</p>

