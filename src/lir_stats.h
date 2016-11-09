/*
 * lir_capacity.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: Евгений
 */

#ifndef LIR_STATS_H_
#define LIR_STATS_H_

extern const char src_ver_lir_stats[];

typedef struct {
	uint32_t integral_uA;
	uint32_t uA_per_hour;
} tLIR_capacity;

typedef struct {
	char name[16];
	uint32_t time_mark;
	uint16_t begin;
	uint16_t end;
} tStat_interval;

typedef struct {
	char title[64];
	tLIR_capacity capacity;
	tStat_interval intervals[8];
	uint8_t intervals_count;
} tLIR_stats;

void stat_init(tLIR_stats *stat, const char *title);
void stat_print(tLIR_stats *stat);
void stat_begin_add(tLIR_stats *stat, uint16_t mV, const char *name);
void stat_end_add(tLIR_stats *stat, uint16_t mV, uint16_t time_sec);

void capacity_init(tLIR_capacity *stat);
void capacity_calc(tLIR_capacity *stat, uint16_t mV);

void capacity_calc_test();

#endif /* LIR_CAPACITY_H_ */
