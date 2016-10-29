#include "stm32kiss.h"

const KS_PIN * const adc_chanel_pins[18] = {
		PIN_A0, //0
		PIN_A1, //1
		PIN_A2, //2
		PIN_A3, //3
		PIN_A4, //4
		PIN_A5, //5
		PIN_A6, //6
		PIN_A7, //7
		PIN_B0, //8
		PIN_B1, //9
		PIN_C0, //10
		PIN_C1, //11
		PIN_C2, //12
		PIN_C3, //13
		PIN_C4, //14
		PIN_C5, //15
		NULL,
		NULL,
};

KS_ADC_CONTEXT adc_context;

void adc_on(uint8_t adc_chanel, uint32_t ADC_SampleTime)
{
	if (adc_chanel >= LENGTH(adc_chanel_pins)) return;

	RCC_ADCCLKConfig (RCC_PCLK2_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	pin_analog_input(adc_chanel_pins[adc_chanel]);

	ADC_InitTypeDef ADC_InitStructure = {
			.ADC_Mode = ADC_Mode_Independent,
			.ADC_ScanConvMode = DISABLE,
			.ADC_ContinuousConvMode = DISABLE,
			.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
			.ADC_DataAlign = ADC_DataAlign_Left,
			.ADC_NbrOfChannel = 1
	};

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, adc_chanel, 1, ADC_SampleTime);

	ADC_Cmd (ADC1,ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd (ADC1,ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);

	adc_context.SampleTime = ADC_SampleTime;
	adc_context.chanel_num = adc_chanel;
	adc_context.enabled    = true;

	if ((adc_chanel == ADC_Channel_TempSensor) || (adc_chanel == ADC_Channel_Vrefint))// || (adc_chanel == ADC_Channel_Vbat))
		delay_ms(1);
}

void adc_chanel(uint8_t adc_chanel)
{
	ADC_RegularChannelConfig(ADC1, adc_chanel, 1, adc_context.SampleTime);
	adc_context.chanel_num = adc_chanel;
}

void adc_off()
{
	ADC_Cmd(ADC1,DISABLE);
	ADC_DeInit(ADC1);
	RCC_ADCCLKConfig (RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);

	adc_context.chanel_num = 0;
	adc_context.SampleTime = 0;
	adc_context.enabled    = false;
}

uint16_t adc_read()
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	while (ADC_GetSoftwareStartConvStatus(ADC1) == SET);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	uint16_t result = ADC_GetConversionValue(ADC1);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

	return result;
}
