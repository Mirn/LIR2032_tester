#include "stm32kiss.h"
#include "lir_leds.h"
#include "lir_ctrl.h"
#include "lir_worker.h"

void log_flash_init();

const char *lir_names[8] = {
		"LIR_1",
		"LIR_2",
		"LIR_3",
		"LIR_4",
		"LIR_5",
		"LIR_6",
		"LIR_7",
		"LIR_8",
};
tLIR_worker lir_workers[8];

void report_status()
{
	static uint32_t time = 0;
	if (time == 0)
		time = DWT_CYCCNT;

	printf("%7i\t", seconds);
	lir_printf_status(); PrintChar(' ');
	lir_printf_ctrl(); PrintChar(' ');

	for (uint32_t pos = 0; pos < 8; pos++)
		PrintChar(lir_workers[pos].state->info);

	PrintChar('\t');
	for (uint32_t pos = 0; pos < 8; pos++)
		printf("%i\t", lir_mV[pos]);

	printf("%i\t", DWT_CYCCNT - time);
	time = DWT_CYCCNT;

	printf("%i\t", vref_mV);

	for (uint32_t pos = 0; pos < 8; pos++)
		printf("%i\t", lir_nV_delta[pos]);

	printf("\r\n");

}

void main(void)
{
	printf("\r\n\r\n========================================\r\nLIR2032 TESTER program started\r\n\r\n");
	printf("SystemCoreClock = %i Hz\r\n", SystemCoreClock);
	printf("Chip ID number: %x %x %x %x; %x\r\n", DEVICE_ID_1, DEVICE_ID_2, DEVICE_ID_3, DEVICE_ID_4, DEVICE_ID_HASH);
	printf("\r\n");

	//capacity_calc_test();

	log_flash_init();
	flash_log_enabled = true;
	printf("\r\nREBOOT\r\n\r\n");
	flash_log_enabled = false;

	leds_init();
	lir_ctrl_init();

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1leds_demo();

	for (uint32_t pos = 0; pos < 8; pos++)
		lir_ctrl[pos] = lir_worker_init(&(lir_workers[pos]), lir_names[pos]);

	uint32_t done_cnt_old = 0;
	uint32_t done_cnt_new = 0;
	while (true)
	{
		lir_info_update();

		if (done_cnt_new != 8)
			report_status();
		else
			if ((seconds % 100) == 0)
				report_status();

		for (uint32_t pos = 0; pos < 8; pos++)
		{
			lir_ctrl[pos] = lir_worker_run(&(lir_workers[pos]), lir_mV[pos], lir_status[pos] == Charge_Complete);
			leds_level[pos] = lir_workers[pos].led_level;
			leds_color[pos] = lir_workers[pos].led_color;
		}
		led_status = (done_cnt_new != 8);

		done_cnt_old = done_cnt_new;
		done_cnt_new = 0;
		for (uint32_t pos = 0; pos < 8; pos++)
			if (lir_workers[pos].done)
				done_cnt_new++;

		if ((done_cnt_new == 8) && (done_cnt_old < 8))
		{
			printf("\r\nALL DONE\r\n\r\n");
			for (uint32_t pos = 0; pos < 8; pos++)
				stat_print(&lir_workers[pos].info_stats);
		}


//		bool load   = false;//seconds <  3;
//		bool charge = seconds >  6;
//
//		for (uint32_t pos = 0; pos < 8; pos++)
//		{
//			lir_ctrl[pos] = LIR_free, leds_color[pos] = LED_off;
//			if (load   && (lir_mV[pos] > 1000)) lir_ctrl[pos] = LIR_load,   leds_color[pos] = LED_yellow;
//			if (charge && (lir_mV[pos] > 1000)) lir_ctrl[pos] = LIR_charge, leds_color[pos] = LED_green;
//			leds_color[pos] = LED_off;////////////////////////////////////////////////////////////////////////
//		}

		lir_ctrl_update();
		wait_second();
	}
}
