/*
 * lir_mode.h
 *
 *  Created on: 27 ���. 2016 �.
 *      Author: �������
 */

#ifndef LIR_MODE_H_
#define LIR_MODE_H_

#include "lir_leds.h"

typedef enum {
	LIR_free = 0,
	LIR_load = 1,
	LIR_charge = 2
} tLIR_Mode;

typedef struct {
	tCOLOR  color;
	uint8_t level;
} tLED_state;

#endif /* LIR_MODE_H_ */
