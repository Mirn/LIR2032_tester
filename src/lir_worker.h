/*
 * lir_worker.h
 *
 *  Created on: 27 окт. 2016 г.
 *      Author: Евгений
 */

#ifndef LIR_WORKER_H_
#define LIR_WORKER_H_

#include "lir_mode.h"
#include "lir_stats.h"

typedef enum {
	WAIT_CONTINUE = 0,
	WAIT_DONE = 1,
	WAIT_ERROR = 0xFF
} tworker_result;

typedef tworker_result (*tworker_func)(const uint16_t mV, const uint16_t limit, const bool charge_done, const uint32_t time, tLIR_stats * const stats);

typedef struct LIR_worker_state {
	tLIR_Mode mode;
	uint16_t wait_min;
	uint16_t wait_max;
	uint16_t timeout_done;
	uint16_t timeout_error;
	uint16_t limit;
	char     info;
	tworker_func func_check;
	tLIR_stats * const stats;
	const struct LIR_worker_state *next;
} tLIR_worker_state;

typedef struct {
	uint16_t time_total;
	uint16_t time_current;
	uint16_t done_cnt;
	uint16_t error_cnt;
	const tLIR_worker_state *state;
} tLIR_worker;

tLIR_Mode lir_worker_init(tLIR_worker *worker);
tLIR_Mode lir_worker_run(tLIR_worker *worker, uint16_t mV, bool charge_done);

#endif /* LIR_WORKER_H_ */
