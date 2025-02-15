#include <stdio.h>

#include "control.h"

#define TAG "Project 2"

SSD1306_t dev;
int slot_remain = 3;
EventGroupHandle_t control_event_group = NULL;

void app_main(void)
{ 
    init_prj();

    scan_task_create();
    
    
}
