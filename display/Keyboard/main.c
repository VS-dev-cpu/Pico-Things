#include "st7789.h"
#include "graphics.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/vreg.h"

const int LED_R = 6;
const int LED_G = 7;
const int LED_B = 8;

const int BUTTON_A = 12;
const int BUTTON_B = 13;
const int BUTTON_X = 14;
const int BUTTON_Y = 15;

lcd screen;
uint16_t pixels[240 * 320];
int w = 320;
int h = 240;

char abc[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '<'};
int selected = 0;
int BACKSPACE = 37;

char text[32];
int cursor = -1;

#define FLAG_VALUE 123

void core1()
{
    // Sync with First Core
    multicore_fifo_push_blocking(FLAG_VALUE);

    uint32_t g = multicore_fifo_pop_blocking();

    while (g != FLAG_VALUE)
    {
        uint32_t g = multicore_fifo_pop_blocking();
    }

    // Main UI Loop

    while (1)
    {
        if (!gpio_get(BUTTON_A))
        {
            if (selected < 37)
            {
                selected++;
                sleep_ms(200);
            }
        }
        if (!gpio_get(BUTTON_B))
        {
            if (selected > 0)
            {
                selected--;
                sleep_ms(200);
            }
        }

        if (!gpio_get(BUTTON_X))
        {
            if (selected == BACKSPACE)
            {
                cursor--;
                sleep_ms(200);
            }
            else
            {
                if (selected < sizeof(abc))
                {
                    text[cursor] = abc[selected];
                    cursor++;
                    sleep_ms(200);
                }
            }
        }
    }
}

int main()
{
    // Sync with Second Core
    multicore_launch_core1(core1);
    uint32_t g = multicore_fifo_pop_blocking();

    while (g != FLAG_VALUE)
    {
        multicore_launch_core1(core1);
        uint32_t g = multicore_fifo_pop_blocking();
    }

    multicore_fifo_push_blocking(FLAG_VALUE);

    // Set Clock

    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(133000, true);

    // Init Screen

    screen.pio = pio0;
    screen.sm = 0;
    screen.orientation = HORIZONTAL_OPPOSITE;
    screen.PIN_DIN = 19;
    screen.PIN_CLK = 18;
    screen.PIN_CS = 17;
    screen.PIN_DC = 16;
    screen.PIN_RESET = 21;
    screen.PIN_BL = 20;
    attach(&screen);

    // Init Leds and Buttons

    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);

    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);

    gpio_put(LED_R, 1);
    gpio_put(LED_G, 1);
    gpio_put(LED_B, 1);

    gpio_set_function(BUTTON_A, GPIO_FUNC_SIO);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_function(BUTTON_B, GPIO_FUNC_SIO);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_function(BUTTON_X, GPIO_FUNC_SIO);
    gpio_set_dir(BUTTON_X, GPIO_IN);
    gpio_pull_up(BUTTON_X);
    gpio_set_function(BUTTON_Y, GPIO_FUNC_SIO);
    gpio_set_dir(BUTTON_Y, GPIO_IN);
    gpio_pull_up(BUTTON_Y);

    // Main Loop

    while (1)
    {
        fill(pixels, 320 * 240, 0x0000);
        if (cursor >= 0)
            string(pixels, w, h, text, cursor, 10, 10, getStringWidth(cursor, 40), 40, RGBtoHEX(255, 255, 255), 2);
        if (selected > 0)
            character(pixels, w, h, abc[selected - 1], 320 - 30, 240 / 2 - 70, 20, 40, RGBtoHEX(150, 150, 150), 2);
        character(pixels, w, h, abc[selected], 320 - 30, 240 / 2 - 20, 20, 40, RGBtoHEX(255, 255, 255), 2);

        if (selected < 37)
            character(pixels, w, h, abc[selected + 1], 320 - 30, 240 / 2 + 30, 20, 40, RGBtoHEX(150, 150, 150), 2);

        update(&screen, pixels);
    }
}