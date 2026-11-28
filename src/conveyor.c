#include "conveyor.h"
#include "globals.h"
#include <stdlib.h>

#define BELT_COLOR   DGREY
#define SPAWN_COLOR  0x206020   // dark green: where parts appear

// GIVEN -- traces the outer ring of the grid clockwise, starting at the
// top-left corner: top row L->R, right column T->B, bottom row R->L, left
// column B->T. You shouldn't need to change this.
static void build_path(BeltPoint* path)
{
    int idx = 0;
    for (int c = 0; c < GRID_W; c++)          path[idx++] = (BeltPoint){ CELL_X(c), CELL_Y(0) };
    for (int r = 1; r < GRID_H; r++)          path[idx++] = (BeltPoint){ CELL_X(GRID_W - 1), CELL_Y(r) };
    for (int c = GRID_W - 2; c >= 0; c--)     path[idx++] = (BeltPoint){ CELL_X(c), CELL_Y(GRID_H - 1) };
    for (int r = GRID_H - 2; r >= 1; r--)     path[idx++] = (BeltPoint){ CELL_X(0), CELL_Y(r) };
}

// TODO [Base]: allocate the belt's doubly linked list with create_dlinkedlist()
// (see doubly_linked_list.h) and reset the rest of the belt's state.
//create_dlinkedlist(); 

void conveyor_init(Conveyor* cv)
{
    cv->items = create_dlinkedlist();
    cv->count = 0;
    cv->paused = false;
    cv->ticks_since_spawn = 0;
    cv->spawn_interval = SPAWN_MIN_STEPS + rand() % (SPAWN_MAX_STEPS - SPAWN_MIN_STEPS + 1);
    build_path(cv->path);
}

// TODO [Base]: "Making the conveyor belt moved". Advance every part one slot
// around the loop. A part sitting at the last slot (BELT_LEN - 1) wraps back
// to slot 0, since the belt is a continuous loop -- think about how to do
// that using the DLL's insertTail()/deleteNode() rather than by hand-editing
// pointers. Walk the list with getHead()/getNext()/getData(). A no-op while
// paused (check cv->paused first).
void conveyor_tick(Conveyor* cv) {
    // TODO: implement.
    
    while (!cv->paused){
        LLNode* currentNode = getHead(cv->items);
        while(currentNode){
            ConveyorItem * currentItem = (ConveyorItem*)getData(currentNode); 
            //if the current node is the last slot, it must be deleted out of the tail and added to the head 
            if(currentItem->pos == BELT_LEN -1){
                deleteNode(cv->items, currentNode); 
                insertHead(cv->items, currentNode); 
                // Continue to the next, now the one right after the head for another tick 
                currentNode = getNext(currentNode); 
            } else{
                // if not tail, move one forward 
                currentItem->pos++; 
                currentNode = getNext(currentNode); 
            }
        }

    }
}


// TODO [Base]: "obvious spawn point". Add a new random part at slot 0 using
// insertTail(). Should fail (return false) if the spawn slot is already
// occupied -- how will you check that using only the DLL's accessor
// functions (getHead/getTail/getData)?
bool conveyor_spawn(Conveyor* cv)
{
    // TODO: implement.
    // check if head and tail are in slot 0 
    LLNode* head = getHead(cv->items);
    LLNode* tail = getTail(cv->items); 
    // get head and tail position 
    return false;
}

// GIVEN -- calls conveyor_spawn() on a random schedule. You shouldn't need to
// change this once conveyor_spawn() actually works.
bool conveyor_update_spawn(Conveyor* cv)
{
    if (cv->paused) return false;
    if (cv->ticks_since_spawn < cv->spawn_interval) cv->ticks_since_spawn++;
    if (cv->ticks_since_spawn < cv->spawn_interval) return false;   // not due yet

    if (!conveyor_spawn(cv)) return false;

    cv->ticks_since_spawn = 0;
    cv->spawn_interval = SPAWN_MIN_STEPS + rand() % (SPAWN_MAX_STEPS - SPAWN_MIN_STEPS + 1);
    return true;
}

// TODO [Base]: "failure state: the belt jams". True when every slot holds a
// part. (Hint: you don't need to walk the list for this one.)
bool conveyor_full(Conveyor* cv)
{
    // TODO: implement.
    return false;
}

// TODO [Base]: used when the player picks a part up off the belt. Find the
// node whose data->pos == pos, make a standalone copy of its type/pos to
// return (the caller will free() it), then remove the real node from the
// list with deleteNode().
ConveyorItem* conveyor_take_at(Conveyor* cv, int pos)
{
    // TODO: implement.
    return NULL;
}

// TODO: used when the player drops a held part back onto an empty belt tile.
// Insert a new node at the given pos. Returns false if that slot is already
// occupied, pos is out of range, or allocation fails.
bool conveyor_put_at(Conveyor* cv, int pos, PartType type)
{
    // TODO: implement.
    return false;
}

// GIVEN -- you shouldn't need to change this.
int conveyor_slot_at(Conveyor* cv, int x, int y)
{
    for (int i = 0; i < BELT_LEN; i++) {
        if (cv->path[i].x == x && cv->path[i].y == y) return i;
    }
    return -1;
}

// TODO [Base]: walk the list and fill out[i] with the PartType sitting at
// slot i (out[] is already initialized to all -1 = empty for you below).
void conveyor_occupancy(Conveyor* cv, int out[BELT_LEN])
{
    for (int i = 0; i < BELT_LEN; i++) out[i] = -1;
    // TODO: fill in the occupied slots by walking cv->items.
}

// GIVEN -- draws one slot. You shouldn't need to change this, but it depends
// on conveyor_occupancy() being implemented to look right.
void conveyor_draw_slot(Conveyor* cv, int pos, int contents)
{
    BeltPoint p = cv->path[pos];
    int color = (pos == 0) ? SPAWN_COLOR : BELT_COLOR;
    uLCD_filled_rectangle(&myLCD, p.x, p.y, p.x + TILE - 1, p.y + TILE - 1, color);
    if (contents >= 0) part_draw((PartType)contents, p.x, p.y);
}

// GIVEN.
void conveyor_draw(Conveyor* cv)
{
    int occ[BELT_LEN];
    conveyor_occupancy(cv, occ);
    for (int i = 0; i < BELT_LEN; i++) conveyor_draw_slot(cv, i, occ[i]);
}

// GIVEN.
void conveyor_set_paused(Conveyor* cv, bool paused)
{
    //cv->paused = paused;
}

// TODO: free the whole list via destroyList().
void conveyor_free(Conveyor* cv)
{
    // TODO: implement.
    cv->count = 0;
}
