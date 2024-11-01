/*
 * serial.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Dwijendra Vats
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "main.h"

typedef struct
{
	void(*begin)(UART_HandleTypeDef *, unsigned long);
	void(*end)();
	uint32_t(*available)();
	int(*peek)();
	int(*read)();
	int(*readBytes)(uint8_t *, uint16_t);
	size_t(*write)(const char *);
	size_t(*print)(const char *, size_t);
	size_t(*println)(const char *);
	void(*flush)();
	void(*restart)();
}Serial_HandleTypeDef;

#endif /* SERIAL_H_ */
