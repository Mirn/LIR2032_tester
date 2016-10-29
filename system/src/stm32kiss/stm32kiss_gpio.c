#include "stm32kiss.h"

KS_PORT_CONTEXT __PORTA_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTB_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTC_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTD_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTE_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTF_CONTEXT = {clock_enabled_count:0};
KS_PORT_CONTEXT __PORTG_CONTEXT = {clock_enabled_count:0};

const KS_PORT __PORTA = {
		context            : PORTA_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOA,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTB = {
		context            : PORTB_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOB,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTC = {
		context            : PORTC_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOC,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTD = {
		context            : PORTD_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOD,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTE = {
		context            : PORTE_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOE,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTF = {
		context            : PORTF_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOF,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PORT __PORTG = {
		context            : PORTG_CONTEXT,
		clock_enable_const : RCC_APB2Periph_GPIOG,
		RCC_PeriphClockCmd : RCC_APB2PeriphClockCmd
};

const KS_PIN __PIN_A0  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_0 , ctrl: &GPIOA->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_A1  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_1 , ctrl: &GPIOA->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_A2  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_2 , ctrl: &GPIOA->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_A3  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_3 , ctrl: &GPIOA->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_A4  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_4 , ctrl: &GPIOA->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_A5  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_5 , ctrl: &GPIOA->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_A6  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_6 , ctrl: &GPIOA->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_A7  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_7 , ctrl: &GPIOA->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_A8  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_8 , ctrl: &GPIOA->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_A9  = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_9 , ctrl: &GPIOA->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_A10 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_10, ctrl: &GPIOA->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_A11 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_11, ctrl: &GPIOA->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_A12 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_12, ctrl: &GPIOA->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_A13 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_13, ctrl: &GPIOA->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_A14 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_14, ctrl: &GPIOA->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_A15 = {GPIO: GPIOA,  port: PORTA, mask: GPIO_Pin_15, ctrl: &GPIOA->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_B0  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_0 , ctrl: &GPIOB->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_B1  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_1 , ctrl: &GPIOB->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_B2  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_2 , ctrl: &GPIOB->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_B3  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_3 , ctrl: &GPIOB->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_B4  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_4 , ctrl: &GPIOB->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_B5  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_5 , ctrl: &GPIOB->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_B6  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_6 , ctrl: &GPIOB->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_B7  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_7 , ctrl: &GPIOB->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_B8  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_8 , ctrl: &GPIOB->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_B9  = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_9 , ctrl: &GPIOB->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_B10 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_10, ctrl: &GPIOB->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_B11 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_11, ctrl: &GPIOB->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_B12 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_12, ctrl: &GPIOB->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_B13 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_13, ctrl: &GPIOB->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_B14 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_14, ctrl: &GPIOB->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_B15 = {GPIO: GPIOB,  port: PORTB, mask: GPIO_Pin_15, ctrl: &GPIOB->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_C0  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_0 , ctrl: &GPIOC->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_C1  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_1 , ctrl: &GPIOC->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_C2  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_2 , ctrl: &GPIOC->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_C3  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_3 , ctrl: &GPIOC->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_C4  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_4 , ctrl: &GPIOC->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_C5  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_5 , ctrl: &GPIOC->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_C6  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_6 , ctrl: &GPIOC->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_C7  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_7 , ctrl: &GPIOC->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_C8  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_8 , ctrl: &GPIOC->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_C9  = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_9 , ctrl: &GPIOC->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_C10 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_10, ctrl: &GPIOC->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_C11 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_11, ctrl: &GPIOC->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_C12 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_12, ctrl: &GPIOC->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_C13 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_13, ctrl: &GPIOC->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_C14 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_14, ctrl: &GPIOC->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_C15 = {GPIO: GPIOC,  port: PORTC, mask: GPIO_Pin_15, ctrl: &GPIOC->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_D0  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_0 , ctrl: &GPIOD->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_D1  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_1 , ctrl: &GPIOD->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_D2  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_2 , ctrl: &GPIOD->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_D3  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_3 , ctrl: &GPIOD->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_D4  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_4 , ctrl: &GPIOD->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_D5  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_5 , ctrl: &GPIOD->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_D6  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_6 , ctrl: &GPIOD->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_D7  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_7 , ctrl: &GPIOD->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_D8  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_8 , ctrl: &GPIOD->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_D9  = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_9 , ctrl: &GPIOD->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_D10 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_10, ctrl: &GPIOD->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_D11 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_11, ctrl: &GPIOD->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_D12 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_12, ctrl: &GPIOD->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_D13 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_13, ctrl: &GPIOD->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_D14 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_14, ctrl: &GPIOD->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_D15 = {GPIO: GPIOD,  port: PORTD, mask: GPIO_Pin_15, ctrl: &GPIOD->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_E0  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_0 , ctrl: &GPIOE->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_E1  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_1 , ctrl: &GPIOE->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_E2  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_2 , ctrl: &GPIOE->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_E3  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_3 , ctrl: &GPIOE->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_E4  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_4 , ctrl: &GPIOE->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_E5  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_5 , ctrl: &GPIOE->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_E6  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_6 , ctrl: &GPIOE->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_E7  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_7 , ctrl: &GPIOE->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_E8  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_8 , ctrl: &GPIOE->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_E9  = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_9 , ctrl: &GPIOE->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_E10 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_10, ctrl: &GPIOE->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_E11 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_11, ctrl: &GPIOE->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_E12 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_12, ctrl: &GPIOE->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_E13 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_13, ctrl: &GPIOE->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_E14 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_14, ctrl: &GPIOE->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_E15 = {GPIO: GPIOE,  port: PORTE, mask: GPIO_Pin_15, ctrl: &GPIOE->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_F0  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_0 , ctrl: &GPIOF->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_F1  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_1 , ctrl: &GPIOF->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_F2  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_2 , ctrl: &GPIOF->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_F3  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_3 , ctrl: &GPIOF->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_F4  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_4 , ctrl: &GPIOF->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_F5  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_5 , ctrl: &GPIOF->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_F6  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_6 , ctrl: &GPIOF->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_F7  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_7 , ctrl: &GPIOF->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_F8  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_8 , ctrl: &GPIOF->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_F9  = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_9 , ctrl: &GPIOF->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_F10 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_10, ctrl: &GPIOF->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_F11 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_11, ctrl: &GPIOF->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_F12 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_12, ctrl: &GPIOF->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_F13 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_13, ctrl: &GPIOF->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_F14 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_14, ctrl: &GPIOF->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_F15 = {GPIO: GPIOF,  port: PORTF, mask: GPIO_Pin_15, ctrl: &GPIOF->CRH, cfg: (7 * 4)};

const KS_PIN __PIN_G0  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_0 , ctrl: &GPIOG->CRL, cfg: (0 * 4)};
const KS_PIN __PIN_G1  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_1 , ctrl: &GPIOG->CRL, cfg: (1 * 4)};
const KS_PIN __PIN_G2  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_2 , ctrl: &GPIOG->CRL, cfg: (2 * 4)};
const KS_PIN __PIN_G3  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_3 , ctrl: &GPIOG->CRL, cfg: (3 * 4)};
const KS_PIN __PIN_G4  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_4 , ctrl: &GPIOG->CRL, cfg: (4 * 4)};
const KS_PIN __PIN_G5  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_5 , ctrl: &GPIOG->CRL, cfg: (5 * 4)};
const KS_PIN __PIN_G6  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_6 , ctrl: &GPIOG->CRL, cfg: (6 * 4)};
const KS_PIN __PIN_G7  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_7 , ctrl: &GPIOG->CRL, cfg: (7 * 4)};
const KS_PIN __PIN_G8  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_8 , ctrl: &GPIOG->CRH, cfg: (0 * 4)};
const KS_PIN __PIN_G9  = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_9 , ctrl: &GPIOG->CRH, cfg: (1 * 4)};
const KS_PIN __PIN_G10 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_10, ctrl: &GPIOG->CRH, cfg: (2 * 4)};
const KS_PIN __PIN_G11 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_11, ctrl: &GPIOG->CRH, cfg: (3 * 4)};
const KS_PIN __PIN_G12 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_12, ctrl: &GPIOG->CRH, cfg: (4 * 4)};
const KS_PIN __PIN_G13 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_13, ctrl: &GPIOG->CRH, cfg: (5 * 4)};
const KS_PIN __PIN_G14 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_14, ctrl: &GPIOG->CRH, cfg: (6 * 4)};
const KS_PIN __PIN_G15 = {GPIO: GPIOG,  port: PORTG, mask: GPIO_Pin_15, ctrl: &GPIOG->CRH, cfg: (7 * 4)};

void pin_enable_clock(const KS_PIN *pin)
{
	pin->port->context->clock_enabled_count |= pin->mask;
	(*pin->port->RCC_PeriphClockCmd)(pin->port->clock_enable_const, ENABLE);
}

void pin_mode(const KS_PIN *pin, GPIOMode_TypeDef pin_mode)
{
	if (pin == NULL) return;
	pin_enable_clock(pin);

    GPIO_InitTypeDef GPIO_InitStructure = {
    		.GPIO_Mode  = pin_mode,
    		.GPIO_Speed = GPIO_Speed_50MHz,
    		.GPIO_Pin   = pin->mask
    };

    GPIO_Init(pin->GPIO, &GPIO_InitStructure);
}

void pin_disable(const KS_PIN *pin)
{
	if (pin == NULL) return;

	pin_mode(pin, GPIO_Mode_IN_FLOATING);

	pin->port->context->clock_enabled_count &= pin->mask;
	if (pin->port->context->clock_enabled_count == 0)
		(*pin->port->RCC_PeriphClockCmd)(pin->port->clock_enable_const, DISABLE);
}

/*void fast_pin_mode(const KS_PIN *pin, uint32_t mode)
{
	uint32_t tmp = *pin->ctrl;
	tmp &= ~(0xF << (pin->cfg));
	tmp |= (mode << (pin->cfg));
	*pin->ctrl = tmp;
}
*/
