#include "driver_adc.h"

void adc_Init(void)
{
#if defined CONFIG_SUPPORT_PICO
    stdio_init_all();
    adc_init();
    adc_gpio_init(Light_sensor);
    adc_select_input(0);
#elif defined CONFIG_SUPPORT_ESP32S2
    adc_digi_config_t config; 
	config.conv_limit_en = false;
	config.conv_limit_num = 0;
	/** Sample rate = APB_CLK(80 MHz) / (dig_clk.div_num+ 1) / TRIGGER_INTERVAL / 2. */
	config.interval = 0;
	config.dig_clk.use_apll = 0;  // APB clk
	config.dig_clk.div_num = 9;
	config.dig_clk.div_b = 0;
	config.dig_clk.div_a = 0;
	config.dma_eof_num = 24;
   
    /**
    所谓的样式表就是ADC的采集列表。设置样式表数组的话，就是根据数组依次采集，然后依次放入buf中，比如你创建了样式表有通道1和通道2两个元素，则buf[0]存放通道1的值，buf[1]存放通道2的值，buf[2]再存放通道1的值，依次循环。我只用了一个通道因此长度位1，当然样式表可以先放两个通道2再放一个通道1的话，buf[0],buf[1]是通道2，buf[2]是通道1，再循环。
    总的来说，这个样式表对于多通道无顺序采集还是挺方便的
    */
    //设置样式表，可数组
    adc_digi_pattern_table_t adc1_patt = {0};
    //样式表长度
    config.adc1_pattern_len = 1;
    //样式表地址配置赋值
    config.adc1_pattern = &adc1_patt;
    //样式表的量程
    adc1_patt.atten = ADC_ATTEN_11db;
    //样式表的通道
    adc1_patt.channel = (adc_channel_t)ADC1_CHANNEL_5;  
    //该通道的引脚初始化
    adc_gpio_init(ADC_UNIT_1,(adc_channel_t)ADC1_CHANNEL_5);
    //转换模式，单次，见下面的转换模式图
    config.conv_mode = ADC_CONV_SINGLE_UNIT_1;
    //DMA模式下使用数据格式1因此为12为的ADC
    config.format = ADC_DIGI_FORMAT_12BIT;
    //配置的初始化
    adc_digi_controller_config(&config);
#endif

}


uint16_t get_adc_value(void)
{
	uint16_t read_raw;
#if defined CONFIG_SUPPORT_PICO
    read_raw = adc_read();
    return (read_raw - 700);
#elif defined CONFIG_SUPPORT_ESP32S2
	read_raw = adc1_get_raw(ADC1_CHANNEL_5);
    read_raw = (read_raw*1100)/4096;
    return read_raw;
#endif
}