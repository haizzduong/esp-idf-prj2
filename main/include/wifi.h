#pragma once

#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <stdlib.h>

#define TAG_WIFI    "Wifi"
#define WIFI_SSID   "NHD"
#define WIFI_PASS   "123456789"

static void wifi_event_handler(void *, esp_event_base_t, int32_t, void *);

void init_wifi(void);