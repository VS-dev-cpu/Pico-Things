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

char abc[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

void pong()
{
    while (1)
    {
        gpio_put(LED_R, 1);
        gpio_put(LED_G, 1);
        gpio_put(LED_B, 1);

        int w = 240;
        int h = 320;

        int pos_x = w / 2 - 10;
        int pos_y = h / 2 - 10;
        int speed = 2;
        int dir_x = speed;
        int dir_y = speed;

        int right_pos = w / 2 - 20;
        int left_pos = w / 2 - 20;

        while (1)
        {
            fill(pixels, w * h, RGBtoHEX(0, 0, 150)); // Background

            rect(pixels, w, h, pos_x, pos_y, 20, 20, RGBtoHEX(255, 140, 0));

            line(pixels, 240, 320, pos_x + 10, pos_y + 10, pos_x + 10 + -(dir_x * 200), pos_y + 10 + -(dir_y * 200), RGBtoHEX(255, 0, 0), 2);
            string(pixels, w, h, "ASDF CHICKEN", 13, 10, 10, getStringWidth(13, 20), 20, RGBtoHEX(255, 255, 255), 1);

            rect(pixels, w, h, left_pos, 20, 80, 10, RGBtoHEX(255, 255, 255));
            rect(pixels, w, h, right_pos, 290, 80, 10, RGBtoHEX(255, 255, 255));

            if (!gpio_get(BUTTON_A))
                if (left_pos < 240 - 80)
                    left_pos += 10;
            if (!gpio_get(BUTTON_B))
                if (left_pos > 0)
                    left_pos -= 10;

            if (!gpio_get(BUTTON_X))
                if (right_pos < 240 - 80)
                    right_pos += 10;
            if (!gpio_get(BUTTON_Y))
                if (right_pos > 0)
                    right_pos -= 10;

            update(&screen, pixels);

            pos_x += dir_x;
            pos_y += dir_y;

            if (pos_x < 0)
                dir_x = speed;
            if (pos_x > 220)
                dir_x = -speed;

            if (pos_y < 0)
                dir_y = speed;
            if (pos_y > 300)
                dir_y = -speed;

            if (pos_x > left_pos && pos_x < left_pos + 80 && pos_y < 30)
                dir_y = speed;

            if (pos_x > right_pos && pos_x < right_pos + 80 && pos_y > 270)
                dir_y = -speed;

            pos_x += dir_x;
            pos_y += dir_y;
        }
    }
}

void test()
{
    while (1)
    {
        fill(pixels, 320 * 240, RGBtoHEX(0, 0, 150)); // Background

        rect(pixels, 320, 240, 10, 10, 20, 20, RGBtoHEX(255, 140, 0));

        update(&screen, pixels);
    }
}

int main()
{
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(133000, true);
    screen.pio = pio0;
    screen.sm = 0;
    screen.orientation = VERTICAL;
    screen.PIN_DIN = 19;
    screen.PIN_CLK = 18;
    screen.PIN_CS = 17;
    screen.PIN_DC = 16;
    screen.PIN_RESET = 21;
    screen.PIN_BL = 20;
    attach(&screen);

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

    pong();
}