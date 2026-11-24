#include "player.h"
#include "layout.h"
#include "globals.h"

// TODO [Base]: "Create a sprite for the player". Place the player at a
// reasonable starting tile inside the 9x9 grid (CELL_X(col)/CELL_Y(row) from
// layout.h give you pixel coordinates for a given column/row).
void player_init(Player* p)
{
    p->x = CELL_X(1); // TODO: pick a real starting tile
    p->y = CELL_Y(1);
    p->holding = false;
    p->held = PART_GEAR;
}

// TODO [Base]: "Nav Switch moves the player in all directions". Move one
// tile in the given direction if the destination is inside the play grid
// (CELL_X(0)..CELL_X(GRID_W-1), CELL_Y(0)..CELL_Y(GRID_H-1)). Return whether
// the move actually happened.
bool player_move(Player* p, int dx, int dy)
{
    // TODO: implement.
    return false;
}

// TODO [Base]: draw the player. Consider showing a different look when
// p->holding is true (e.g. draw the held part on top using part_draw()).
void player_draw(Player* p)
{
    // TODO: implement.
    uLCD_filled_rectangle(&myLCD, p->x, p->y, p->x + TILE - 1, p->y + TILE - 1, WHITE);
}
