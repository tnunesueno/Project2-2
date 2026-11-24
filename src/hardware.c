//=================================================================
// Copyright 2026 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

#include "globals.h"

static const char *TAG = "example";
int alarm_count = 10000;

// Function to read all inputs and populate the GameInputs struct,
//  mainly to remain consistent with the Mbed version.
void read_inputs(GameInputs* inputs, Buttons* buttons, Nav_Switch* myNav) 
{
    inputs->b0 = pb0(buttons);
    inputs->b1 = pb1(buttons);
    inputs->b2 = pb2(buttons);
    inputs->b3 = pb3(buttons);

    inputs->up = up(myNav);
    inputs->down = down(myNav);
    inputs->left = left(myNav);
    inputs->right = right(myNav);
    inputs->center = center(myNav);
    
    return;
}

led_strip_handle_t configure_led(gpio_num_t led_gpio_pin)
{
    gpio_reset_pin(led_gpio_pin); // Data pin for LED strip
    gpio_set_direction(led_gpio_pin, GPIO_MODE_OUTPUT);

    // LED strip general initialization, for the single LED (trivial "strip") on the ESP32-C6-DevKit board
    led_strip_config_t strip_config = {
        .strip_gpio_num = led_gpio_pin, // The GPIO that connected to the LED strip's data line
        .max_leds = 1,      // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812,        // LED model
        // set the color order of the LED: GRB
        .color_component_format = {
            .format = {
                .r_pos = 1, // red is the second byte in the color data
                .g_pos = 0, // green is the first byte in the color data
                .b_pos = 2, // blue is the third byte in the color data
                .num_components = 3, // total 3 color components
            },
        },
        .flags = {
            .invert_out = false, // don't invert the output signal
        }
    };

    // LED strip backend configuration: SPI
    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .spi_bus = SPI2_HOST,           // SPI bus ID
        .flags = {
            .with_dma = true, // Using DMA can improve performance and help drive more LEDs
        }
    };

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
    ESP_LOGI(TAG, "Created LED strip object with SPI backend");
    return led_strip;
}


// DO NOT CHANGE THIS FUNCTION. It is used to configure the timer and speaker for sound output.
static gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
    .direction = GPTIMER_COUNT_UP,      // Counting direction is up
    .resolution_hz = 1 * 1000 * 1000,   // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
};


// DO NOT CHANGE THIS FUNCTION. It is used to configure the timer and speaker for sound output.
static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    // General process for handling event callbacks:
    // 1. Retrieve user context data from user_ctx (passed in from gptimer_register_event_callbacks)
    // 2. Get alarm event data from edata, such as edata->count_value
    // 3. Perform user-defined operations
    // 4. Return whether a high-priority task was awakened during the above operations to notify the scheduler to switch tasks
    static bool level = 0;
    gptimer_set_raw_count(timer, MAX_TIMER_COUNT-alarm_count); // Reset the timer count to current freq.
    gpio_set_level(SPEAKER_GPIO_PIN, level);
    if (speaker.enabled) {
        level = !level;  // produce a square wave by toggling the output level
    }
    else level = 0;  // turn off the speaker, or both speaker and transistor will heat up
    return false;
}

// DO NOT CHANGE THIS FUNCTION. It is used to configure the timer and speaker for sound output.
static gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,      // When the alarm event occurs, the timer will automatically reload to 0
    .alarm_count = MAX_TIMER_COUNT, // Set the actual alarm rate
    .flags.auto_reload_on_alarm = true, // Enable auto-reload function
};

// DO NOT CHANGE THIS FUNCTION. It is used to configure the timer and speaker for sound output.
static gptimer_event_callbacks_t cbs = {
    .on_alarm = example_timer_on_alarm_cb, // Call the user callback function when the alarm event occurs
};


// Functions to support sound output

// Configure the speaker and timer for sound output -- do not change
// But note that some elements of the my_sound struct can be changed prior to making
// calls to sound_on() and tone() to change the sound output.
int configure_sound(sound* my_sound, gptimer_handle_t* gptimer)
{
    my_sound->speaker_gpio = SPEAKER_GPIO_PIN;
    my_sound->resolution =  1000000;  // Frequency resolution in Hz (1 MHz, i.e., 1 tick equals 1 microsecond)
    my_sound->frequency = 20;         // Frequency of current tone (square wave), Hz
    my_sound->volume = 50;            // Default volume level (not implemented)
    my_sound->duration_ms = 0;        // Default duration (continuous)
    my_sound->enabled = 0;            // Default enabled flag (disabled)
    gpio_reset_pin(my_sound->speaker_gpio); // Pin for speaker output
    gpio_set_direction(my_sound->speaker_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(my_sound->speaker_gpio, 0);

    // Create a timer instance
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, gptimer));
	if (DEBUG) printf("New timer created! \n");
    // Set the timer's alarm action
    ESP_ERROR_CHECK(gptimer_set_alarm_action(*gptimer, &alarm_config));
	if (DEBUG) printf("Alarm action set \n");

    // Register timer event callback functions, allowing user context to be carried
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(*gptimer, &cbs, NULL));
	if (DEBUG) printf("Sound configured! \n");

    return 0;
}

int sound_on(sound* my_sound, gptimer_handle_t* gptimer) {
    // enable timer (and its interrupt) to play sound
    ESP_ERROR_CHECK(gptimer_enable(*gptimer));
    if (DEBUG) printf("Timer enabled! \n");

    // Start the timer
    ESP_ERROR_CHECK(gptimer_start(*gptimer));
    if (DEBUG) printf("Timer started! \n");
    my_sound->enabled = 0;  // technically, it won't be on until a tone() is called
    return 0;
}

// Play a tone at the specified frequency for the specified duration (in milliseconds)
// First two parameters are the sound struct and the timer handle created during
// the initialization of the sound system.
// The frequency is in Hz, and duration is in milliseconds (0 means continuous, if enabled).
int tone(sound* my_sound, gptimer_handle_t* gptimer_clock_source_t, int frequency, int duration_ms) {
    my_sound->frequency = frequency;
    my_sound->duration_ms = duration_ms;
    alarm_count = (timer_config.resolution_hz/frequency)/2;
    my_sound->enabled = 1;
    if (duration_ms > 0) {
        vTaskDelay(pdMS_TO_TICKS(duration_ms));
        my_sound->enabled = 0;
        gpio_set_level(my_sound->speaker_gpio, 0); //When not playing a tone, the speaker output is set to 0 (low) to keep transistor and speaker from getting hot
    }
    return 0;
}


// DO NOT CHANGE THIS FUNCTION. It is used to initialize the hardware for the game.
int init_hardware() {

    // Initialize the RGB LED on the processor board, the button inputs, and the navigation switch
    // Initialize the display
    // This routine can take about 10 seconds to complete -- do not change anything here
    // in an attempt to speed up the initialization process

	led_strip_handle_t rgb_led = configure_led(LED_GPIO_PIN); // Configure the RGB LED
	led_strip_clear(rgb_led);
	led_strip_set_pixel(rgb_led, 0, 0x80, 0, 0  ); // First sign of life for app -- turn on LED as 50% red
	led_strip_refresh(rgb_led); // Refresh strip to send data

	init_Buttons(&buttons, PB0_GPIO_PIN, PB1_GPIO_PIN, PB2_GPIO_PIN, PB3_GPIO_PIN);
	init_Nav_Switch(&myNav, NAV_UP_GPIO_PIN, NAV_DOWN_GPIO_PIN, NAV_LEFT_GPIO_PIN, NAV_RIGHT_GPIO_PIN, NAV_CENTER_GPIO_PIN);

	esp_rom_delay_us(1000000); // Delay to allow time to open serial monitor
	esp_rom_printf("%s", "\nStarting...\n");  // Unless the serial monitor is connected very quickly,

	// Initialize the uLCD -- nothing will be visible for three seconds!!
	// Do not call any uLCD functions until uLCD_init and uLCD_reset are complete
	uLCD_init(&myLCD, UART_NUM_1, LCD_RESET_GPIO_PIN, LCD_TX_GPIO_PIN, LCD_RX_GPIO_PIN); 
	esp_rom_printf("\nResetting LCD (requires 3 seconds to complete)\n");    
	uLCD_reset(&myLCD);
	if (DEBUG) printf("LCD reset! \n");
    led_strip_set_pixel(rgb_led, 0, 0, 0, 0xFF  );      // Give a blue flicker to show life after LCD reset
	led_strip_refresh(rgb_led); // Refresh strip to send data

	// Clear the LCD screen
	uLCD_cls(&myLCD);
	if (DEBUG) printf("LCD screen cleared!\n");


	// Change the LCD baud rate to 115200 for faster drawing
	// Note that there are limitations on the LCD side for max baud rate. Even at 115200,
	// the effective speed is about 40000 baud due to processing delays in the LCD.
	uLCD_baudrate(&myLCD, 115200);
	if (DEBUG) printf("LCD Baudrate changed!\n");

	uLCD_locate(&myLCD, 0, 0);
	if (DEBUG) { // In debug mode, pause 2 seconds to see some text on LCD before demo starts
		uLCD_puts(&myLCD, "Wait...\n", 0);
		vTaskDelay(pdMS_TO_TICKS(2000)); 
	}

    led_strip_set_pixel(rgb_led, 0, 0, 0x7F, 0 ); // Finished initialization -- LED turns 50% green
	led_strip_refresh(rgb_led); // Refresh strip to send data

    return 0; // Return 0 on success
}