// ============================================
// The Graphics class file -- ESP-IDF port.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"

#include "globals.h"
#include <stdlib.h> // abs()

void draw_nothing(int u, int v)
{
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = 0xFFFFFF; // white
        else if (img[i] == 'Z') colors[i] = BLACK;
        else if (img[i] == 'P') colors[i] = 0x800080; // purple
        else colors[i] = BLACK;
    }
    uLCD_BLIT(&myLCD, u, v, 11, 11, colors);
    ets_delay_us(250); // Recovery time!
}

// For each sprite, there is an array of 121 characters describing the color of each pixel of the tile where the sprite is drawn
// Characters describe colors according to draw_img
// Feel free to add more!
void draw_wall(int u, int v)
{
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, 0x8e8e8e8e);
}

void draw_nightvision(int u, int v)
{
    // TODO: Implement
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, WHITE);
}

void draw_explorer(int u, int v)
{
    // TODO: Implement
    // This is the only given sprite: it is intended to show how you can make one yourself
    // Feel free to overwrite this one with your own explorer!
    char explorer_sprite[122] = "ZZWWWWWWWZZZZWWZWZWWZZZZWWWWWWWZZZZWZWWWZWZZZZWWZZZWWZZZZZZWWWZZZZWWWWWWWWWWWWWWWWWWWWWWZZZZWWWZZZZWWWWWZWWWWWWWWWWZWWWWW";
    draw_img(u, v, explorer_sprite);
}

void draw_heal(int u, int v)
{
    // TODO: Implement
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, BLUE);
}

void draw_poison(int u, int v)
{
    // TODO: Implement
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, RED);
}

void draw_teleporter(int u, int v)
{
    // TODO: Implement
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, 0x800080); // Purple color
}

// Provided: calculate which sprite to draw based on the screen position
// Use this in draw_game()
void draw_dark_outline(int u, int v)
{
    // Convert screen coordinates back to relative tile position
    int i = (u - 3) / 11 - 5;  // Calculate i from u
    int j = (v - 15) / 11 - 4; // Calculate j from v

    // Fill the entire tile with dark color first
    uLCD_filled_rectangle(&myLCD, u, v, u+10, v+10, BLACK);

    // Determine which side is closest to the center and draw orange line
    if (abs(i) > abs(j)) {
        // Horizontal edge is closest to center
        if (i < 0) {
            // Right edge (orange line on right)
            uLCD_filled_rectangle(&myLCD, u+10, v, u+10, v+10, ORANGE);  // orange line on right edge
        } else {
            // Left edge (orange line on left)
            uLCD_filled_rectangle(&myLCD, u, v, u, v+10, ORANGE);  // orange line on left edge
        }
    } else if (abs(i) != abs(j)) {
        // Vertical edge is closest to center
        if (j < 0) {
            // Bottom edge (orange line on bottom)
            uLCD_filled_rectangle(&myLCD, u, v+10, u+10, v+10, ORANGE);  // orange line on bottom edge
        } else {
            // Top edge (orange line on top)
            uLCD_filled_rectangle(&myLCD, u, v, u+10, v, ORANGE);  // orange line on top edge
        }
    }
}