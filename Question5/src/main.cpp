
/*
 * Question 5 - Cyclic Executive
 * Name: Chelsea Law
 *Student ID: 816044994
 * Arduino MKRZERO interface.
 * TC5 generates a hardware interrupt every 1 ms.
 * The ISR updates the system time structure.
 */

#include <Arduino.h>
#include <stdint.h>

// Time structure updated by the hardware timer ISR.
struct SystemTime
{
    volatile uint32_t total_ms;
    volatile uint32_t seconds;
    volatile uint16_t milliseconds;
};

static SystemTime system_time = {0, 0, 0};

// Functions implemented in cyclic.c
extern "C"
{
    void cyclic_init(void);
    void cyclic_run_frame(void);

    // Serial printing functions callable from C.
    void serial_print(const char *message)
    {
        Serial.print(message);
    }

    void serial_println(const char *message)
    {
        Serial.println(message);
    }

    void serial_print_number(uint32_t value)
    {
        Serial.println(value);
    }

    // Read the time maintained by the hardware ISR.
    uint32_t get_time_ms(void)
    {
        noInterrupts();
        uint32_t current = system_time.total_ms;
        interrupts();

        return current;
    }

    // Sleep using the interrupt-driven clock.
    void sleep_ms(uint32_t duration)
    {
        uint32_t start = get_time_ms();

        while ((uint32_t)(get_time_ms() - start)
               < duration)
        {
            // Wait while hardware interrupts update time.
        }
    }

    // Timer 5 interrupt service routine.
    void TC5_Handler(void)
    {
        if (TC5->COUNT16.INTFLAG.bit.MC0)
        {
            TC5->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;

            system_time.total_ms++;
            system_time.milliseconds++;

            if (system_time.milliseconds >= 1000)
            {
                system_time.milliseconds = 0;
                system_time.seconds++;
            }
        }
    }
}

// Configure TC5 to interrupt once per millisecond.
void configure_timer(void)
{
    PM->APBCMASK.reg |= PM_APBCMASK_TC5;

    GCLK->CLKCTRL.reg =
        GCLK_CLKCTRL_CLKEN |
        GCLK_CLKCTRL_GEN_GCLK0 |
        GCLK_CLKCTRL_ID_TC4_TC5;

    while (GCLK->STATUS.bit.SYNCBUSY);

    TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;

    while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

    // 48 MHz / 16 / 3000 = 1000 Hz
    TC5->COUNT16.CTRLA.reg =
        TC_CTRLA_MODE_COUNT16 |
        TC_CTRLA_WAVEGEN_MFRQ |
        TC_CTRLA_PRESCALER_DIV16;

    while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

    TC5->COUNT16.CC[0].reg = 2999;

    while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

    TC5->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
    TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC0;

    NVIC_ClearPendingIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn, 2);
    NVIC_EnableIRQ(TC5_IRQn);

    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    while (TC5->COUNT16.STATUS.bit.SYNCBUSY);
}

void setup(void)
{
    Serial.begin(9600);

    // Allow time for the USB serial connection.
    uint32_t start = millis();

    while (!Serial && (millis() - start < 5000))
    {
    }

    configure_timer();

    Serial.println("Question 5 - Cyclic Executive");
    Serial.println("Hardware timer initialized");

    cyclic_init();
}

void loop(void)
{
    cyclic_run_frame();
}
