#include "stm32kiss.h"

#pragma GCC diagnostic ignored "-Wpointer-arith"

void usart_a_on(uint32_t speed);
void usart_a_off();
void usart_init_clock(void (*RCC_PeriphClockCmd)(uint32_t RCC_APBPeriph, FunctionalState NewState), uint32_t usart_clock_const);
void usart_init_gpio(const KS_PIN *pin_tx, const KS_PIN *pin_rx);
void usart_init_speed(USART_TypeDef *usart, uint32_t speed);
void usart_init_interrupt(USART_TypeDef *usart, uint32_t irq_number);
void usart_init_go(USART_TypeDef *usart);
void usart_dma_init_recive(DMA_Channel_TypeDef *dma_chanel, USART_TypeDef *usart, void *data, uint32_t size);
void usart_dma_init_trnasmit(DMA_Channel_TypeDef *dma_chanel, uint32_t irq_number);
void usart_a_transmit(void *data, uint32_t size);

void __usart_a_tx_check();
void __usart_a_rx_check();

void __usart_b_tx_check();
void __usart_b_rx_check();

void usart_a_transmit(void *data, uint32_t size);
void usart_b_transmit(void *data, uint32_t size);

/*tFIFO *usart_a_rx_fifo;
tFIFO *usart_a_tx_fifo;

tFIFO *usart_b_rx_fifo;
tFIFO *usart_b_tx_fifo;*/

fifo_declare(usart_a_rx_fifo, 64)
fifo_declare(usart_a_tx_fifo, 64)
fifo_declare(usart_b_rx_fifo, 64)
fifo_declare(usart_b_tx_fifo, 64)

volatile bool usart_a_transmit_complite=false;
volatile bool usart_b_transmit_complite=false;

static uint32_t usart_a_rx_old_pos;
static uint32_t usart_b_rx_old_pos;

void (* volatile usart_a_irq_func)() = NULL;
void (* volatile usart_b_irq_func)() = NULL;
void (* volatile usart_a_dma_tx_func)() = NULL;
void (* volatile usart_b_dma_tx_func)() = NULL;

void DMA1_Channel4_IRQHandler(void) //USART1 TX DMA
{
	if (usart_a_dma_tx_func != NULL)
		(*usart_a_dma_tx_func)();
}

void DMA1_Channel7_IRQHandler(void) //USART2 TX DMA
{
	if (usart_b_dma_tx_func != NULL)
		(*usart_b_dma_tx_func)();
}

void USART1_IRQHandler(void) //ERRORS
{
	if (usart_a_irq_func != NULL)
		(*usart_a_irq_func)();
}

void USART2_IRQHandler(void) //ERRORS
{
	if (usart_b_irq_func != NULL)
		(*usart_b_irq_func)();
}

////////////////////////////////////////////////////////////////////////

void __usart_a_dma_tx_handler()
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TE4))
	{
		DMA_ClearFlag(DMA1_FLAG_TE4);
		//error_add(ERROR_UART_TX_DMA);
	};

	if (DMA_GetFlagStatus(DMA1_FLAG_TC4))
	{
		DMA_ClearFlag(DMA1_FLAG_TC4);
		usart_a_transmit_complite = true;
		__usart_a_tx_check();
	};
}

void __usart_b_dma_tx_handler()
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TE7))
	{
		DMA_ClearFlag(DMA1_FLAG_TE7);
		//error_add(ERROR_UART_TX_DMA);
	};

	if (DMA_GetFlagStatus(DMA1_FLAG_TC7))
	{
		DMA_ClearFlag(DMA1_FLAG_TC7);
		usart_b_transmit_complite = true;
		__usart_b_tx_check();
	};
}

void __usart_a_irq_handler()
{
	if (USART_GetITStatus(USART1, USART_IT_FE)!=0) //Framing Error interrupt
		USART_ReceiveData(USART1);

	if (USART_GetITStatus(USART1, USART_IT_NE)!=0) //Noise Error interrupt
		USART_ReceiveData(USART1);

	if (USART_GetITStatus(USART1, USART_IT_ORE)!=0) //OverRun Error interrupt
		USART_ReceiveData(USART1);
}

void __usart_b_irq_handler()
{
	if (USART_GetITStatus(USART2, USART_IT_FE)!=0) //Framing Error interrupt
		USART_ReceiveData(USART2);

	if (USART_GetITStatus(USART2, USART_IT_NE)!=0) //Noise Error interrupt
		USART_ReceiveData(USART2);

	if (USART_GetITStatus(USART2, USART_IT_ORE)!=0) //OverRun Error interrupt
		USART_ReceiveData(USART2);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void __usart_a_rx_check()
{
    uint32_t usart_rx_new_pos = usart_a_rx_fifo->size - DMA_GetCurrDataCounter(DMA1_Channel5);
    int32_t  count = 0;

    count = usart_rx_new_pos - usart_a_rx_old_pos;
    usart_a_rx_old_pos = usart_rx_new_pos;

    if (count < 0)
    	count = usart_a_rx_fifo->size + count;

    usart_a_rx_fifo->wr += count;
}


void __usart_b_rx_check()
{
    uint32_t usart_rx_new_pos = usart_b_rx_fifo->size - DMA_GetCurrDataCounter(DMA1_Channel6);
    int32_t  count = 0;

    count = usart_rx_new_pos - usart_b_rx_old_pos;
    usart_b_rx_old_pos = usart_rx_new_pos;

    if (count < 0)
    	count = usart_b_rx_fifo->size + count;

    usart_b_rx_fifo->wr += count;
}

void __usart_a_tx_check()
{
	static uint8_t tx_buf[32];

	if (usart_a_transmit_complite == false) return;

	usart_a_transmit(tx_buf, fifo_read(usart_a_tx_fifo, tx_buf, sizeof(tx_buf)));
}

void __usart_b_tx_check()
{
	static uint8_t tx_buf[32];

	if (usart_b_transmit_complite == false) return;

	usart_b_transmit(tx_buf, fifo_read(usart_b_tx_fifo, tx_buf, sizeof(tx_buf)));
}

void usart_a_check()
{
	NVIC_DisableIRQ(DMA1_Channel4_IRQn);
	__usart_a_rx_check();
	__usart_a_tx_check();
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void usart_b_check()
{
	NVIC_DisableIRQ(DMA1_Channel7_IRQn);
	__usart_b_rx_check();
	__usart_b_tx_check();
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void usart_a_on(uint32_t speed)
{
	static bool inited = false;
	if (inited)	usart_a_off();

//    fifo_init(usart_a_tx_fifo, 256);
//    fifo_init(usart_a_rx_fifo, 256);
	fifo_init(usart_a_rx_fifo);
	fifo_init(usart_a_tx_fifo);
    usart_a_rx_old_pos = 0;
    usart_a_transmit_complite=true;

    usart_a_dma_tx_func = __usart_a_dma_tx_handler;
	usart_a_irq_func    = __usart_a_irq_handler;

	usart_init_clock(RCC_APB2PeriphClockCmd, RCC_APB2Periph_USART1);
	usart_init_gpio(PIN_A9, PIN_A10);
	usart_init_speed(USART1, speed);
	usart_init_interrupt(USART1, USART1_IRQn);
	usart_dma_init_trnasmit(DMA1_Channel4, DMA1_Channel4_IRQn);
	usart_dma_init_recive(DMA1_Channel5, USART1, usart_a_rx_fifo->data, usart_a_rx_fifo->size);
	usart_init_go(USART1);
	inited = true;
}

void usart_b_on(uint32_t speed)
{
	static bool inited = false;
	if (inited)	usart_a_off();

//    fifo_init(usart_b_tx_fifo, 256);
//    fifo_init(usart_b_rx_fifo, 256);
	fifo_init(usart_b_rx_fifo);
	fifo_init(usart_b_tx_fifo);
    usart_b_rx_old_pos = 0;
    usart_b_transmit_complite=true;

    usart_b_dma_tx_func = __usart_b_dma_tx_handler;
    usart_b_irq_func    = __usart_b_irq_handler;

	usart_init_clock(RCC_APB1PeriphClockCmd, RCC_APB1Periph_USART2);
	usart_init_gpio(PIN_A2, PIN_A3);
	usart_init_speed(USART2, speed);
	usart_init_interrupt(USART2, USART2_IRQn);
	usart_dma_init_trnasmit(DMA1_Channel7, DMA1_Channel7_IRQn);
	usart_dma_init_recive(DMA1_Channel6, USART2, usart_b_rx_fifo->data, usart_b_rx_fifo->size);
	usart_init_go(USART2);
	inited = true;
}

void usart_a_off()
{
	NVIC_DisableIRQ(USART1_IRQn);
	NVIC_DisableIRQ(DMA1_Channel4_IRQn);
	NVIC_DisableIRQ(DMA1_Channel5_IRQn);

    usart_a_transmit_complite=false;

	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);
	USART_Cmd(USART1, DISABLE);

    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_DeInit(DMA1_Channel4);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TE | DMA_IT_HT | DMA_IT_TC, DISABLE);

    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA_DeInit(DMA1_Channel5);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TE | DMA_IT_HT | DMA_IT_TC, DISABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
}

void usart_b_off()
{
	NVIC_DisableIRQ(USART2_IRQn);
	NVIC_DisableIRQ(DMA1_Channel6_IRQn);
	NVIC_DisableIRQ(DMA1_Channel7_IRQn);

    usart_b_transmit_complite=false;

	USART_DMACmd(USART2, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);
	USART_Cmd(USART2, DISABLE);

    DMA_Cmd(DMA1_Channel6, DISABLE);
    DMA_DeInit(DMA1_Channel6);
    DMA_ITConfig(DMA1_Channel6, DMA_IT_TE | DMA_IT_HT | DMA_IT_TC, DISABLE);

    DMA_Cmd(DMA1_Channel7, DISABLE);
    DMA_DeInit(DMA1_Channel7);
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TE | DMA_IT_HT | DMA_IT_TC, DISABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
}

void usart_init_clock(void (*RCC_PeriphClockCmd)(uint32_t RCC_APBPeriph, FunctionalState NewState), uint32_t usart_clock_const)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    (*RCC_PeriphClockCmd)(usart_clock_const, ENABLE);
}

void usart_init_gpio(const KS_PIN *pin_tx, const KS_PIN *pin_rx)
{
	pin_analog_output(pin_tx);
	pin_input(pin_rx);
}

void usart_init_speed(USART_TypeDef *usart, uint32_t speed)
{
	USART_InitTypeDef USART_InitStructure;

	STRUCT_CLEAR(USART_InitStructure);

	USART_InitStructure.USART_BaudRate    = speed;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits    = USART_StopBits_1;
	USART_InitStructure.USART_Parity      = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode        = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(usart, &USART_InitStructure);
}

void usart_init_interrupt(USART_TypeDef *usart, uint32_t irq_number)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	STRUCT_CLEAR(NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel                   = irq_number;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(usart, USART_IT_CTS,  DISABLE);  //CTS change interrupt (not available for UART4 and UART5)
	USART_ITConfig(usart, USART_IT_LBD,  DISABLE);  //LIN Break detection interrupt
	USART_ITConfig(usart, USART_IT_TC,   DISABLE);  //Transmission complete interrupt
	USART_ITConfig(usart, USART_IT_IDLE, DISABLE);  //Idle line detection interrupt
	USART_ITConfig(usart, USART_IT_PE,   DISABLE);  //Parity Error interrupt

	USART_ITConfig(usart, USART_IT_TXE,  DISABLE);   //Transmit Data Register empty interrupt
	USART_ITConfig(usart, USART_IT_ERR,  ENABLE);    //Error interrupt(Frame error, noise error, overrun error)
	USART_ITConfig(usart, USART_IT_RXNE, DISABLE);   //Receive Data register not empty interrupt
}

void usart_dma_init_trnasmit(DMA_Channel_TypeDef *dma_chanel, uint32_t irq_number)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    STRUCT_CLEAR(NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel                   = irq_number;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(dma_chanel, DISABLE);
    DMA_DeInit(dma_chanel);
    DMA_ITConfig(dma_chanel, DMA_IT_TE | DMA_IT_HT | DMA_IT_TC, ENABLE);
}

void usart_dma_init_recive(DMA_Channel_TypeDef *dma_chanel, USART_TypeDef *usart, void *data, uint32_t size)
{
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(dma_chanel);

    STRUCT_CLEAR(DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(usart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) data;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;

    DMA_InitStructure.DMA_BufferSize         = size;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

    DMA_Init(dma_chanel, &DMA_InitStructure);
    usart->SR=0;
    DMA_Cmd(dma_chanel, ENABLE);
}

void usart_init_go(USART_TypeDef *usart)
{
	USART_DMACmd(usart, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
	USART_Cmd(usart, ENABLE);
}

void usart_a_write(void *data, uint32_t size)
{
	if (size == 0) return;
	if (size > usart_a_tx_fifo->size) return;

	usart_a_check();
	uint32_t free = 0;
	while (free < size)
	{
		free = fifo_free(usart_a_tx_fifo);
		usart_a_check();
	}

	fifo_write(usart_a_tx_fifo, data, size);
	usart_a_check();
}

void usart_a_read(void *data, uint32_t size)
{
	while (size > 0)
	{
		usart_a_check();
		while (fifo_count(usart_a_rx_fifo) == 0)
			usart_a_check();

		uint32_t readed = fifo_read(usart_a_rx_fifo, data, size);
		size -= readed;
		data += readed;
	}
}

void usart_b_write(void *data, uint32_t size)
{
	if (size == 0) return;
	if (size > usart_b_tx_fifo->size) return;

	usart_b_check();
	uint32_t free = 0;
	while (free < size)
	{
		free = fifo_free(usart_b_tx_fifo);
		usart_b_check();
	}

	fifo_write(usart_b_tx_fifo, data, size);
	usart_b_check();
}

void usart_b_read(void *data, uint32_t size)
{
	while (size > 0)
	{
		usart_b_check();
		while (fifo_count(usart_b_rx_fifo) == 0)
			usart_b_check();

		uint32_t readed = fifo_read(usart_b_rx_fifo, data, size);
		size -= readed;
		data += readed;
	}
}

inline void usart_transmit(DMA_Channel_TypeDef *dma_chanel, USART_TypeDef *usart, void *data, uint32_t size)
{
	if (size == 0) return;

    DMA_DeInit(dma_chanel);

    DMA_InitTypeDef DMA_InitStructure;
    STRUCT_CLEAR(DMA_InitStructure);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(usart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) data;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;

    DMA_InitStructure.DMA_BufferSize         = size;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

    DMA_Init(dma_chanel, &DMA_InitStructure);
    DMA_ITConfig(dma_chanel, DMA_IT_TE | DMA_IT_TC, ENABLE);
    usart->SR=0;
    DMA_Cmd(dma_chanel, ENABLE);
}

void usart_a_transmit(void *data, uint32_t size)
{
	if (size == 0) return;
	usart_transmit(DMA1_Channel4, USART1, data, size);
    usart_a_transmit_complite=false;
}

void usart_b_transmit(void *data, uint32_t size)
{
	if (size == 0) return;
	usart_transmit(DMA1_Channel7, USART2, data, size);
	usart_b_transmit_complite=false;
}
