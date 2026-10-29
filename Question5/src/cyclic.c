
/*
 * Question 5 - Cyclic Executive
 * Name: Chelsea Law
 * Student ID: 816044994
 *
 * Adapted from Rob Williams,
 * Real-Time Systems Development,
 * Chapter 4, Section 4.7.
 *
 * Strategy:
 * A table-based cyclic executive dispatches tasks
 * in a fixed sequence. Each slot lasts 5 seconds.
 * The system clock is maintained by a hardware
 * timer interrupt in main.cpp.
 */

#include <stdint.h>

#define SLOTX 4
#define CYCLEX 5
#define SLOT_T 5000UL

// Functions supplied by main.cpp
extern void serial_print(const char *message);
extern void serial_println(const char *message);
extern void serial_print_number(uint32_t value);
extern uint32_t get_time_ms(void);
extern void sleep_ms(uint32_t duration);

// Scheduling variables
static uint32_t slot_start_ms = 0;
static int cycle = 0;

// Task 1
static void one(void)
{
    serial_println("task 1 running");
    sleep_ms(1000);
}

// Task 2
static void two(void)
{
    serial_println("task 2 running");
    sleep_ms(2000);
}

// Task 3
static void three(void)
{
    serial_println("task 3 running");
    sleep_ms(3000);
}

// Task 4
static void four(void)
{
    serial_println("task 4 running");
    sleep_ms(4000);
}

// Task 5 from the original demonstration
// (defined but not selected by the task table).
void five(void)
{
    serial_println("task 5 running");
    sleep_ms(5000);
}

// Wait until the 5-second slot has elapsed.
static void burn(void)
{
    uint32_t burn_start = get_time_ms();

    uint32_t elapsed =
        get_time_ms() - slot_start_ms;

    if (elapsed < SLOT_T)
    {
        sleep_ms(SLOT_T - elapsed);
    }

    uint32_t burn_time =
        get_time_ms() - burn_start;

    serial_print("burn time = ");
    serial_print_number(burn_time);

    // Begin timing the next slot.
    slot_start_ms = get_time_ms();

    // End the current row of the task table.
    cycle = CYCLEX;
}

// Function pointer for each task
typedef void (*task_function)(void);

// Cyclic executive task table
static task_function ttable[SLOTX][CYCLEX] =
{
    {one,  two,   burn, burn, burn},
    {one,  three, burn, burn, burn},
    {one,  four,  burn, burn, burn},
    {burn, burn,  burn, burn, burn}
};

// Initialise the scheduler
void cyclic_init(void)
{
    slot_start_ms = get_time_ms();

    serial_println("Cyclic executive initialized");
    serial_println("Slot duration = 5000 ms");
}

// Execute one complete cycle of four slots.
void cyclic_run_frame(void)
{
    for (int slot = 0; slot < SLOTX; slot++)
    {
        for (cycle = 0; cycle < CYCLEX; cycle++)
        {
            ttable[slot][cycle]();
        }
    }
}
