#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"

#include "ssd1306.h"


#define SDA_GPIO_OLED   (21)
#define SCL_GPIO_OLED   (22)
#define RST_GPIO_OLED   (-1)

#define TAG_DISPLAY     "Display"

extern SSD1306_t    dev;
extern int          slot_remain;

void    init_display(void);
void    display(void);