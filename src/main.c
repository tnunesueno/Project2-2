//=================================================================
// Conveyor Belt Factory -- the main program file.
//
// Copyright 2026 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

// PROJECT INCLUDES
#include "globals.h"
#include "layout.h"
#include "conveyor.h"
#include "boxes.h"
#include "player.h"
#include "esp_random.h"
#include <stdbool.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS

// Hardware objects used by the course libraries (declared extern in globals.h)
uLCD_4DGL  myLCD;
Buttons    buttons;
Nav_Switch myNav;
sound      speaker;      // not used yet unless you add sound in advanced features
int        debug_flag = 0;

// Tuning constants -- GIVEN, adjust to taste
#define FRAME_MS            40   // how often the main loop runs, in milliseconds
#define BELT_STEP_FRAMES    10   // belt moves one slot every N frames
#define MOVE_REPEAT_FRAMES   4   // holding a direction repeats a move every N frames
#define GAME_TASK_STACK   8192   // bytes; app_main's default stack is too small for this game

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
static GameInputs get_inputs(void);
static void draw_hud(Conveyor* cv, BoxLine* bl);
static void handle_movement(Player* p, GameInputs* in, int* move_timer);
static void handle_buttons(Conveyor* cv, BoxLine* bl, Player* p, GameInputs* in, GameInputs* prev);
static void show_game_over(int score, int money);
static void show_win_screen(int score, int money);
static void play_round(void);
static void game_task(void* arg);

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS

/**
 * Reads the current state of the pushbuttons and nav switch.
 * GIVEN - a thin wrapper around the course's read_inputs() so the rest of
 * this file doesn't need to pass &buttons/&myNav around everywhere.
 */
static GameInputs get_inputs(void)
{
    GameInputs in;
    read_inputs(&in, &buttons, &myNav);
    return in;
}

/**
 * Prints the HUD(Heads Up Display): the belt's money, the player's score, and anything else
 * you want visible every frame (e.g. PAUSED). layout.h's HUD_TOP_ROW /
 * HUD_BOTTOM_ROW are text rows that sit above/below the 9x9 play grid, so
 * HUD text won't overlap the belt.
 */
static void draw_hud(Conveyor* cv, BoxLine* bl)
{
    // TODO: Implement

    // 1. Move the text cursor to HUD_TOP_ROW, column 0 (uLCD_locate)
    // 2. Set the text color (uLCD_text_color)
    // 3. Print the money and score (uLCD_printf) -- something like "$%d  SCORE %d"
    // 4. Optional: use HUD_BOTTOM_ROW for a second line (e.g. "PAUSED" when cv->paused)

    // HINT: re-sending text that hasn't changed every single frame is a lot
    // of slow serial commands. Consider only calling uLCD_printf() when the
    // numbers actually changed since last frame (e.g. keep a "last printed"
    // copy and compare).
    (void)cv; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)bl; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
}

/**
 * Moves the player based on the nav switch. Called once per frame.
 * MOVE_REPEAT_FRAMES and *move_timer are given so that holding a direction
 * keeps moving the player every few frames, instead of requiring a fresh
 * press per tile -- you don't have to use them if you'd rather require a
 * press-and-release per tile instead.
 */
static void handle_movement(Player* p, GameInputs* in, int* move_timer)
{
    // TODO: Implement

    // 1. Check in->up / in->down / in->left / in->right
    // 2. If one is pressed and *move_timer has counted down to 0 (or this is
    //    a fresh press), call player_move() with the matching (dx, dy) and
    //    reset *move_timer to MOVE_REPEAT_FRAMES
    // 3. Otherwise, if any direction is held, decrement *move_timer
    // 4. If no direction is held, reset *move_timer to 0 so the next press
    //    moves immediately
    (void)p; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)in; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)move_timer; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
}

/**
 * Handles what the pushbuttons do. At minimum you need some way to:
 *   - pick a part off the belt the player is standing on
 *   - deliver a held part to a box the player is next to
 * The Advanced features list suggests other ideas for the remaining
 * buttons: trash a held part, pause the belt, etc.
 */
static void handle_buttons(Conveyor* cv, BoxLine* bl, Player* p, GameInputs* in, GameInputs* prev)
{
    // TODO: Implement

    // 1. Edge-detect each button: it should fire once per press, not once
    //    per frame held down (compare in->b0 against prev->b0, etc.)

    // 2. Decide what each button does. Some building blocks you'll need:
    //    2a. conveyor_slot_at(cv, p->x, p->y) -- which belt slot (if any)
    //        the player is standing on
    //    2b. conveyor_take_at(cv, slot) -- pick up whatever's in that slot
    //        (remember to free() the ConveyorItem* it returns once you've
    //        read ->type out of it)
    //    2c. boxes_adjacent(bl, p->x, p->y) -- which box (if any) the player
    //        is next to
    //    2d. boxes_try_deliver(bl, box_index, p->held) -- attempt a delivery

    // 3. Update p->holding / p->held to reflect whether the player is

    //    carrying a part after each action
    (void)cv; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)bl; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)p; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)in; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)prev; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
}

/**
 * Shown when the belt jams (conveyor_full() returns true). Should display
 * the final score/money and wait for some input (e.g. the center button)
 * before returning, so the player has a chance to read it.
 */
static void show_game_over(int score, int money)
{
    // TODO: Implement

    // 1. Clear the screen (uLCD_cls)
    // 2. Print "GAME OVER" plus the final score and money
    // 3. Loop calling get_inputs() until some button is pressed, so the
    //    screen doesn't disappear instantly
    (void)score; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)money; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
}

/**
 * Shown when the player wins. What counts as "winning" this game is your
 * design choice -- a score threshold? Surviving a number of belt spawns?
 * Decide, then call this function from play_round() when that condition
 * is met.
 */
static void show_win_screen(int score, int money)
{
    // TODO: Implement

    // 1. Clear the screen
    // 2. Print a win message plus the final score and money
    // 3. Loop calling get_inputs() until some button is pressed
    (void)score; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
    (void)money; //DELETE LINE WHEN THIS METHOD IS IMPLEMENTED
}

/**
 * Plays one round of the game: sets up a fresh belt/boxes/player, runs the
 * main loop until the belt jams (or the player wins), then shows the
 * appropriate end screen. GIVEN -- the overall shape of this loop shouldn't
 * need to change, but it calls several functions above that you need to fill in.
 */
static void play_round(void)
{
    Conveyor belt;
    conveyor_init(&belt);
    BoxLine line;
    boxes_init(&line);
    Player p;
    player_init(&p);

    uLCD_cls(&myLCD);
    uLCD_textbackground_color(&myLCD, BLACK);
    uLCD_text_mode(&myLCD, OPAQUE);   // text overwrites what's behind it -- helpful for HUD updates

    GameInputs prev = get_inputs();   // so a button held from a previous screen doesn't fire immediately
    int belt_frames = 0;
    int move_timer = 0;
    bool game_over = false;

    while (!game_over) {
        GameInputs in = get_inputs();

        handle_movement(&p, &in, &move_timer);
        handle_buttons(&belt, &line, &p, &in, &prev);

        if (!belt.paused) {
            if (++belt_frames >= BELT_STEP_FRAMES) {
                belt_frames = 0;
                conveyor_tick(&belt);           // TODO in conveyor.c
                conveyor_update_spawn(&belt);   // TODO in conveyor.c
                if (conveyor_full(&belt)) game_over = true;  // TODO in conveyor.c
            }
        }

        // GIVEN: redraw the belt, boxes, and player each frame. The actual
        // drawing logic lives in each file's own TODOs (conveyor.c,
        // boxes.c, player.c) -- NOT in main.c. Careful when you implement
        // those: calling uLCD_cls() every single frame will cause a visible
        // flicker, since this LCD is slow to redraw over serial -- try to
        // only clear/redraw tiles that actually changed instead of the
        // whole screen.

        conveyor_draw(&belt);
        boxes_draw(&line);
        player_draw(&p);

        draw_hud(&belt, &line);

        prev = in;
        vTaskDelay(pdMS_TO_TICKS(FRAME_MS));
    }

    show_game_over(line.score, line.money);
    conveyor_free(&belt);
}

/**
 * The game's FreeRTOS task. GIVEN -- initializes the hardware, seeds the
 * random number generator, then plays rounds forever.
 */
static void game_task(void* arg)
{
    (void)arg;
    init_hardware();   // course routine: LED, buttons, nav switch, LCD reset + baud (~5 s)
    srand(esp_random());

    while (true) {
        play_round();
    }
}

/**
 * Program entry point! GIVEN -- runs the game in its own task with a bigger
 * stack than app_main gets by default. Most of your implementation should be
 * in conveyor.c, boxes.c, player.c, and parts.c -- start there.
 */
void app_main(void)
{
    xTaskCreate(game_task, "game", GAME_TASK_STACK, NULL, 5, NULL);
}