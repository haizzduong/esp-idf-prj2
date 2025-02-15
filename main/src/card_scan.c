#include "card_scan.h"

static void scan_handle(void *arg, esp_event_base_t base, int32_t event_id, void *data)
{
    rc522_handle_t scanner = (rc522_handle_t)arg;
    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *)data;
    rc522_picc_t *picc = event->picc;

    bool scanner_at_ingate = 1;

    if (scanner == scanner_outgate) 
    {
        scanner_at_ingate = 0;
    }

    if (picc->state == RC522_PICC_STATE_ACTIVE) 
    {
        char uid_str[RC522_PICC_UID_STR_BUFFER_SIZE_MAX];
        rc522_picc_uid_to_str(&picc->uid, uid_str, sizeof(uid_str));
        ESP_LOGI(RFID_TAG, "Card detected on the scanner #%s, ID: %s", scanner_at_ingate ? "IN" : "OUT", uid_str);
    }
    else if (picc->state == RC522_PICC_STATE_IDLE && event->old_state >= RC522_PICC_STATE_ACTIVE) 
    {
        ESP_LOGI(RFID_TAG, "Card has been removed from the scanner #%s", canner_at_ingate ? "IN" : "OUT");
        
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void scan_init()
{
    rc522_spi_create(&scanner_ingate_config, &driver_ingate);
    rc522_spi_create(&scanner_outgate_config, &driver_outgate);


    rc522_driver_install(driver_ingate);
    rc522_driver_install(driver_outgate);


    rc522_create(
        &(rc522_config_t) {
            .driver = driver_ingate,
        },
        &scanner_ingate);

    rc522_create(
        &(rc522_config_t) {
            .driver = driver_outgate,
        },
        &scanner_outgate);

    rc522_register_events(scanner_1, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_1);
    rc522_register_events(scanner_2, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_2);
}


void scan_task_create(void *)
{
    rc522_start(scanner_ingate);
    rc522_start(scanner_outgate);
}