#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "font.h"

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

uint16_t RGBtoHEX(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0b11111000) << 8) |
           ((g & 0b11111100) << 3) |
           ((b & 0b11111000) >> 3);
}

void fill(uint16_t image[], int size, uint32_t color)
{
    for (int i = 0; i < size; i++)
    {
        image[i] = color;
    }
}

void pixel(uint16_t image[], int screen_w, int screen_h, int x, int y, uint16_t color)
{
    if (0 <= x && x < screen_w && 0 <= y && y < screen_h)
        image[x + y * screen_w] = color;
}

void pixel_s(uint16_t image[], int screen_w, int screen_h, int x, int y, uint16_t color, int size)
{
    if (0 <= x && x < screen_w && 0 <= y && y < screen_h)
    {
        if (size == 1)
            image[x + y * screen_w] = color;
        if (size == 2)
        {
            image[x + y * screen_w] = color;
            image[x + 1 + y * screen_w] = color;
            image[x - 1 + y * screen_w] = color;
            image[x + y * screen_w + 1] = color;
            image[x + y * screen_w - 1] = color;
        }
    }
}

void line(uint16_t image[], int screen_w, int screen_h, int x1, int y1, int x2, int y2, uint16_t color, int size)
{
    // fast horizontal line
    if (y1 == y2)
    {
        int32_t start = min(x1, x2);
        int32_t end = max(x1, x2);

        for (int i = start; i < end; i++)
        {
            pixel_s(image, screen_w, screen_h, i, y1, color, size);
        }
        return;
    }

    // fast vertical line
    if (x1 == x2)
    {
        int32_t start = min(y1, y2);
        int32_t end = max(y1, y2);
        for (int i = start; i < end; i++)
        {
            pixel_s(image, screen_w, screen_h, x1, i, color, size);
        }
        return;
    }

    // general purpose line
    // lines are either "shallow" or "steep" based on whether the x delta
    // is greater than the y delta
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    bool shallow = abs(dx) > abs(dy);
    if (shallow)
    {
        // shallow version
        int32_t s = abs(dx);          // number of steps
        int32_t sx = dx < 0 ? -1 : 1; // x step value
        int32_t sy = (dy << 16) / s;  // y step value in fixed 16:16
        int32_t x = x1;
        int32_t y = y1 << 16;
        while (s--)
        {
            pixel_s(image, screen_w, screen_h, x, (y >> 16), color, size);
            y += sy;
            x += sx;
        }
    }
    else
    {
        // steep version
        int32_t s = abs(dy);          // number of steps
        int32_t sy = dy < 0 ? -1 : 1; // y step value
        int32_t sx = (dx << 16) / s;  // x step value in fixed 16:16
        int32_t y = y1;
        int32_t x = x1 << 16;
        while (s--)
        {
            pixel_s(image, screen_w, screen_h, (x >> 16), y, color, size);
            y += sy;
            x += sx;
        }
    }
}

void rect(uint16_t image[], int screen_w, int screen_h, int x, int y, int w, int h, uint16_t color)
{
    for (int yy = y; yy < y + h; yy++)
    {
        for (int xx = x; xx < x + w; xx++)
        {
            pixel(image, screen_w, screen_h, xx, yy, color);
        }
    }
}

void texture(uint16_t image[], int screen_w, int screen_h, int x, int y, int w, int h, uint16_t texture[], int texture_w, int texture_h)
{
    float x_step = texture_w / w;
    float y_step = texture_h / h;
    for (int xx = 0; xx < w; xx++)
    {
        for (int yy = 0; yy < h; yy++)
        {
            pixel(image, screen_w, screen_h, x + xx, y + yy, texture[(int)(x_step * xx) + (int)(y_step * yy) * texture_w]);
        }
    }
}

void character(uint16_t image[], int screen_w, int screen_h, char in_char, int x, int y, int w, int h, uint16_t color, int size)
{
    float *current;
    int items;
    switch (in_char)
    {
        // LETTERS
    case 'A':
        current = font_A;
        break;
    case 'B':
        current = font_B;
        break;
    case 'C':
        current = font_C;
        break;
    case 'D':
        current = font_D;
        break;
    case 'E':
        current = font_E;
        break;
    case 'F':
        current = font_F;
        break;
    case 'G':
        current = font_G;
        break;
    case 'H':
        current = font_H;
        break;
    case 'I':
        current = font_I;
        break;
    case 'J':
        current = font_J;
        break;
    case 'K':
        current = font_K;
        break;
    case 'L':
        current = font_L;
        break;
    case 'M':
        current = font_M;
        break;
    case 'N':
        current = font_N;
        break;
    case 'O':
        current = font_O;
        break;
    case 'P':
        current = font_P;
        break;
    case 'Q':
        current = font_Q;
        break;
    case 'R':
        current = font_R;
        break;
    case 'S':
        current = font_S;
        break;
    case 'T':
        current = font_T;
        break;
    case 'U':
        current = font_U;
        break;
    case 'V':
        current = font_V;
        break;
    case 'W':
        current = font_W;
        break;
    case 'X':
        current = font_X;
        break;
    case 'Y':
        current = font_Y;
        break;
    case 'Z':
        current = font_Z;
        break;

        // NUMBERS
    case '0':
        current = font_0;
        break;
    case '1':
        current = font_1;
        break;
    case '2':
        current = font_2;
        break;
    case '3':
        current = font_3;
        break;
    case '4':
        current = font_4;
        break;
    case '5':
        current = font_5;
        break;
    case '6':
        current = font_6;
        break;
    case '7':
        current = font_7;
        break;
    case '8':
        current = font_8;
        break;
    case '9':
        current = font_9;
        break;

    default:
        return;
        break;
    }

    items = current[0];

    for (int i = 0; i < items; i++)
    {
        int x1 = x + current[1 + 4 * i] * w;
        int y1 = y + current[2 + 4 * i] * h;
        int x2 = x + current[3 + 4 * i] * w;
        int y2 = y + current[4 + 4 * i] * h;

        line(image, screen_w, screen_h, x1, y1, x2, y2, color, size);
    }
}

void string(uint16_t image[], int screen_w, int screen_h, char *text, int text_len, int x, int y, int w, int h, uint16_t color, int size)
{
    int ls = w / text_len;

    for (int i = 0; i < text_len; i++)
    {
        character(image, screen_w, screen_h, text[i], x + ls * i + i * LETTER_CLEARENCE, y, ls, h, color, size);
    }
}

int getStringWidth(int text_len, int h)
{
    return text_len * (h / 2);
}