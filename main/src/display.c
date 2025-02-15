#include "display.h"



void init_display()
{
    
    ESP_LOGI(TAG_DISPLAY, "INTERFACE is i2c");
	ESP_LOGI(TAG_DISPLAY, "CONFIG_SDA_GPIO=%d",SDA_GPIO_OLED);
	ESP_LOGI(TAG_DISPLAY, "CONFIG_SCL_GPIO=%d",SCL_GPIO_OLED);
	ESP_LOGI(TAG_DISPLAY, "CONFIG_RST_GPIO=%d",RST_GPIO_OLED);

	i2c_master_init(&dev, SDA_GPIO_OLED, SCL_GPIO_OLED, RST_GPIO_OLED);

    ESP_LOGI(TAG_DISPLAY, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
}

void display()
{


    char display_text[30];
    snprintf(display_text, sizeof(display_text), "Remaining slots: %d", slot_remain);

    ssd1306_display_text(&dev, 0, display_text, 16, false);

}
