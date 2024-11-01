/*
 * uart.c
 *
 *  Created on: Oct 29, 2024
 *      Author: Dwijendra Vats
 */

#include "uart.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

Serial_HandleTypeDef UART_Drv =
{
		.begin = UART_begin,
		.end = UART_end,
		.available = UART_available,
		.peek = UART_peek,
		.read = UART_read,
		.readBytes = UART_readBytes,
		.write = UART_write,
		.print = UART_print,
		.println = UART_println,
		.flush = UART_flush,
		.restart = UART_restart
};

Serial_HandleTypeDef *Serial = &UART_Drv;

uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t read_indx = 0;
bool isWritten = 1;
//uint8_t finalBuf[1024];
//uint32_t receive_count = 0;
//volatile uint16_t finalBuf_Indx = 0;
//volatile uint16_t rx_Indx = 0;

static UART_HandleTypeDef *uart_typeDef;

void UART_begin(UART_HandleTypeDef *huart, unsigned long baudRate)
{
	uart_typeDef = huart;

	huart->Instance = USART3;
	huart->Init.BaudRate = baudRate;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart->Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(huart) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(huart) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buffer, RX_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}

}

void UART_end()
{
	if(HAL_UART_DeInit(uart_typeDef) != HAL_OK)
	{
		Error_Handler();
	}
}

uint32_t UART_available()
{
	uint32_t dma_write_index = (RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart_typeDef->hdmarx)) % RX_BUFFER_SIZE;


	if (dma_write_index >= read_indx) {
		return dma_write_index - read_indx;
	} else {
		return (RX_BUFFER_SIZE - read_indx) + dma_write_index;
	}
}

int UART_peek()
{
	if(UART_available() == 0)
		return -1;

	return rx_buffer[read_indx];
}

int UART_read()
{
	if(UART_available() == 0)
		return -1;

	uint8_t byte = rx_buffer[read_indx];
	read_indx = (read_indx + 1) % RX_BUFFER_SIZE;

	return byte;
}

int UART_readBytes(uint8_t *buffer, uint16_t length)
{
	int bytes_read = 0;

	while(bytes_read < length)
	{
		buffer[bytes_read] = rx_buffer[read_indx];

		read_indx = (read_indx + 1) % RX_BUFFER_SIZE;

		bytes_read++;
	}

	return bytes_read;
}

size_t UART_write(const char *str)
{
	size_t size = strlen(str);
	if (HAL_UART_Transmit(uart_typeDef, (uint8_t *)str, size, HAL_MAX_DELAY) == HAL_OK) {
		return size;
	} else {
		return 0;
	}
}

size_t UART_print(const char *str, size_t size)
{
	char temp[size];
	for(int i = 0; i < size; i++)
		temp[i] = str[i];

	size_t len = size + 3;
	char buffer[len];
	snprintf(buffer, sizeof(buffer), "%s\r\n", temp);
	if(isWritten == 1)
	{
		HAL_UART_Transmit_DMA(uart_typeDef, (uint8_t*)buffer, len);
		isWritten = 0;
	}
	return len;
}
size_t UART_println(const char *str)
{
	size_t len = strlen(str);
	char buffer[len + 3];

	snprintf(buffer, sizeof(buffer), "%s\r\n", str);

	size_t written = UART_write(buffer);
	return written;
}
void UART_flush()
{
	uint32_t timeout = HAL_GetTick() + 1000;

	while (__HAL_UART_GET_FLAG(uart_typeDef, UART_FLAG_TC) == RESET) {
		if (HAL_GetTick() > timeout) {
			return;
		}
	}
}

void UART_restart()
{
	read_indx = 0;
	HAL_UART_DMAStop(uart_typeDef);
	HAL_UARTEx_ReceiveToIdle_DMA(uart_typeDef, rx_buffer, RX_BUFFER_SIZE);
}

// STM32 specific Callback functions

//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if(Size == RX_BUFFER_SIZE)
//	{
//		UART_readBytes(finalBuf + finalBuf_Indx, Size);
//		receive_count++;
//		finalBuf_Indx  = RX_BUFFER_SIZE * receive_count;
//	}
//	else if(Size == RX_BUFFER_SIZE / 2)
//	{
//		return;
//	}
//	else
//	{
//		UART_readBytes(finalBuf + finalBuf_Indx, Size);
//		finalBuf_Indx += Size;
//	}
//}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	isWritten = 1;
}


