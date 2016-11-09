/*
 * lir_ctrl.c
 *
 *  Created on: 26 окт. 2016 г.
 *      Author: Евгений
 */

#include "stm32kiss.h"
#include "lir_ctrl.h"
#include "simple_filts.h"

const char src_ver_lir_ctrl[] = __DATE__"\t"__TIME__"\t"__FILE__"\r";

const KS_PIN * const PIN_CHARGE[8] = {
		PIN_A5,
		PIN_A6,
		PIN_A7,
		PIN_C4,
		PIN_C5,
		PIN_B0,
		PIN_B1,
		PIN_B2,
};

const KS_PIN * const PIN_LOAD[8] = {
		PIN_C8,
		PIN_C9,
		PIN_A8,
		PIN_A11,
		PIN_A12,
		PIN_A13,
		PIN_A14,
		PIN_A15,
};

const KS_PIN * const PIN_STATUS[8] = {
		PIN_B10,
		PIN_B11,
		PIN_B12,
		PIN_B13,
		PIN_B14,
		PIN_B15,
		PIN_C6,
		PIN_C7,
};

#define AVRG_LENGTH  64
#define RC_TAU       16

static uint32_t avrg_bufs[8][AVRG_LENGTH];
static tfilt_avrg avrg_filts[8] = {
		{0, 0, &(avrg_bufs[0][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[1][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[2][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[3][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[4][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[5][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[6][0]), AVRG_LENGTH},
		{0, 0, &(avrg_bufs[7][0]), AVRG_LENGTH},
};
static tfilt_rc rc_filts[8] = {
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
		{0, RC_TAU},
};

tLIR_Mode lir_ctrl[8];
tCharge_Status lir_status[8];
uint32_t lir_uV_filted[8];
 int32_t lir_uV_delta[8];
uint32_t lir_uV[8];
uint16_t lir_mV[8];
uint16_t vref_mV;

static tCharge_Status charge_status_read(const KS_PIN * const pin, const bool charge)
{
	if (!charge)
		return Charge_NC;

	if (pin_read(pin) == true)
		return Charge_Complete;
	else
		return Charge_Process;
}

static uint32_t lir_uV_read(const uint8_t num)
{
	const uint8_t chanels[8] = {10, 11, 12, 13, 1, 2, 3, 4};
	adc_chanel(chanels[num]);
	adc_read();

	uint64_t result = 0;
	const uint32_t cnt = 0x100;

	uint32_t pos = cnt;
	while (pos--)
		result += adc_read();

	result = result * 3300ULL;
	result = result * 1000ULL;
	result = result / (65520ULL / 2ULL);
	result = result / cnt;
	return result;
}


static uint16_t vref_mV_read()
{
	adc_chanel(ADC_Channel_Vrefint);
	adc_read();

	uint32_t result = 0;
	const uint32_t cnt = 0x100;

	uint32_t pos = cnt;
	while (pos--)
		result += adc_read();

	return 	(23825 * 3300) / (result / cnt); //(23825.454545454548 / (result / cnt)) * 3300.0f;//
}

static void lir_delta_calc(uint32_t pos, uint32_t uV)
{
	uV = filt_rc_calc(&(rc_filts[pos]), uV);
	uV = filt_avrg_calc(&(avrg_filts[pos]), uV);

	lir_uV_delta[pos] = uV - lir_uV_filted[pos];
	lir_uV_filted[pos] = uV;
}

void lir_ctrl_init()
{
	printf("lir_ctrl_init(): ");
	adc_on(ADC_Channel_1, ADC_SampleTime_13Cycles5);

	for (uint32_t pos = 0; pos < 8; pos++)
	{
		lir_ctrl[pos] = LIR_free;
		lir_status[pos] = Charge_NC;
		pin_output_v(PIN_CHARGE[pos], 0);
		pin_output_v(PIN_LOAD[pos], 0);
		pin_input_up(PIN_STATUS[pos]);

		delay_ms(100);

		lir_uV[pos] = lir_uV_read(pos);
		lir_mV[pos] = lir_uV[pos] / 1000;

		filt_avrg_init(&(avrg_filts[pos]));
		filt_rc_init(&(rc_filts[pos]), lir_uV[pos]);

		for (uint32_t i = 0; i < (AVRG_LENGTH + (RC_TAU * 3)); i++)
			lir_delta_calc(pos, lir_uV_read(pos));
	}
	printf("OK\r\n");
}

void lir_info_update()
{
	vref_mV = vref_mV_read();
	for (uint32_t pos = 0; pos < 8; pos++)
	{
		lir_uV[pos] = lir_uV_read(pos);
		lir_mV[pos] = lir_uV[pos] / 1000;
		lir_status[pos] = charge_status_read(PIN_STATUS[pos], pin_read(PIN_CHARGE[pos]));

		lir_delta_calc(pos, lir_uV[pos]);
	}
}

void lir_ctrl_update()
{
	for (uint32_t pos = 0; pos < 8; pos++)
	{
		pin_write(PIN_LOAD[pos], lir_ctrl[pos] == LIR_load);
		pin_write(PIN_CHARGE[pos], lir_ctrl[pos] == LIR_charge);
	}
}

void lir_printf_status()
{
	char info[9] = {0};
	for (uint32_t pos = 0; pos < 8; pos++)
	{
		tCharge_Status status = lir_status[pos];
		char c = ' ';
		if (status == Charge_Error)    c = 'E';
		if (status == Charge_NC)       c = '_';
		if (status == Charge_Process)  c = ':';
		if (status == Charge_Complete) c = '#';
		info[pos] = c;
	}
	printf("%s", info);
}

void lir_printf_ctrl()
{
	char info[9] = "________";
	for (uint32_t pos = 0; pos < 8; pos++)
	{
		if (lir_ctrl[pos] == LIR_load)   info[pos] = (lir_mV[pos] > 1000) ? 'L' : '_';
		if (lir_ctrl[pos] == LIR_charge) info[pos] = (lir_mV[pos] > 1000) ? 'C' : '_';
	}
	printf("%s", info);
}
