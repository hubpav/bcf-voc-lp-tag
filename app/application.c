#include <application.h>

bc_led_t led;

bc_sgpc3_t sgpc3;

void sgpc3_event_handler(bc_sgpc3_t *self, bc_sgpc3_event_t event, void *event_param)
{
    if (event == BC_SGPC3_EVENT_ERROR)
    {
        bc_log_error("SPGC3: Event error");
    }
    else if (event == BC_SGPC3_EVENT_UPDATE)
    {
        uint16_t value;

        if (bc_sgpc3_get_tvoc_ppb(self, &value))
        {
            bc_log_info("SPGC3: Event update (%u ppb)", value);

            if (value != 0)
            {
                bc_led_pulse(&led, 100);
            }
        }
    }
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 1000);

    bc_sgpc3_init(&sgpc3, BC_I2C_I2C0, 0x58);
    bc_sgpc3_set_event_handler(&sgpc3, sgpc3_event_handler, NULL);
    bc_sgpc3_set_update_interval(&sgpc3, 5000);
}
