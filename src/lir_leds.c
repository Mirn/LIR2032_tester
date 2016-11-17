/*
 * lir_leds.c
 *
 *  Created on: 24 окт. 2016 г.
 *      Author: Евгений
 */

#include "stm32kiss.h"
#include "lir_leds.h"

const char src_ver_lir_leds[] = __DATE__"\t"__TIME__"\t"__FILE__"\r";

const KS_PIN * const PIN_LEDS_R[8] = {
		PIN_B7,
		PIN_B8,
		PIN_B9,
		PIN_C13,
		PIN_C14,
		PIN_C15,
		PIN_D0,
		PIN_D1,
};

const KS_PIN * const PIN_LEDS_G[8] = {
		PIN_C10,
		PIN_C11,
		PIN_C12,
		PIN_D2,
		PIN_B3,
		PIN_B4,
		PIN_B5,
		PIN_B6,
};

static uint32_t second_cnt = 0;

volatile uint32_t seconds = 0;
volatile bool tick_second = false;

volatile tCOLOR  leds_color[8] = {0};
volatile uint8_t leds_level[8] = {0};
volatile uint8_t led_status = false;

#define SECOND_FREQ 1000

static void led_tick()
{
	second_cnt++;
	tick_second = (second_cnt >= SECOND_FREQ);
	if (tick_second)
	{
		second_cnt = 0;
		seconds++;
	}

	uint32_t led_cnt = (((seconds * SECOND_FREQ) + second_cnt)) % (8*2*100);
	uint8_t led_phase = led_cnt % 64;
	uint8_t led_pos   = led_cnt / 200;

	for (uint32_t pos = 0; pos < 8; pos++)
	{
		uint8_t level = leds_level[pos];
		tCOLOR  color = leds_color[pos];
		if (level == 0) continue;

		bool f = (color == LED_faded_red) || (color == LED_faded_green);
		bool g = (color == LED_green) || (color == LED_faded_green); //(led_phase <= 1) && ((color == LED_green) || (color == LED_faded_green));
		bool r = (color == LED_red)   || (color == LED_faded_red);   //(led_phase <= 1) && ((color == LED_red)   || (color == LED_faded_red));

		//g = g || (((led_phase & 1) == 0) && (color == LED_yellow));
		//r = r || (((led_phase & 1) == 1) && (color == LED_yellow));

		g = g && ((led_phase <= 32) || (!f));
		r = r && ((led_phase <= 32) || (!f));

		g = g && (led_pos < level);
		r = r && (led_pos < level);

		pin_write(PIN_LEDS_G[pos], g);
		pin_write(PIN_LEDS_R[pos], r);
	}

	pin_write(LED_STAT, led_status != 0);
}

void leds_init()
{
	systick_on(SECOND_FREQ, led_tick);

	pin_output_v(LED_STAT, 0);

	AFIO->MAPR |= 1 << 15;
	for (uint32_t pos = 0; pos < 8; pos++)
	{
		pin_output_v(PIN_LEDS_G[pos], 0);
		pin_output_v(PIN_LEDS_R[pos], 0);
	}
}

void leds_demo()
{
	printf("leds_demo(): ");
	led_status = 1;	delay_ms(100);
	led_status = 0;	delay_ms(100);

	led_status = 1;	delay_ms(100);
	led_status = 0;	delay_ms(100);

	led_status = 1;	delay_ms(100);
	led_status = 0;	delay_ms(100);

	for (uint32_t pos = 0; pos < 8; pos++)
		leds_level[pos] = 8;

	for (uint32_t cycle = 0; cycle < 3; cycle++)
	{
		for (uint32_t pos = 0; pos < 8; pos++)	leds_color[pos] = LED_red;    delay_ms(500);
		for (uint32_t pos = 0; pos < 8; pos++)	leds_color[pos] = LED_green;  delay_ms(500);
	}

	for (uint32_t pos = 0; pos < 8; pos++)
		leds_color[pos] = LED_off;
	second_cnt = 0;
	seconds = 0;
	second_cnt = 0;
	seconds = 0;
	printf("OK\r\n");
}

void wait_second()
{
	while (tick_second == false)
		;
	tick_second = false;
}
