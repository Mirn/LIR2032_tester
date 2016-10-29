#ifndef __STM32KISS_DMA_USARTS_H__
#define __STM32KISS_DMA_USARTS_H__

void usart_a_on(uint32_t speed);
void usart_b_on(uint32_t speed);

void usart_a_off();
void usart_b_off();

void usart_a_check();
void usart_b_check();

extern tFIFO *usart_a_rx_fifo;
extern tFIFO *usart_a_tx_fifo;

extern tFIFO *usart_b_rx_fifo;
extern tFIFO *usart_b_tx_fifo;

void usart_a_write(void *data, uint32_t size);
void usart_a_read(void *data, uint32_t size);

void usart_b_write(void *data, uint32_t size);
void usart_b_read(void *data, uint32_t size);

#endif //#ifndef __STM32KISS_DMA_USARTS_H__
