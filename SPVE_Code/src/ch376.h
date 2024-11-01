#ifndef _CH376_H
#define _CH376_H

#include <Arduino.h>
#include <Stream.h>

#if defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
#include "avr/dtostrf.h"
#endif

#define MAXDIRDEPTH 3
#define ANSWTIMEOUT 1000

const uint32_t CURSORBEGIN = 0x00000000;
const uint32_t CURSOREND = 0xFFFFFFFF;
const uint16_t SECTORSIZE = 512;

// Mapping the Register for CH376
#define GET_IC_VER (0x01)
#define SET_BAUDRATE (0x02)
#define ENTER_SLEEP (0x03)
#define RESET_ALL (0x05)
#define CHECK_EXIST (0x06)
#define SET_SDO_INT (0x0B)
#define GET_FILE_SIZE (0x0C)
#define SET_USB_MODE (0x15)
#define GET_STATUS (0x22)
#define RD_USB_DATA (0x27)
#define WR_USB_DATA (0x2C)
#define WR_REQ_DATA (0x2D)
#define WR_OFS_DATA (0x2E)
#define SET_FILE_NAME (0x2F)
#define DISK_CONNECT (0x30)
#define DISK_MOUNT (0x31)
#define FILE_OPEN (0x32)
#define FILE_ENUM_GO (0x33)
#define FILE_CREATE (0x34)
#define FILE_ERASE (0x35)
#define FILE_CLOSE (0x36)
#define DIR_INFO_READ (0x37)
#define DIR_INFO_SAVE (0x38)
#define BYTE_LOCATE (0x39)
#define BYTE_READ (0x3A)
#define BYTE_RD_GO (0x3B)
#define BYTE_WRITE (0x3C)
#define BYTE_WR_GO (0x3D)
#define DISK_CAPACITY (0x3E)
#define DISK_QUERY (0x3F)
#define DIR_CREATE (0x40)
#define SEG_LOCATE (0x4A)
#define SEC_READ (0x4B)
#define SEC_WRITE (0x4C)
#define DISK_BOC_CMD (0x50)
#define DISK_READ (0x54)
#define DISK_RD_GO (0x55)
#define DISK_WRITE (0x56)
#define DISK_WR_GO (0x57)
#define CMD_RET_SUCCESS (0x51)
#define CMD_RET_ABORT (0x52)

const uint8_t MODE_HOST_INV = 0x04;
const uint8_t MODE_HOST_0 = 0x05;
const uint8_t MODE_HOST_1 = 0x07;
const uint8_t MODE_HOST_2 = 0x06;
const uint8_t MODE_HOST_SD = 0x03;
const uint8_t MODE_DEFAULT = 0x00;

const uint8_t ANSW_RET_SUCCESS = 0x51;
const uint8_t ANSW_USB_INT_SUCCESS = 0x14;
const uint8_t ANSW_USB_INT_CONNECT = 0x15;
const uint8_t ANSW_USB_INT_DISCONNECT = 0x16;

const uint8_t ANSW_USB_INT_USB_READY = 0x18;
const uint8_t ANSW_USB_INT_DISK_READ = 0x1d;
const uint8_t ANSW_USB_INT_DISK_WRITE = 0x1e;

const uint8_t ANSW_RET_ABORT = 0x5F;
const uint8_t ANSW_USB_INT_DISK_ERR = 0x1f;
const uint8_t ANSW_USB_INT_BUF_OVER = 0x17;
const uint8_t ANSW_ERR_OPEN_DIR = 0x41;
const uint8_t ANSW_ERR_MISS_FILE = 0x42;
const uint8_t ANSW_ERR_FOUND_NAME = 0x43;
const uint8_t ANSW_ERR_DISK_DISCON = 0x82;
const uint8_t ANSW_ERR_LARGE_SECTOR = 0x84;
const uint8_t ANSW_ERR_TYPE_ERROR = 0x92;
const uint8_t ANSW_ERR_BPB_ERROR = 0xa1;
const uint8_t ANSW_ERR_DISK_FULL = 0xb1;
const uint8_t ANSW_ERR_FDT_OVER = 0xb2;
const uint8_t ANSW_ERR_FILE_CLOSE = 0xb4;

const uint8_t CH376_ERR_OVERFLOW = 0x03;
const uint8_t CH376_ERR_TIMEOUT = 0x02;
const uint8_t CH376_ERR_NO_RESPONSE = 0x01;
const uint8_t CH376_ERR_LONGFILENAME = 0x04;

// File attributes
const uint8_t CH376_ATTR_READ_ONLY = 0x01;
const uint8_t CH376_ATTR_HIDDEN = 0x02;
const uint8_t CH376_ATTR_SYSTEM = 0x04;
const uint8_t CH376_ATTR_VOLUME_ID = 0x08;
const uint8_t CH376_ATTR_DIRECTORY = 0x10;
const uint8_t CH376_ATTR_ARCHIVE = 0x20;

enum commInterface
{
	UARTT
};

enum fileProcessENUM
{
	REQUEST,
	NEXT,
	READWRITE,
	DONE
};

union fSizeContainer
{
	uint8_t b[4];	// byte
	uint32_t value; // unsigned long
};

struct fatFileInfo
{
	char name[11];	   // 11
	uint8_t fattr;	   // 1
	uint8_t uattr;	   // 1
	uint8_t del;	   // 1
	uint16_t crTime;   // 2
	uint16_t crDate;   // 2
	uint16_t ownId;	   // 2
	uint16_t accRight; // 2
	uint16_t modTime;  // 2
	uint16_t modDate;  // 2
	uint16_t startCl;  // 2
	uint32_t size;	   // 4
};

struct diskInfo
{						  // disk information
	uint32_t totalSector; // the number of total sectors (low byte first)
	uint32_t freeSector;  // the number of free sectors (low byte first)
	uint8_t diskFat;	  // FAT type: 0x01-FAT12, 0x02-FAT16, 0x03-FAT32+
};

class USB
{
private:
	void write(uint8_t data);
	void print(const char str[]);
	void driveAttach();
	void driveDetach();

	uint8_t mount();
	uint8_t getInterrupt();
	uint8_t fileEnumGo();
	uint8_t byteRdGo();
	uint8_t fileCreate();
	uint8_t byteWrGo();
	uint8_t readSerDataUSB();
	uint8_t reqByteRead(uint8_t a);
	uint8_t reqByteWrite(uint8_t a);
	uint8_t writeMachine(uint8_t *buffer, uint8_t b_size);
	uint8_t writeDataFromBuff(uint8_t *buffer);
	uint8_t readDataToBuff(uint8_t *buffer, uint8_t siz);
	uint8_t readMachine(uint8_t *buffer, uint8_t b_size);
	uint8_t dirInfoRead();
	uint8_t setMode(uint8_t mode);
	uint8_t dirCreate();

	void rdFatInfo();
	void setSpeed();
	void setError(uint8_t errCode);
	void clearError();
	void sendCommand(uint8_t b_parancs);
	void sendFilename();
	void writeFatData();
	void constructDate(uint16_t value, uint8_t ymd);
	void constructTime(uint16_t value, uint8_t hms);
	void rdDiskInfo();
	void rstFileContainer();
	void rstDriveContainer();

	bool _deviceAttached = false;
	bool _controllerReady = false;
	bool _hwSerial;
	uint8_t _streamLength = 0;
	uint8_t _fileWrite = 0;
	uint8_t _dirDepth = 0;
	uint8_t _byteCounter = 0;
	uint8_t _answer = 0;
	uint8_t _driveSource = 0;
	uint8_t _spiChipSelect = 0;
	uint8_t _intPin = 0;
	uint8_t _errorCode = 0;
	uint16_t _sectorCounter = 0;
	uint32_t _speed;

	fSizeContainer _cursorPos;
	char _filename[12];
	HardwareSerial *_comPortHW; // Serial interface
	Stream *_comPort;
	commInterface _interface;
	fileProcessENUM fileProcesSTM = REQUEST;

	fatFileInfo _fileData;
	diskInfo _diskData;

public:
	USB(HardwareSerial &usb, uint32_t speed);
	virtual ~USB();
	bool init();
	uint8_t saveFileAttrb();
	uint8_t openFile();
	uint8_t closeFile();
	uint8_t moveCursor(uint32_t position);
	uint8_t deleteFile();
	uint8_t deleteDir();
	uint8_t pingDevice();
	uint8_t listDir(const char *filename = "*");
	uint8_t readFile(char *buffer, uint8_t b_size);
	uint8_t readRaw(uint8_t *buffer, uint8_t b_size);
	int32_t readLong(char trmChar = '\n');
	uint32_t readULong(char trmChar = '\n');
	double readDouble(char trmChar = '\n');
	uint8_t writeChar(char trmChar);
	uint8_t writeFile(char *buffer, uint8_t b_size);
	uint8_t writeRaw(uint8_t *buffer, uint8_t b_size);
	uint8_t writeNum(uint8_t buffer);
	uint8_t writeNum(int8_t buffer);
	uint8_t writeNum(uint16_t buffer);
	uint8_t writeNum(int16_t buffer);
	uint8_t writeNum(uint32_t buffer);
	uint8_t writeNum(int32_t buffer);
	uint8_t writeNum(double buffer);

	uint8_t writeNumln(uint8_t buffer);
	uint8_t writeNumln(int8_t buffer);
	uint8_t writeNumln(uint16_t buffer);
	uint8_t writeNumln(int16_t buffer);
	uint8_t writeNumln(uint32_t buffer);
	uint8_t writeNumln(int32_t buffer);
	uint8_t writeNumln(double buffer);
	uint8_t cd(const char *dirPath, bool mkDir);
	bool readFileUntil(char trmChar, char *buffer, uint8_t b_size);
	bool checkIntMessage();
	bool driveReady();
	bool drivePlugged();
	void resetFileList();

	uint32_t getFreeSectors();
	uint32_t getTotalSectors();
	uint32_t getFileSize();
	uint32_t getCursorPos();
	uint16_t getYear();
	uint16_t getMonth();
	uint16_t getDay();
	uint16_t getHour();
	uint16_t getMinute();
	uint16_t getSecond();
	uint8_t getStreamLen();
	uint8_t getStatus();
	uint8_t getFileSystem();
	uint8_t getFileAttrb();
	uint8_t getSource();
	uint8_t getError();
	uint8_t getChipVer();
	char *getFileName();
	char *getFileSizeStr();
	bool getDeviceStatus();
	bool getCHpresence();
	bool getEOF();

	void setFileName(const char *filename = "");
	void setYear(uint16_t year);
	void setMonth(uint16_t month);
	void setDay(uint16_t day);
	void setHour(uint16_t hour);
	void setMinute(uint16_t minute);
	void setSecond(uint16_t second);
	void setSource(uint8_t inpSource);
};

#endif