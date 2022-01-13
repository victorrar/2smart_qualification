#pragma once

#include <Arduino.h>

#define GPIO_LED_R  GPIO_NUM_27
#define GPIO_LED_G  GPIO_NUM_26
#define GPIO_LED_B  GPIO_NUM_25

#define GPIO_SONAR_TX  GPIO_NUM_16
#define GPIO_SONAR_RX  GPIO_NUM_17
#define SONAR_LIMIT  100

#define LEDC_CHANNEL_R  1
#define LEDC_CHANNEL_G  2
#define LEDC_CHANNEL_B  3

#define HIGH_THRESHOLD  50
#define LOW_THRESHOLD   10

#define DISTANCE_BUF_SIZE   10

