#include "parts.h"
#include "globals.h" // myLCD + color macros (WHITE, RED, GREEN, BLUE, ORANGE, LGREY, ...)

// TODO [Base]: "At least 3 distinct parts moving around on the conveyor belt"
// Return a different 24-bit RGB color for each PartType.
int part_color(PartType t)
{
    // TODO: give each PartType its own color instead of always returning WHITE.
    return WHITE;
}

// TODO [Base]: used by the "NEED:" HUD text. Give each part a short label.
const char* part_name(PartType t)
{
    // TODO: return a short name for each PartType (e.g. "GEAR").
    return "?";
}

// TODO [Base]: give each part its own look so they're distinguishable on the
// belt and when the player is holding one. Some ideas: uLCD_filled_circle(),
// uLCD_filled_rectangle(), uLCD_line() -- combine a few calls to make a shape.
void part_draw(PartType t, int x, int y)
{
    // TODO: draw something different for each PartType.
    // Placeholder so the belt shows SOMETHING while you work on the rest:
    uLCD_filled_rectangle(&myLCD, x + 3, y + 3, x + 7, y + 7, part_color(t));
}
