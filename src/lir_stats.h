/*
 * lir_capacity.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: Евгений
 */

#ifndef LIR_STATS_H_
#define LIR_STATS_H_

typedef struct {
	uint32_t integral_uA;
	uint32_t uA_per_hour;
} tLIR_capacity;

typedef struct {
	tLIR_capacity capacity;
	uint16_t mV_begin;
	uint16_t mV_end;
	uint16_t time;
} tLIR_stats;

void capacity_init(tLIR_capacity *stat);
void capacity_calc(tLIR_capacity *stat, uint16_t mV);

void capacity_calc_test();

#endif /* LIR_CAPACITY_H_ */
