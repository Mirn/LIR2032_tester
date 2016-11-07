/*
 * lir_ctrl.h
 *
 *  Created on: 26 ���. 2016 �.
 *      Author: �������
 */

#ifndef LIR_CTRL_H_
#define LIR_CTRL_H_

#include "lir_mode.h"

typedef enum
{
	Charge_NC = 0,
	Charge_Process = 1,
	Charge_Complete = 2,
	Charge_Error = 3

} tCharge_Status;

extern tLIR_Mode lir_ctrl[8];
extern tCharge_Status lir_status[8];
extern uint32_t lir_uV[8];
extern uint16_t lir_mV[8];
extern uint16_t vref_mV;

void lir_ctrl_init();
void lir_ctrl_update();
void lir_info_update();
void lir_printf_ctrl();
void lir_printf_status();

#endif /* LIR_CTRL_H_ */
