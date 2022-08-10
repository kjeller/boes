#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H
#include <stdio.h>
#include "RGBMatrixConfig.h"
#if defined CONFIG_SUPPORT_PICO

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#define Light_sensor 26

#elif defined CONFIG_SUPPORT_ESP32S2

#include "driver/adc.h"


#endif

void adc_Init(void);
uint16_t get_adc_value(void);


#endif

