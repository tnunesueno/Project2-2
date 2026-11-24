// ============================================
// The header file for general project settings
//
// Copyright 2026 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

// This is an important file that is included in all the other files. 
// It contains the global settings, macros, and error codes for the project. 
// You should not need to change items in this file, but you may need to add other
// global items, depending on your implementation.  
//If you do, please make sure to document them well.

#ifndef GLOBAL_H
#define GLOBAL_H

#define ULCD_4DGL_H

// Include all the hardware libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_rom_sys.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/gptimer.h"
#include "soc/uart_pins.h"
#include "hardware.h"
#include "led_strip.h"
#include "Buttons.h"
#ifdef ULCD_4DGL_H
#include "uLCD_4DGL.h"  
#endif
#include "Nav_Switch.h"

// LCD screen object
#ifdef ULCD_4DGL_H
extern uLCD_4DGL myLCD;
#endif

// Speaker object and its associated timer
extern sound speaker;

extern Buttons buttons;    // Initialize the push buttons
extern Nav_Switch myNav;   // Initialize the navigation switch
extern int debug_flag;     // set to flag events, maybe stop execution or print later

// Global settings
#define DEBUG   0                     // Debug flag

#define BACKGROUND_COLOR 0x000000       // Black

#define PI 3.1415926

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood
#define ERROR_MEH -1 // This is how errors are done


// (Missile Command constants from the previous project removed -- the
// conveyor game's layout constants live in layout.h.)

// === [define the macro of error handle function] ===
// when the condition (c) is not true, print the error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        printf("\nERROR:%d\n",e); \
    } \
} while (0)

#endif //GLOBAL_H