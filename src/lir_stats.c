/*
 * lir_capacity.c
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: Евгений
 */

#include "stm32kiss.h"
#include "lir_stats.h"

#define LOAD_R_Ohm 250

void capacity_init(tLIR_capacity *stat)
{
	stat->integral_uA = 0;
	stat->uA_per_hour = 0;
}

void capacity_calc(tLIR_capacity *stat, uint16_t mV)
{
	uint32_t uV = mV * 1000;
	uint32_t uA = uV / LOAD_R_Ohm;
	stat->integral_uA += uA;
	stat->uA_per_hour = stat->integral_uA / (60*60);
}

#include "test_discharge_const.h"

void capacity_calc_test()
{
	tLIR_capacity cap = {0};

	capacity_init(&cap);
	for (uint32_t pos = 0; pos < LENGTH(test_discharge_mV); pos++)
		capacity_calc(&cap, test_discharge_mV[pos]);

	printf("void capacity_test():\t%i\t%i\r\n\r\n", cap.integral_uA, cap.uA_per_hour);
}
