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
#define TIMEOUT_TOTAL UINT32_MAX //!(8*60*60)

#define CHARGE_LEVEL_BEGIN_mV 3300
#define CHARGE_LEVEL_STOP_mV  4200

#define PARAM_FIRST_CHARGE  false
#define PARAM_SECOND_CHARGE true

tworker_result wait_charge(const uint16_t mV, const uint32_t param, const bool charge_done, uint8_t *led_level);
tworker_result wait_discharge(const uint16_t mV, const uint32_t limit, const bool charge_done, uint8_t *led_level);

const tLIR_worker_state STATE_DONE = {
		.led_color = LED_green,
		.mode = LIR_free,
		.info = '.',
		.next = NULL
};

const tLIR_worker_state STATE_ERROR = {
		.led_color = LED_red,
		.mode = LIR_free,
		.info = 'e',
		.next = NULL
};

const tLIR_worker_state STATE_CHARGE_SECOND = {
		.name          = "Charge_second",
		.mode          = LIR_charge,
		.param_limit   = PARAM_SECOND_CHARGE,
		.wait_max      = 2*60*60,
		.wait_min      = 20*60,
		.led_color     = LED_green,
		.func_check    = wait_charge,
		.cap_reg       = false,
		.info          = 'f',
		.next          = &STATE_DONE
};

const tLIR_worker_state STATE_DISCHARGE_FIRST = {
		.name          = "Load_250Ohm  ",
		.mode          = LIR_load,
		.param_limit   = CHARGE_LEVEL_BEGIN_mV,
		.wait_max      = 4*60*60,
		.wait_min      = 1*60*60,
		.led_color     = LED_yellow,
		.func_check    = wait_discharge,
		.cap_reg       = true,
		.info          = 'L',
		.next          = &STATE_CHARGE_SECOND
};

const tLIR_worker_state STATE_DISCHARGE_CR2032 = {
		.name          = "Load_250Ohm  ",
		.mode          = LIR_load,
		.param_limit   = 1500,
		.wait_max      = UINT32_MAX,
		.wait_min      = 1*60*60,
		.led_color     = LED_yellow,
		.func_check    = wait_discharge,
		.cap_reg       = true,
		.info          = 'L',
		.next          = &STATE_DONE
};

const tLIR_worker_state STATE_CHARGE_FIRST = {
		.name          = "Charge_first ",
		.mode          = LIR_charge,
		.param_limit   = PARAM_FIRST_CHARGE,
		.wait_max      = 2*60*60,
		.wait_min      = 0,
		.led_color     = LED_green,
		.func_check    = wait_charge,
		.cap_reg       = false,
		.info          = 'c',
		.next          = &STATE_DISCHARGE_FIRST
};

//const struct LIR_worker_state *STATE_FIRST = &STATE_CHARGE_FIRST;
const struct LIR_worker_state *STATE_FIRST = &STATE_DISCHARGE_CR2032;

//static uint32_t cnt_c = 0;
//static uint32_t cnt_l = 0;

tworker_result wait_charge(uint16_t mV, UNUSED uint32_t param, bool charge_done, uint8_t *led_level)
{
//	cnt_c++;
//	if (cnt_c > (10*8 + 4)) return WAIT_DONE;

	if (param == PARAM_FIRST_CHARGE)
		*led_level = 1;

	if (param == PARAM_SECOND_CHARGE)
	{
		int32_t level = mV - CHARGE_LEVEL_BEGIN_mV;
		level = MAX(0, level);
		*led_level = 1 + ((level*6) / (CHARGE_LEVEL_STOP_mV - CHARGE_LEVEL_BEGIN_mV));
	}

	if (charge_done) return WAIT_DONE;

	if (mV > LIMIT_mV_HI) return WAIT_ERROR;
	if (mV < LIMIT_mV_LO) return WAIT_ERROR;

	return WAIT_CONTINUE;
}

tworker_result wait_discharge(uint16_t mV, uint32_t limit, bool charge_done, uint8_t *led_level)
{
//	cnt_l++;
//	if (cnt_l > (10*8 + 5))
//	{
//		cnt_c = 1;
//		return WAIT_DONE;
//	}

	int32_t level = mV - CHARGE_LEVEL_BEGIN_mV;
	level = MAX(0, level);
	*led_level = 1 + ((level*6) / (CHARGE_LEVEL_STOP_mV - CHARGE_LEVEL_BEGIN_mV));

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

	worker->led_color = worker->state->led_color;
	worker->error = (worker->state == &STATE_ERROR);
	worker->done  = (worker->state->next == NULL);

	if (worker->done)
	{
		worker->led_level = 255;
		return LIR_free;
	}

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
		wait_result = (*wait_checker)(mV, state->param_limit, charge_done, &worker->led_level);

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

		//if (worker->state->next == NULL)
		//	stat_print(&worker->info_stats);
	}
	else
	{
		if (worker->time_current == 0)
			stat_begin_add(&worker->info_stats, mV, state->name);

		if (state->cap_reg)
			capacity_calc(&worker->info_stats.capacity, mV);

		worker->time_current++;
	}

	worker->time_total++;
	return worker->state->mode;
}
