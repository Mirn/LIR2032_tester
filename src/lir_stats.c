/*
 * lir_capacity.c
 *
 *  Created on: 29 ���. 2016 �.
 *      Author: �������
 */

#include "stm32kiss.h"
#include "lir_stats.h"

#define LOAD_R_Ohm 250

void stat_init(tLIR_stats *stat)
{
	if (stat == NULL)
		return;

	memset(stat, 0, sizeof(*stat));
	capacity_init(&stat->capacity);
}

void stat_begin_add(tLIR_stats *stat, uint16_t mV)
{
	if (stat->intervals_count >= LENGTH(stat->intervals))
		return;

	stat->intervals[stat->intervals_count].begin = mV;
}

void stat_end_add(tLIR_stats *stat, uint16_t mV, uint16_t time_sec)
{
	if (stat->intervals_count >= LENGTH(stat->intervals))
		return;

	stat->intervals[stat->intervals_count].end = mV;
	stat->intervals[stat->intervals_count].time_sec = time_sec;
	stat->intervals_count++;
}

void stat_print(tLIR_stats *stat)
{
	printf("capacity\t%i\tuA_per_hour\r\n", stat->capacity.uA_per_hour);

	for (uint32_t pos = 0; pos < stat->intervals_count; pos++)
		printf("Interval #%i\t%i\t%i\t%i\r\n", pos, stat->intervals[pos].time_sec, stat->intervals[pos].begin, stat->intervals[pos].end);
	printf("\r\n");
}

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
