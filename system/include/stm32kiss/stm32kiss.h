#ifndef __STM32KISS_H__
#define __STM32KISS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
//#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"

#define STRUCT_CLEAR(v) memset((void *)&v, 0, sizeof(v))
#define LENGTH(v) (sizeof(v) / sizeof(v[0]))

typedef struct
{
	int32_t min;
	int32_t max;
} tLIMIT;

#define AVRG(a,b) (((a) + (b))/2)
#define DELTA(a,b, scale) ((MAX(a, b) - MIN(a, b))*scale) / AVRG(a, b)
#define IN_LIMIT(value, limit) ((limit.min <= (value)) && ((value) <= limit.max))

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define RANGE(v, a, b) MAX(a, MIN(b, v))

#pragma GCC diagnostic ignored "-Wformat"

#define PI 3.1415926535897932384626433832795

void PrintChar(char c);

#include "stm32kiss_adc.h"
#include "stm32kiss_dac.h"
#include "stm32kiss_gpio.h"
#include "stm32kiss_ticks.h"
#include "stm32kiss_button.h"
#include "stm32kiss_fifo.h"
#include "stm32kiss_dma_usarts.h"

#define UNUSED __attribute__((unused))

#define FLASH_SIZE_BASE  0x1FFFF7E0
#define FLASH_SIZE_PTR   ((uint16_t *)(FLASH_SIZE_BASE))
#define FLASH_SIZE       (*FLASH_SIZE_PTR)

#define DEVICE_ID_1_BASE  0x1FFFF7E8
#define DEVICE_ID_2_BASE  DEVICE_ID_1_BASE+2
#define DEVICE_ID_3_BASE  DEVICE_ID_1_BASE+4
#define DEVICE_ID_4_BASE  DEVICE_ID_1_BASE+8
#define DEVICE_ID_1_PTR   ((uint16_t *)(DEVICE_ID_1_BASE))
#define DEVICE_ID_2_PTR   ((uint16_t *)(DEVICE_ID_2_BASE))
#define DEVICE_ID_3_PTR   ((uint32_t *)(DEVICE_ID_3_BASE))
#define DEVICE_ID_4_PTR   ((uint32_t *)(DEVICE_ID_4_BASE))
#define DEVICE_ID_1       (*DEVICE_ID_1_PTR)
#define DEVICE_ID_2       (*DEVICE_ID_2_PTR)
#define DEVICE_ID_3       (*DEVICE_ID_3_PTR)
#define DEVICE_ID_4       (*DEVICE_ID_4_PTR)
#define DEVICE_ID_HASH (DEVICE_ID_1 ^ DEVICE_ID_2 ^ DEVICE_ID_3 ^ DEVICE_ID_4)

#endif //#ifndef __STM32KISS_H__
