/*
 * lir_leds.h
 *
 *  Created on: 24 окт. 2016 г.
 *      Author: Евгений
 */

#ifndef LIR_LEDS_H_
#define LIR_LEDS_H_

extern const char src_ver_lir_leds[];

typedef enum
{
	LED_off = 0,
	LED_green,
	LED_red,
	//LED_yellow,
	LED_faded_green,
	LED_faded_red,
} tCOLOR;

#define LED_STAT PIN_A0

extern volatile uint32_t seconds;
extern volatile bool tick_second;

extern volatile tCOLOR  leds_color[8];
extern volatile uint8_t leds_level[8];
extern volatile uint8_t led_status;

void leds_init();
void leds_demo();
void wait_second();
#endif /* LIR_LEDS_H_ */
