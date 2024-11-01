/*
 * W25Q128.h
 *
 *  Created on: Jan 21, 2024
 *      Author: Dwijendra Vats
 */

#ifndef INC_W25Q128_H_
#define INC_W25Q128_H_

#include "flash.h"

void SPI_Read(uint8_t *data, uint32_t len);
void SPI_Write(uint8_t *data, uint8_t len);
void W25Q_Reset(void);
uint32_t W25Q_ReadID(void);
void W25Q_Read(uint32_t startpage, uint8_t offset, uint32_t size, uint8_t *rData);
void W25Q_Fast_Read(uint32_t startpage, uint8_t offset, uint32_t size, uint8_t *rData);
void write_enable(void);
void write_disable(void);
void W25Q_Sector_Erase(uint16_t numsector);
uint32_t bytestoWrite(uint32_t size, uint16_t offset);
void W25Q_Write_Page(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);

extern Flash_HandleTyepeDef *Flash;

#endif /* INC_W25Q128_H_ */
