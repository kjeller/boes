#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "timer.h"

#ifndef GPIO_START
#define GPIO_START 3
#endif
#ifndef GPIO_STOP
#define GPIO_STOP 4
#endif
#ifndef GPIO_RESET
#define GPIO_RESET 5
#endif

// Mask for GPIOs to use as Start, Stop and Reset buttons
#ifndef GPIO_MASK
#define GPIO_MASK (1 >> GPIO_START | 1 >> GPIO_STOP | 1 >> GPIO_RESET)
#endif

Timer timer = {Pause, 0, 0, 0, "000", "000"};

void btn_callback(uint gpio, uint32_t events)
{
    switch (gpio)
    {
        case GPIO_START:
            start(&timer);
            break;

        case GPIO_STOP:
            stop(&timer);
            break;

        case GPIO_RESET:
            reset(&timer);
            break;
    }
}

int main()
{
    stdio_init_all();
    gpio_init_mask(GPIO_MASK);
    gpio_set_dir_in_masked(GPIO_MASK);
    gpio_set_irq_enabled_with_callback(GPIO_START, GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    // Only one callback function can be declared. Use btn_callback() for the other GPIOs as well.
    gpio_set_irq_enabled(GPIO_STOP, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(GPIO_RESET, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_pulls(GPIO_START, true, false);
    gpio_set_pulls(GPIO_STOP, true, false);
    gpio_set_pulls(GPIO_RESET, true, false);

    while(true)
    {
        run(&timer);

        // TODO temp code: slow sleep for serial comm
        sleep_ms(10);
        printf("%s:%s\n", timer.sec_counter, timer.ms_counter);
    }

    return 0;
}
