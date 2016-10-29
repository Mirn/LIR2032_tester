/*
 * lir_capacity.c
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: Евгений
 */

#include "stm32kiss.h"

#define LOAD_R_Ohm 250

#include "test_discharge_const.h"

void capacity_calc_test()
{
	uint32_t integral_uA = 0;
	for (uint32_t pos = 0; pos < LENGTH(test_discharge_mV); pos++)
	{
		uint32_t uV = test_discharge_mV[pos] * 1000;
		uint32_t uA = uV / LOAD_R_Ohm;
		integral_uA += uA;
	}

	printf("void capacity_test():\t%i\t%i\r\n\r\n", integral_uA, integral_uA / 3600);
}
