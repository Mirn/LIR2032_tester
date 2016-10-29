/*
 * lir_worker.c
 *
 *  Created on: 27 окт. 2016 г.
 *      Author: Евгений
 */
#include "stm32kiss.h"
#include "lir_worker.h"

#ifdef UNUSED
#undef UNUSED
#endif
#define UNUSED(x) (void)(x)

#define LIMIT_mV_LO 3000
#define LIMIT_mV_HI 4300

tworker_result wait_charge(const uint16_t mV, const uint16_t limit, const bool charge_done, const uint32_t time);

const tLIR_worker_state STATE_DONE = {
		.mode = LIR_free,
		.info = '.',
		.next = NULL
};

const tLIR_worker_state STATE_ERROR = {
		.mode = LIR_free,
		.info = 'e',
		.next = NULL
};

const tLIR_worker_state STATE_CHARGE_FIRST = {
		.mode          = LIR_charge,
		.limit         = 4300,
		.timeout_wait  = 60*60,
		.timeout_done  = 5,
		.timeout_error = 5,
		.func_check    = wait_charge,
		.info          = 'c',
		.next          = &STATE_DONE
};

const struct LIR_worker_state *STATE_FIRST = &STATE_CHARGE_FIRST;

tworker_result wait_charge(uint16_t mV, uint16_t limit, bool charge_done, uint32_t time)
{
	(void)(limit);
	(void)(time);

	if (charge_done) return WAIT_DONE;

	if (mV > LIMIT_mV_HI) return WAIT_ERROR;
	if (mV < LIMIT_mV_LO) return WAIT_ERROR;

	return WAIT_CONTINUE;
}

tLIR_Mode lir_worker_init(tLIR_worker *worker)
{
	worker->done_cnt = 0;
	worker->error_cnt = 0;
	worker->state = STATE_FIRST;
	worker->time_current = 0;
	worker->time_total = 0;

	if (worker->state == NULL)
		return LIR_free;
	else
		return worker->state->mode;
}

tLIR_Mode lir_worker_run(tLIR_worker * const worker, const uint16_t mV, const bool charge_done)
{
	if (worker->state == NULL)
		return LIR_free;

	if (worker->state->next != NULL)
	{
		const tLIR_worker_state *state = worker->state;

		tworker_result wait_result = WAIT_ERROR;
		tworker_func   wait_checker = state->func_check;

		if (wait_checker != NULL)
			wait_result = (*wait_checker)(mV, state->limit, charge_done, worker->time_current);

		if (wait_result != WAIT_CONTINUE)
			worker->time_current = 0;

		if (wait_result == WAIT_CONTINUE)
			if (worker->time_current > state->timeout_wait)
				worker->state = &STATE_ERROR;

		if (wait_result == WAIT_ERROR)
		{
			worker->error_cnt++;
			if (worker->error_cnt >= state->timeout_error)
				worker->state = &STATE_ERROR;
		}
		else
			worker->error_cnt = 0;

		if (wait_result == WAIT_DONE)
		{
			worker->done_cnt++;
			if (worker->done_cnt >= state->timeout_done)
				worker->state = state->next;
		}
		else
			worker->done_cnt = 0;
	}

	worker->time_current++;
	worker->time_total++;
	return worker->state->mode;
}
