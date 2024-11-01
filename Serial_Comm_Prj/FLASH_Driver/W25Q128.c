/*
 * W25Q128.c
 *
 *  Created on: Jan 21, 2024
 *      Author: Dwijendra Vats
 */


#include "main.h"
#include "W25Q128.h"

extern SPI_HandleTypeDef hspi1;


Flash_HandleTyepeDef flash_Drv =
{
		.reset = W25Q_Reset,
		.getID = W25Q_ReadID,
		.read = W25Q_Read,
		.fastRead = W25Q_Fast_Read,
		.sectorErase = W25Q_Sector_Erase,
		.writePage = W25Q_Write_Page
};

Flash_HandleTyepeDef *Flash = &flash_Drv;

#define W25Q_SPI hspi1

#define spi_csLOW() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
#define spi_csHIGH() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

#define numBLOCK 256   // Number of blocks for 128mb flash

void SPI_Write(uint8_t *data, uint8_t len)
{
	HAL_SPI_Transmit(&W25Q_SPI, data, len, HAL_MAX_DELAY);
}

void SPI_Read(uint8_t *data, uint32_t len)
{
	HAL_SPI_Receive(&W25Q_SPI, data, len, HAL_MAX_DELAY);
}

void W25Q_Reset(void)
{
	uint8_t udata[2];
	udata[0] = 0x66; // Enable Reset
	udata[1] = 0x99; // Reset
	spi_csLOW();
	SPI_Write(udata, 2);
	spi_csHIGH();
	HAL_Delay(100);
}

uint32_t W25Q_ReadID(void)
{
	uint8_t tData = 0x9F; // Read JEDEC ID
	uint8_t rData[3];
	spi_csLOW();
	SPI_Write(&tData, 1);
	SPI_Read(rData, 3);
	spi_csHIGH();
	return ((rData[0] << 16)|(rData[1] << 8)|(rData[2]));
}

uint32_t bytestoWrite(uint32_t size, uint16_t offset)
{
	if((size+offset)<256)
		return size;
	else
		return 256-offset;

}

void W25Q_Read(uint32_t startpage, uint8_t offset, uint32_t size, uint8_t *rData)
{
	uint8_t tData[5];
	uint32_t memaddr = (startpage * 256) + offset;

	if (numBLOCK < 1024)  //Size < 512Mb
	{
		tData[0] = 0x03;
		tData[1] = (memaddr >> 16) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 8) & 0xFF;
		tData[3] = (memaddr) & 0xFF; // LSB of the memory Address
	}
	else
	{
		tData[0] = 0x03;
		tData[1] = (memaddr >> 24) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 16) & 0xFF;
		tData[3] = (memaddr >> 8) & 0xFF;
		tData[4] = (memaddr) & 0xFF; // LSB of the memory Address
	}
	spi_csLOW(); // pull the CS LOW

	if(numBLOCK < 1024)
	{
		SPI_Write(tData, 4);
	}
	else
	{
		SPI_Write(tData, 5);
	}
	SPI_Read(rData, size);
	spi_csHIGH(); // Pull the CS HIGH

}

void W25Q_Fast_Read(uint32_t startpage, uint8_t offset, uint32_t size, uint8_t *rData)
{
	uint8_t tData[6];
	uint32_t memaddr = (startpage * 256) + offset;

	if (numBLOCK < 1024)
	{
		tData[0] = 0x0B; // Enable Fast Read
		tData[1] = (memaddr >> 16) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 8) & 0xFF;
		tData[3] = (memaddr) & 0xFF; // LSB of the memory Address
		tData[4] = 0; // Dummy clock
	}
	else
	{
		tData[0] = 0x0B; // Enable Fast Read
		tData[1] = (memaddr >> 24) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 16) & 0xFF;
		tData[3] = (memaddr >> 8) & 0xFF;
		tData[4] = (memaddr) & 0xFF; // LSB of the memory Address
		tData[5] = 0; // Dummy clock
	}
	spi_csLOW(); // pull the CS LOW

	if(numBLOCK < 1024)
	{
		SPI_Write(tData, 4);
	}
	else
	{
		SPI_Write(tData, 5);
	}
	SPI_Read(rData, size);
	spi_csHIGH(); // Pull the CS HIGH

}

void write_enable(void)
{
	uint8_t tData = 0x06; // Enable Write
	spi_csLOW();
	SPI_Write(&tData, 1);
	spi_csHIGH();
	HAL_Delay(5);
}

void write_disable(void)
{
	uint8_t tData = 0x04; // Enable Write
	spi_csLOW();
	SPI_Write(&tData, 1);
	spi_csHIGH();
	HAL_Delay(5);
}

void W25Q_Sector_Erase(uint16_t numsector)
{
	uint8_t tData[6];
	uint32_t memaddr = numsector * 16 * 256;

	write_enable();

	if (numBLOCK < 1024)
	{
		tData[0] = 0x20; // Enable Fast Read
		tData[1] = (memaddr >> 16) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 8) & 0xFF;
		tData[3] = (memaddr) & 0xFF; // LSB of the memory Address

		spi_csLOW();
		SPI_Write(tData, 4);
		spi_csHIGH();
	}
	else
	{
		tData[0] = 0x20; // Enable Fast Read
		tData[1] = (memaddr >> 24) & 0xFF; // MSB of the memory Address
		tData[2] = (memaddr >> 16) & 0xFF;
		tData[3] = (memaddr >> 8) & 0xFF;
		tData[4] = (memaddr) & 0xFF; // LSB of the memory Address

		spi_csLOW();
		SPI_Write(tData, 5);
		spi_csHIGH();
	}

	HAL_Delay(450);

	write_disable();
}

void W25Q_Write_Page(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
{
	uint8_t tData[266];
	uint32_t startPage = page;
	uint32_t endPage = startPage + ((size + offset -1)/256);
	uint32_t numPages = endPage - startPage + 1;

	uint16_t startSector = startPage/16;
	uint16_t endSector = endPage / 16;
	uint16_t numSectors = endSector - startSector + 1;
	for(uint16_t i = 0; i < numSectors; i++)
	{
		W25Q_Sector_Erase(startSector++);
	}

	uint32_t dataPosition = 0;

	//write the data
	for(uint32_t i = 0; i < numPages; i++)
	{
		uint32_t memaddr = (startPage*256) + offset;
		uint16_t bytesremaining = bytestoWrite(size, offset);
		uint32_t index = 0;

		write_enable();
		if (numBLOCK < 1024)
		{
			tData[0] = 0x02; // Page Program
			tData[1] = (memaddr >> 16) & 0xFF; // MSB of the memory Address
			tData[2] = (memaddr >> 8) & 0xFF;
			tData[3] = (memaddr) & 0xFF; // LSB of the memory Address

			index = 4;

		}
		else
		{
			tData[0] = 0x02; // Enable Fast Read
			tData[1] = (memaddr >> 24) & 0xFF; // MSB of the memory Address
			tData[2] = (memaddr >> 16) & 0xFF;
			tData[3] = (memaddr >> 8) & 0xFF;
			tData[4] = (memaddr) & 0xFF; // LSB of the memory Address

			index =5;
		}

		uint16_t bytestosend = bytesremaining + index;

		for(uint16_t i = 0; i < bytesremaining; i++)
		{
			tData[index++] = data[i+dataPosition];
		}

		if(bytestosend > 250)
		{
			spi_csLOW();
			SPI_Write(tData, 100);
			SPI_Write(tData + 100, bytestosend - 100);
			spi_csHIGH();
		}
		else
		{
			spi_csLOW();
			SPI_Write(tData, bytestosend);
			spi_csHIGH();
		}

		startPage++;
		offset = 0;
		size = size - bytesremaining;
		dataPosition = dataPosition + bytesremaining;

		HAL_Delay(5);
		write_disable();

	}
}





