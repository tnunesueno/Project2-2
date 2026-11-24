#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "parts.h"

typedef struct {
    int x, y;            // pixel position (top-left of its 11x11 tile)
    bool holding;
    PartType held;
} Player;

// Place the player somewhere sensible inside the grid (not on top of the
// belt or the box).
void player_init(Player* p);

// Draws the player: e.g. a white square when empty-handed, something that
// also shows the held part when carrying one.
void player_draw(Player* p);

// dx,dy expected to be -1, 0, or 1. Moves one tile if the destination is
// inside the play grid. Returns true if the player actually moved.
bool player_move(Player* p, int dx, int dy);

#endif // PLAYER_H
