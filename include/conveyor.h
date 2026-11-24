#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <stdbool.h>
#include "parts.h"
#include "layout.h"
#include "doubly_linked_list.h"   // the class-standard DLL -- this is the data
                                  // structure you'll use to implement the belt

// Number of tile slots around the belt loop (the outer ring of the grid).
// This is also the belt's capacity: each slot holds at most one part.
#define BELT_LEN (2 * (GRID_W + GRID_H) - 4)

// One tile's pixel position along the belt's path (top-left corner, 11x11).
typedef struct {
    int x, y;
} BeltPoint;

// Random spawning: after each spawn, the next one is scheduled a random number
// of belt steps later, between these two values (inclusive). If the spawn
// slot is occupied when a spawn comes due, the spawn waits until the slot is
// empty. Raise these to make parts rarer.
#define SPAWN_MIN_STEPS  6
#define SPAWN_MAX_STEPS 20

// Returned by conveyor_take_at() -- a standalone copy of a part's data for
// the caller to inspect and then free(). This is NOT a node in the belt's
// DLinkedList.
typedef struct ConveyorItem {
    PartType type;
    int pos;
    struct ConveyorItem* next; // unused; always NULL on a returned item
} ConveyorItem;

// The payload you should store as each DLL node's `data` (see
// doubly_linked_list.h: insertTail/insertBefore/insertAfter all take a
// void* data pointer). Suggested invariant: keep the list sorted by pos,
// largest first, so getHead() is the part furthest along the loop and
// getTail() is the part closest to the spawn slot -- but it's your design,
// so feel free to do it differently if you have a reason to.
typedef struct {
    PartType type;
    int pos;   // 0 = spawn slot ... BELT_LEN-1 = last slot before wrapping
} ConveyorItemData;

typedef struct {
    DLinkedList* items;   // the belt's doubly linked list of ConveyorItemData* nodes
    BeltPoint path[BELT_LEN];
    int count;            // number of parts currently on the belt
    bool paused;
    int ticks_since_spawn;
    int spawn_interval;   // belt steps until the next spawn is due (re-randomized after each spawn)
} Conveyor;

// Allocates the belt's DLinkedList (via create_dlinkedlist()) and lays out
// the loop path.
void conveyor_init(Conveyor* cv);

// Advances every part one slot around the loop (parts at the last slot wrap
// back to slot 0). All parts should move together, so they never collide.
// A no-op while paused.
void conveyor_tick(Conveyor* cv);

// Counts one belt step toward the next random spawn. When a spawn is due and
// the spawn slot (pos 0) is empty, adds a random part there and schedules the
// next spawn. Returns true if a part was spawned. A no-op while paused.
bool conveyor_update_spawn(Conveyor* cv);

// Adds a random part at the spawn slot right now. Returns false if the
// spawn slot is occupied (or memory ran out).
bool conveyor_spawn(Conveyor* cv);

// True when every slot holds a part -- no room left to add parts (game over).
bool conveyor_full(Conveyor* cv);

// Removes the item at belt slot `pos` and returns a standalone copy of it
// (NULL if that slot is empty). Caller owns the returned ConveyorItem and
// should free() it after reading ->type.
ConveyorItem* conveyor_take_at(Conveyor* cv, int pos);

// Puts a part onto belt slot `pos` (e.g. the player dropping a part back on).
// Returns false if the slot is taken, out of range, or memory ran out.
bool conveyor_put_at(Conveyor* cv, int pos, PartType type);

// Belt slot index whose tile has top-left (x, y), or -1 if it isn't a belt tile.
// GIVEN -- you shouldn't need to change this.
int conveyor_slot_at(Conveyor* cv, int x, int y);

// Fills out[i] with the PartType at slot i, or -1 if the slot is empty.
void conveyor_occupancy(Conveyor* cv, int out[BELT_LEN]);

// Draws one slot: the belt tile plus `contents` (a PartType, or -1 for empty).
// GIVEN -- depends on conveyor_occupancy() working to look right.
void conveyor_draw_slot(Conveyor* cv, int pos, int contents);

// Redraws the whole belt (every slot). GIVEN.
void conveyor_draw(Conveyor* cv);

// GIVEN.
void conveyor_set_paused(Conveyor* cv, bool paused);

// Frees the whole list (every node, every node's data) via destroyList().
// Call at game over.
void conveyor_free(Conveyor* cv);

#endif // CONVEYOR_H
