#include "action.h"
#include "control.h"


static void IRAM_ATTR gpio_isr_handler(void *arg) {
    uint32_t gpio_num = (uint32_t)arg;
    xEventGroupSetBitsFromISR(control_event_group, gpio_num == GPIO_INGATE_SENSOR ? EVENT_INGATE_CLOSE : EVENT_OUTGATE_CLOSE, &xHigherPriorityTaskWoken);
}

void init_action(void)
{
    ESP_ERROR_CHECK(iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_config));

    ESP_ERROR_CHECK(gpio_config(&sensor_config));
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INGATE_SENSOR, gpio_isr_handler, (void*)GPIO_INGATE_SENSOR));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_OUTGATE_SENSOR, gpio_isr_handler, (void*)GPIO_OUTGATE_SENSOR));
}

void servo_task(void *pv)
{
    EventBits_t servoBits;
    while (1)
    {
        servoBits = xEventGroupWaitBits(control_event_group,
                                        EVENT_INGATE_CLOSE | EVENT_INGATE_OPEN | EVENT_OUTGATE_CLOSE | EVENT_OUTGATE_OPEN
                                        pdTRUE,
                                        pdFALSE,
                                        portMAX_DELAY);
        if (servoBits & EVENT_INGATE_OPEN)
        {
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_IN, OPEN_ANGLE);
        }
        if (servoBits & EVENT_INGATE_CLOSE)
        {
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_IN, CLOSE_ANGLE);
        }
        if (servoBits & EVENT_OUTGATE_OPEN)
        {
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_OUT, OPEN_ANGLE);
        }
        if (servoBits & EVENT_OUTGATE_CLOSE)
        {
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_OUT, CLOSE_ANGLE);
        }

        xEventGroupSetBits(control_event_group
                            EVENT_ACTION_DONE);
    }
}
