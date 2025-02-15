#pragma once

#include <esp_log.h>
#include "rc522.h"
#include "driver/rc522_spi.h"
#include "rc522_picc.h"

#include "handle.c"

#define TAG_CARD_SCAN "Card Scan"

#define READER_SPI_MISO (25)
#define READER_SPI_MOSI (23)
#define READER_SPI_SCLK (19)


#define READER_INGATE_SPI_SDA (22)
#define READER_OUTGATE_SPI_SDA (26)

static rc522_spi_config_t scanner_ingate_config = {
    .host_id = SPI3_HOST,
    .bus_config = &(spi_bus_config_t){
        .miso_io_num = READER_SPI_MISO,
        .mosi_io_num = READER_SPI_MOSI,
        .sclk_io_num = READER_SPI_SCLK,
    },
    .dev_config = {
        .spics_io_num = READER_INGATE_SPI_SDA,
    },
    .rst_io_num = -1, // soft-reset
};


static rc522_spi_config_t scanner_outgate_config = {
    .host_id = SPI3_HOST,
    .dev_config = {
        .spics_io_num = READER_OUTGATE_SPI_SDA,
    },
    .rst_io_num = -1, // soft-reset
};

static rc522_driver_handle_t driver_ingate;
static rc522_driver_handle_t driver_outgate;

static rc522_handle_t scanner_ingate;
static rc522_handle_t scanner_outgate;

static void scan_handle(void *, esp_event_base_t, int32_t, void);

void scan_init(void);
