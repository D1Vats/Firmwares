/*
 * uart.h
 *
 *  Created on: Oct 29, 2024
 *      Author: Dwijendra Vats
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"
#include <stdint.h>

#include "serial.h"

#define RX_BUFFER_SIZE 256


void UART_begin(UART_HandleTypeDef *huart, unsigned long baudRate);
void UART_msp_Init();
void UART_DMA_Init(DMA_HandleTypeDef *hdma_usart_rx);
void UART_end();

uint32_t UART_available();
int UART_peek();

int UART_read();
int UART_readBytes(uint8_t *buffer, uint16_t length);

size_t UART_write(const char *str);

size_t UART_print(const char *str, size_t size);
size_t UART_println(const char *str);
void UART_flush();
void UART_restart();

extern Serial_HandleTypeDef *Serial;



#endif /* UART_H_ */
