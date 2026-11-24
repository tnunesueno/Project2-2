#ifndef BOXES_H
#define BOXES_H

#include <stdbool.h>
#include "parts.h"

// Base points only require "at least one" -- bump this (up to 4) to add more
// stations; they stack vertically in the middle of the play field.
#define NUM_BOXES 1

#define DELIVERY_REWARD 10   // money earned per correct delivery
#define FLASH_FRAMES    6    // how long a box flashes green/red after a delivery attempt

typedef struct {
    int x, y;             // pixel position (top-left of its tile)
    PartType requested;   // part currently needed to fill this box
    int flash_frames;     // >0 while the box is flashing
    int flash_color;      // GREEN = accepted, RED = wrong part
    bool dirty;           // needs to be redrawn
} Box;

typedef struct {
    Box boxes[NUM_BOXES];
    int score;            // correct deliveries
    int money;            // +DELIVERY_REWARD per delivery, minus trash penalties (if you add those)
} BoxLine;

void boxes_init(BoxLine* bl);

// Redraws any box that changed since last frame. Returns true if it drew anything.
bool boxes_draw(BoxLine* bl);

// Unconditionally redraws box i (e.g. after the player walks off it).
void boxes_draw_box(BoxLine* bl, int i);

// Index of the box whose tile is exactly (x, y), or -1.
int boxes_at(BoxLine* bl, int x, int y);

// Index of a box that the tile (x, y) is on or directly next to (up/down/
// left/right), or -1. The player must be here to deliver.
int boxes_adjacent(BoxLine* bl, int x, int y);

// The player tries to deliver `held` to box i.
// Correct part: +1 score, +money, green flash, new random request -> true.
// Wrong part: red flash, nothing else changes -> false.
bool boxes_try_deliver(BoxLine* bl, int i, PartType held);

#endif // BOXES_H
