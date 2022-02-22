#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/vreg.h"

const int LED = PICO_DEFAULT_LED_PIN;

int main()
{
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(133000, true);

    gpio_init(LED);

    gpio_set_dir(LED, GPIO_OUT);

    while (1)
    {
        gpio_put(LED, 1);
        sleep_ms(500);
        gpio_put(LED, 0);
        sleep_ms(500);
    }
}