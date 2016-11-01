#include "stm32kiss.h"
#include "lir_leds.h"
#include "lir_ctrl.h"
#include "lir_worker.h"

tLIR_worker lir_workers[8];

void main(void)
{
	printf("\r\n\r\n========================================\r\nLIR2032 TESTER program started\r\n\r\n");
	printf("SystemCoreClock = %i Hz\r\n", SystemCoreClock);
	printf("Chip ID number: %x %x %x %x; %x\r\n", DEVICE_ID_1, DEVICE_ID_2, DEVICE_ID_3, DEVICE_ID_4, DEVICE_ID_HASH);
	printf("\r\n");

	//capacity_calc_test();

	leds_init();
	lir_ctrl_init();

	leds_demo();

	for (uint32_t pos = 0; pos < 8; pos++)
		lir_ctrl[pos] = lir_worker_init(&(lir_workers[pos]));

	uint32_t time = DWT_CYCCNT;
	while (true)
	{
		lir_info_update();

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
			printf("%i ", lir_workers[pos].time_current);

		printf("\r\n");

		for (uint32_t pos = 0; pos < 8; pos++)
			lir_ctrl[pos] = lir_worker_run(&(lir_workers[pos]), lir_mV[pos], lir_status[pos] == Charge_Complete);

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
