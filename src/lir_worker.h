/*
 * lir_worker.h
 *
 *  Created on: 27 окт. 2016 г.
 *      Author: Евгений
 */

#ifndef LIR_WORKER_H_
#define LIR_WORKER_H_

#include "lir_leds.h"
#include "lir_mode.h"
#include "lir_stats.h"

extern const char src_ver_lir_worker[];

typedef enum {
	WAIT_CONTINUE = 0,
	WAIT_DONE = 1,
	WAIT_ERROR = 0xFF
} tworker_result;

typedef tworker_result (*tworker_func)(const uint16_t mV, const uint32_t param_limit, const bool charge_done, uint8_t *led_level);

typedef struct LIR_worker_state {
	const char *name;
	tLIR_Mode mode;
	tCOLOR  led_color;
	uint32_t wait_min;
	uint32_t wait_max;
	uint32_t param_limit;
	bool     cap_reg;
	char     info;
	tworker_func func_check;
	const struct LIR_worker_state *next;
} tLIR_worker_state;

typedef struct {
	uint32_t time_total;
	uint32_t time_current;
	uint16_t done_cnt;
	uint16_t error_cnt;
	tLIR_stats info_stats;
	tCOLOR  led_color;
	uint8_t led_level;
	bool done;
	bool error;
	const tLIR_worker_state *state;
} tLIR_worker;

tLIR_Mode lir_worker_init(tLIR_worker *worker, const char *title);
tLIR_Mode lir_worker_run(tLIR_worker *worker, uint16_t mV, bool charge_done);

#endif /* LIR_WORKER_H_ */
