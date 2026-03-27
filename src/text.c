#include "../include/text.h"

//to draw text in center
void DrawTextC(const char *text, int y, int size, Color color)
{
    int x = SCREEN_W/2 - MeasureText(text, size)/2;
    DrawText(text, x, y, size, color);
}

// same as DrawTextC but with shadow
void DrawTextCShadow(const char *text, int y, int size, Color color)
{
    int x = SCREEN_W/2 - MeasureText(text, size)/2;

    DrawText(text, x + 2, y + 2, size, BLACK); // shadow
    DrawText(text, x, y, size, color);         // main text
}