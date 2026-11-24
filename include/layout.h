#ifndef LAYOUT_H
#define LAYOUT_H

// Shared screen layout. The play field is a GRID_W x GRID_H grid of 11x11
// tiles. The conveyor belt runs around the grid's outer ring; the box station
// sits in the middle; the player can walk anywhere on the grid.
//
//   y 0..7     : text row 0  (money / score / belt fill)
//   y 15..113  : the 9x9 tile grid
//   y 120..127 : text row 15 (requested part / PAUSE)

#define TILE      11
#define GRID_W    9
#define GRID_H    9
#define ORIGIN_X  5
#define ORIGIN_Y  15

#define CELL_X(col) (ORIGIN_X + (col) * TILE)
#define CELL_Y(row) (ORIGIN_Y + (row) * TILE)

#define HUD_TOP_ROW    0
#define HUD_BOTTOM_ROW 15

#endif // LAYOUT_H
