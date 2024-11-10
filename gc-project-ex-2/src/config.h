#pragma once

// SETTINGS

#define CLOCK_SPEED 200000 // khz

// GPIO
// Display SPI
#define SPI0_BAUD_RATE (CLOCK_SPEED * 1000)
 
#define GPIO_SPI0_SCK 2
#define GPIO_SPI0_MOSI 3
#define GPIO_SPI0_MISO 4

// Display controls
#define GPIO_SCREEN_LED 0
#define GPIO_SCREEN_CS 1
#define GPIO_SCREEN_DC 5
#define GPIO_SCREEN_RESET 6

// SD Card
#define GPIO_SDCARD_CS 9

// Input
#define GPIO_INPUT_AXIS_X 26 // ADC 0
#define GPIO_INPUT_AXIS_Y 27 // ADC 1

// Sound
#define GPIO_SOUND_WS 7
#define GPIO_SOUND_SCK 8
#define GPIO_SOUND_DATA 10
#define GPIO_SOUND_PWM 25