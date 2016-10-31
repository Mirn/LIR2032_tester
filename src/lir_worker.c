/*
 * lir_worker.c
 *
 *  Created on: 27 окт. 2016 г.
 *      Author: Евгений
 */
#include "stm32kiss.h"
#include "lir_worker.h"

//#ifdef UNUSED
//#undef UNUSED
//#endif
//#define UNUSED(x) (void)(x)

#define LIMIT_mV_LO 3000
#define LIMIT_mV_HI 4300

tworker_result wait_charge(const uint16_t mV, const uint16_t limit, const bool charge_done, const uint32_t time, tLIR_stats * const stats);
tworker_result wait_discharge(const uint16_t mV, const uint16_t limit, const bool charge_done, const uint32_t time, tLIR_stats * const stats);

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

const tLIR_worker_state STATE_CHARGE_SECOND = {
		.mode          = LIR_charge,
		.wait_max      = 60*60,
		.wait_min      = 10*60,
		.timeout_done  = 3,
		.timeout_error = 5,
		.func_check    = wait_charge,
		.info          = 'F',
		.stats         = NULL,
		.next          = &STATE_DONE
};

const tLIR_worker_state STATE_DISCHARGE_FIRST = {
		.mode          = LIR_load,
		.limit         = 4000,
		.wait_max      = 60*60,
		.wait_min      = 20*60,
		.timeout_done  = 3,
		.timeout_error = 5,
		.func_check    = wait_discharge,
		.info          = 'L',
		.stats         = NULL,
		.next          = &STATE_CHARGE_SECOND
};

const tLIR_worker_state STATE_CHARGE_FIRST = {
		.mode          = LIR_charge,
		.wait_max      = 60*60,
		.wait_min      = 0,
		.timeout_done  = 3,
		.timeout_error = 5,
		.func_check    = wait_charge,
		.info          = 'c',
		.stats         = NULL,
		.next          = &STATE_DISCHARGE_FIRST
};

const struct LIR_worker_state *STATE_FIRST = &STATE_CHARGE_FIRST;

tworker_result wait_charge(uint16_t mV, UNUSED uint16_t limit, bool charge_done, UNUSED uint32_t time, UNUSED tLIR_stats * const stats)
{
	if (charge_done) return WAIT_DONE;

	if (mV > LIMIT_mV_HI) return WAIT_ERROR;
	if (mV < LIMIT_mV_LO) return WAIT_ERROR;

	return WAIT_CONTINUE;
}

tworker_result wait_discharge(uint16_t mV, uint16_t limit, bool charge_done, UNUSED uint32_t time, UNUSED tLIR_stats * const stats)
{
	if (charge_done) return WAIT_ERROR;

	if (mV > LIMIT_mV_HI) return WAIT_ERROR;
	if (mV < limit) return WAIT_DONE;

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

	const tLIR_worker_state *state = worker->state;

	tworker_result wait_result = WAIT_ERROR;
	tworker_func   wait_checker = state->func_check;

	if (worker->state->next != NULL)
	{
		if (wait_checker != NULL)
			wait_result = (*wait_checker)(mV, state->limit, charge_done, worker->time_current, state->stats);

		if (wait_result == WAIT_CONTINUE)
			if (worker->time_current > state->wait_max)
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
			{
				if (worker->time_current < state->wait_min)
					worker->state = &STATE_ERROR;
				else
				{
					if (worker->state->stats != NULL)
					{
						worker->state->stats->time = worker->time_current;
						worker->state->stats->mV_end = mV;
					}

					worker->state = state->next;

					if (worker->state->stats != NULL)
						capacity_init(&worker->state->stats->capacity);
				}
			}
		}
		else
			worker->done_cnt = 0;
	}

	if (wait_result != WAIT_CONTINUE)
		worker->time_current = 0;
	else
	{
		if (worker->time_current == 0)
			if (worker->state->stats != NULL)
				worker->state->stats->mV_begin = mV;

		if (worker->state->stats != NULL)
			capacity_calc(&worker->state->stats->capacity, mV);

		worker->time_current++;
	}

	worker->time_total++;
	return worker->state->mode;
}
