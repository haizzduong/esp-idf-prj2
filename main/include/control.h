#pragma once

#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "display.h"
#include "wifi.h"

#define EVENT_INGATE_OPEN    (1 << 0)
#define EVENT_OUTGATE_OPEN   (1 << 1)
#define EVENT_INGATE_CLOSE    (1 << 3)
#define EVENT_OUTGATE_CLOSE   (1 << 4)

extern EventGroupHandle_t control_event_group;

void init_prj();

void display_task(void *);

void servo_task(void *);

void scan_task_create(void *);
