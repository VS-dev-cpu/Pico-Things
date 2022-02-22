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

void the1dgame()
{
    gpio_put(LED_R, 1);
    gpio_put(LED_G, 1);
    gpio_put(LED_B, 1);
    while (1)
    {
        int w = 240;
        int h = 320;

        int pos = 0;
        int speed = 10;
        int dir = speed;

        int player_health = 3;
        int enemy_health = 3;
        int score = 0;

        int tick = 0;

        while (1)
        {
            fill(pixels, w * h, RGBtoHEX(0, 0, 255));                                 // Background
            rect(pixels, w, h, w / 3, 0, w / 3, h, RGBtoHEX(0, 0, 150));              // Path
            rect(pixels, w, h, w / 3, pos, w / 3, 20, RGBtoHEX(255, 0, 0));           // Enemy
            rect(pixels, w, h, w / 3, h / 2 - 10, w / 3, 20, RGBtoHEX(255, 53, 184)); // Line

            for (int i = 0; i < player_health; i++)
            {
                rect(pixels, w, h, w / 3 * 2 + 240 / 6, i * 40 + 20, 20, 20, RGBtoHEX(0, 255, 0));
            }

            for (int i = 0; i < enemy_health; i++)
            {
                rect(pixels, w, h, w / 3 * 2 + 240 / 6, h - (i * 40 + 40), 20, 20, RGBtoHEX(255, 0, 0));
            }

            for (int i = 0; i < score; i++)
            {
                rect(pixels, w, h, 20, (i * 40 + 20), 20, 20, RGBtoHEX(255, 53, 184));
            }

            if (!gpio_get(BUTTON_A))
            {
                if (pos < h / 2 + 20 && pos > h / 2 - 20)
                {
                    enemy_health--;
                    if (score >= 10)
                    {
                        score -= 5;
                    }
                }
                else
                {
                    player_health--;
                }
                sleep_ms(50);
            }

            if (enemy_health <= 0)
            {
                if (score >= 10)
                    break;
                enemy_health = 3;
                score++;
                pos = 0;
            }

            if (player_health <= 0)
            {
                if (score < 10)
                {
                    player_health = 3;
                    score -= 2;
                    pos = 0;
                }
                else
                {
                    player_health = 3;
                    pos = 0;
                    if (enemy_health != 3)
                    {
                        enemy_health++;
                        score += 5;
                    }
                }
            }

            update(&screen, pixels);

            if (score < 8)
            {
                pos += dir;

                if (pos < 0)
                    dir = speed;
                if (pos > 300)
                    dir = -speed;
            }
            else if (score == 8)
            {
                score = 25;
                pos = 0;
            }
            else if (score >= 10)
            {
                if (tick % score == 0)
                {
                    pos = rand() % 300;
                }
            }
            tick++;
        }
        sleep_ms(500);
        while (1)
        {
            fill(pixels, 320 * 240, RGBtoHEX(rand() % 255, rand() % 255, rand() % 255));
            gpio_put(LED_R, rand() % 2 - 1);
            gpio_put(LED_G, rand() % 2 - 1);
            gpio_put(LED_B, rand() % 2 - 1);
            update(&screen, pixels);

            if (!gpio_get(BUTTON_A))
                break;
        }
        sleep_ms(500);
    }
}

int main()
{
    // vreg_set_voltage(VREG_VOLTAGE_1_30);
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

    the1dgame();
}