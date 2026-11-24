#ifndef PARTS_H
#define PARTS_H

// The 3+ distinct parts required by the base points. Add more here and
// PART_COUNT keeps everything else (spawning, box requests) in sync.
typedef enum {
    PART_GEAR = 0,
    PART_SPRING,
    PART_BOLT,
    PART_COUNT
} PartType;

// 24-bit RGB color used to draw this part (passed straight into uLCD calls).
int part_color(PartType t);

// Short label used for the "NEED:" text.
const char* part_name(PartType t);

// Draws the part's icon inside the 11x11 tile whose top-left is (x, y).
// Each part should have its own shape as well as its own color, so they're
// easy to tell apart on the small screen. Does NOT need to clear the tile
// first -- the caller already drew the background.
void part_draw(PartType t, int x, int y);

#endif // PARTS_H
