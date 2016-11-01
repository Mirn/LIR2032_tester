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

#define TIMEOUT_ERROR 5
#define TIMEOUT_DONE  3
#define TIMEOUT_TOTAL (8*60*60)

tworker_result wait_charge(const uint16_t mV, const uint16_t limit, const bool charge_done);
tworker_result wait_discharge(const uint16_t mV, const uint16_t limit, const bool charge_done);

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
		.wait_max      = 2*60*60,
		.wait_min      = 10*60,
		.func_check    = wait_charge,
		.cap_reg       = false,
		.info          = 'f',
		.next          = &STATE_DONE
};

const tLIR_worker_state STATE_DISCHARGE_FIRST = {
		.mode          = LIR_load,
		.limit         = 3300,
		.wait_max      = 3*60*60,
		.wait_min      = 5*60,
		.func_check    = wait_discharge,
		.cap_reg       = true,
		.info          = 'L',
		.next          = &STATE_CHARGE_SECOND
};

const tLIR_worker_state STATE_CHARGE_FIRST = {
		.mode          = LIR_charge,
		.wait_max      = 60*60,
		.wait_min      = 0,
		.func_check    = wait_charge,
		.cap_reg       = false,
		.info          = 'c',
		.next          = &STATE_DISCHARGE_FIRST
};

const struct LIR_worker_state *STATE_FIRST = &STATE_CHARGE_FIRST;

tworker_result wait_charge(uint16_t mV, UNUSED uint16_t limit, bool charge_done)
{
	if (charge_done) return WAIT_DONE;

	if (mV > LIMIT_mV_HI) return WAIT_ERROR;
	if (mV < LIMIT_mV_LO) return WAIT_ERROR;

	return WAIT_CONTINUE;
}

tworker_result wait_discharge(uint16_t mV, uint16_t limit, bool charge_done)
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

	stat_init(&worker->info_stats);

	if (worker->state == NULL)
		return LIR_free;
	else
		return worker->state->mode;
}

tLIR_Mode lir_worker_run(tLIR_worker * const worker, const uint16_t mV, const bool charge_done)
{
	if (worker->state == NULL)
		return LIR_free;

	if (worker->state->next == NULL)
		return LIR_free;

	const tLIR_worker_state *state = worker->state;

	bool error = false;
	bool next  = false;

	if (worker->time_current > state->wait_max)
		error = true;

	if (worker->time_total > TIMEOUT_TOTAL)
		error = true;

	tworker_result wait_result = WAIT_ERROR;
	tworker_func   wait_checker = state->func_check;

	if (wait_checker != NULL)
		wait_result = (*wait_checker)(mV, state->limit, charge_done);

	if (wait_result == WAIT_CONTINUE)
	{
		worker->error_cnt = 0;
		worker->done_cnt = 0;
	}

	if (wait_result == WAIT_ERROR)
	{
		worker->error_cnt++;
		if (worker->error_cnt >= TIMEOUT_ERROR)
			error = true;
	}

	if (wait_result == WAIT_DONE)
	{
		worker->done_cnt++;
		if (worker->done_cnt >= TIMEOUT_DONE)
		{
			if (worker->time_current < state->wait_min)
				error = true;
			else
				next = true;
		}
	}

	if (error || next)
	{
		stat_end_add(&worker->info_stats, mV, worker->time_total);
		worker->time_current = 0;

		if (error)
			worker->state = &STATE_ERROR;
		else
			worker->state = state->next;

		if (worker->state->next == NULL)
			stat_print(&worker->info_stats);
	}
	else
	{
		if (worker->time_current == 0)
			stat_begin_add(&worker->info_stats, mV);

		if (state->cap_reg)
			capacity_calc(&worker->info_stats.capacity, mV);

		worker->time_current++;
	}

	worker->time_total++;
	return worker->state->mode;
}
