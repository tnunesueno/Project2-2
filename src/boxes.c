#include <assert.h>
#include "boxes.h"
#include "layout.h"
#include "globals.h"
#include <stdlib.h>

#define BOX_COLOR 0xD2691E  // cardboard brown

static_assert(NUM_BOXES >= 1 && NUM_BOXES <= 4, "boxes must fit in the middle of the grid");

// TODO [Base]: "At least one boxing station" + "Randomizing box item and
// update NEED text". Positions are GIVEN below; you need to give each box a
// random initial requested part (instead of always PART_GEAR).
void boxes_init(BoxLine* bl)
{
    bl->score = 0;
    bl->money = 0;
    for (int i = 0; i < NUM_BOXES; i++) {
        Box* b = &bl->boxes[i];
        // GIVEN: center column; boxes are spaced two rows apart around the center row.
        b->x = CELL_X(GRID_W / 2);
        b->y = CELL_Y(GRID_H / 2 - (NUM_BOXES - 1) + 2 * i);
        b->requested = PART_GEAR; // TODO: pick a random PartType instead
        b->flash_frames = 0;
        b->flash_color = GREEN;
        b->dirty = true;
    }
}

// GIVEN -- draws one box with its border (flashing green/red briefly after a
// delivery attempt) and the part it's requesting inside.
void boxes_draw_box(BoxLine* bl, int i)
{
    Box* b = &bl->boxes[i];
    int border = b->flash_frames > 0 ? b->flash_color : BOX_COLOR;
    uLCD_filled_rectangle(&myLCD, b->x, b->y, b->x + TILE - 1, b->y + TILE - 1, border);
    uLCD_filled_rectangle(&myLCD, b->x + 1, b->y + 1, b->x + TILE - 2, b->y + TILE - 2, BLACK);
    part_draw(b->requested, b->x, b->y);   // show which part this box wants
    b->dirty = false;
}

// GIVEN.
bool boxes_draw(BoxLine* bl)
{
    bool drew = false;
    for (int i = 0; i < NUM_BOXES; i++) {
        Box* b = &bl->boxes[i];
        if (b->dirty) {
            boxes_draw_box(bl, i);
            drew = true;
        }
        if (b->flash_frames > 0 && --b->flash_frames == 0) {
            b->dirty = true; // flash over: redraw in normal colors next frame
        }
    }
    return drew;
}

// GIVEN.
int boxes_at(BoxLine* bl, int x, int y)
{
    for (int i = 0; i < NUM_BOXES; i++) {
        if (bl->boxes[i].x == x && bl->boxes[i].y == y) return i;
    }
    return -1;
}

// GIVEN.
int boxes_adjacent(BoxLine* bl, int x, int y)
{
    for (int i = 0; i < NUM_BOXES; i++) {
        int dx = abs(bl->boxes[i].x - x);
        int dy = abs(bl->boxes[i].y - y);
        if (dx + dy <= TILE) return i;   // same tile, or one tile away (not diagonal)
    }
    return -1;
}

// TODO [Base]: "A visible score or delivered-item counter that changes as
// actions are completed". Correct part: +1 score, add DELIVERY_REWARD money,
// flash green, pick a new random request, return true. Wrong part: flash
// red, return false (nothing else changes). b->flash_frames/b->dirty are
// already shown below as a starting point.
bool boxes_try_deliver(BoxLine* bl, int i, PartType held)
{
    if (i < 0 || i >= NUM_BOXES) return false;
    Box* b = &bl->boxes[i];
    b->flash_frames = FLASH_FRAMES;
    b->dirty = true;
    // TODO: check held against b->requested and implement both outcomes.
    b->flash_color = RED;
    return false;
}
