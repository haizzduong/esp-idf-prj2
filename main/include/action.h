#pragma once

#include "iot_servo.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG_ACTION "Action"

#define GPIO_INGATE_SENSOR  
#define GPIO_OUTGATE_SENSOR 

#define LEDC_CHANNEL_IN     LEDC_CHANNEL_0
#define LEDC_CHANNEL_OUT    LEDC_CHANNEL_1

#define SERVO_INGATE_PIN    
#define SERVO_OUTGATE_PIN   
#define GPIO_INOUT_IR_PIN   ((1ULL << GPIO_INGATE_SENSOR) | (1ULL << GPIO_OUTGATE_SENSOR))

#define OPEN_ANGLE          90
#define CLOSE_ANGLE         0

static servo_config_t servo_config = {
    .max_angle = 180,
    .min_width_us = 500,
    .max_width_us = 2500,
    .freq = 50,
    .timer_number = LEDC_TIMER_0,
    .channels = {
        .servo_pin = {
            SERVO_INGATE_PIN,
            SERVO_OUTGATE_PIN,
        },
        .ch = {
            LEDC_CHANNEL_IN,
            LEDC_CHANNEL_OUT,
        },
    },
    .channel_number = 2,
};

static gpio_config_t sensor_config = {
    .intr_type = GPIO_INTR_NEGEDGE,         
    .mode = GPIO_MODE_INPUT,                
    .pin_bit_mask = GPIO_INOUT_IR_PIN,     
    .pull_up_en = GPIO_PULLUP_DISABLE,        
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
};

void init_action(void);
