#ifndef LED_H
#define LED_H

void led_init(void);
void led_on(void);
void led_off(void);
void led_blink(uint32_t cnt, uint32_t dly);

#endif
