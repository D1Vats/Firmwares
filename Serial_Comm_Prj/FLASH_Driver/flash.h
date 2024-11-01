/*
 * flash.h
 *
 *  Created on: Oct 30, 2024
 *      Author: dwije
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "main.h"

typedef struct
{
	void(*reset)(void);
	uint32_t(*getID)(void);
	void(*read)(uint32_t, uint8_t, uint32_t, uint8_t *);
	void(*fastRead)(uint32_t, uint8_t, uint32_t, uint8_t *);
	void(*sectorErase)(uint16_t);
	void(*writePage)(uint32_t, uint16_t , uint32_t , uint8_t *);
}Flash_HandleTyepeDef;



#endif /* FLASH_H_ */
