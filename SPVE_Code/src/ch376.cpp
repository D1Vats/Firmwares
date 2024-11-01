#include "ch376.h"

//with HW serial
USB ::USB(HardwareSerial &usb, uint32_t speed)
{ // @suppress("Class members should be properly initialized")
    _interface = UARTT;
    _comPortHW = &usb;
    _comPort = &usb;
    _speed = speed;
    _hwSerial = true;
}

USB ::~USB()
{
}

bool USB ::init()
{
    delay(60);
    if (_hwSerial)
    {
        _comPortHW->begin(9600);
    }
    sendCommand(RESET_ALL);
    delay(100);
    if (_hwSerial)
    {
        setSpeed();
    }
    _controllerReady = pingDevice(); // check the communication
    if (_controllerReady)
        clearError(); // reinit clear last error code
    setMode(MODE_HOST_0);
    checkIntMessage();
}

uint8_t USB ::pingDevice()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(CHECK_EXIST);
        write(0x01);
        if (readSerDataUSB() == 0xFE)
        {
            tmpReturn = 1; //true
        }
    }
    if (!tmpReturn)
    {
        setError(CH376_ERR_NO_RESPONSE);
    }
    return tmpReturn;
}

bool USB ::driveReady()
{
    uint8_t tmpReturn;
    tmpReturn = mount();
    if (tmpReturn == ANSW_USB_INT_SUCCESS)
    {
        rdDiskInfo();
    } //end if not INT SUCCESS
    return _deviceAttached;
}

bool USB ::drivePlugged()
{
    uint8_t tmpReturn = 0;
    
    if (_interface == UARTT)
    {
        while (_comPort->available())
        {
            tmpReturn = readSerDataUSB();
        } //end while
    }
    return tmpReturn;
}

bool USB ::checkIntMessage()
{
    uint8_t tmpReturn = 0;
    bool intRequest = false;
    if (_interface == UARTT)
    {
        while (_comPort->available())
        {
            tmpReturn = readSerDataUSB();
        } //end while
    }
    switch (tmpReturn)
    { // 0x15 device attached, 0x16 device disconnect
    case ANSW_USB_INT_CONNECT:
        intRequest = true;
        driveAttach(); //device attached
        break;
    case ANSW_USB_INT_DISCONNECT:
        intRequest = true;
        driveDetach(); //device detached
        break;
    } //end switch
    return intRequest;
}

uint8_t USB ::openFile()
{
    if (!_deviceAttached)
        return 0x00;
    if (_interface == UARTT)
    {
        sendCommand(FILE_OPEN);
        _answer = readSerDataUSB();
    }
    if (_answer == ANSW_USB_INT_SUCCESS)
    { // get the file size
        dirInfoRead();
    }
    return _answer;
}

uint8_t USB ::saveFileAttrb()
{
    uint8_t tmpReturn = 0;
    if (!_deviceAttached)
        return 0x00;
    _fileWrite = 1;
    if (_interface == UARTT)
    {
        sendCommand(DIR_INFO_READ);
        write(0xff); // current file is 0xff
        readSerDataUSB();
        writeFatData(); //send fat data
        sendCommand(DIR_INFO_SAVE);
        tmpReturn = readSerDataUSB();
    }
    return tmpReturn;
}

uint8_t USB ::closeFile()
{
    uint8_t tmpReturn = 0;
    uint8_t d = 0x00;
    if (!_deviceAttached)
        return 0x00;

    if (_fileWrite == 1)
    {             // if closing file after write procedure
        d = 0x01; // close with 0x01 (to update file length)
    }
    if (_interface == UARTT)
    {
        sendCommand(FILE_CLOSE);
        write(d);
        tmpReturn = readSerDataUSB();
    }
    cd("/", 0);
    rstFileContainer();
    return tmpReturn;
}

uint8_t USB ::deleteFile()
{
    if (!_deviceAttached)
        return 0x00;
    openFile();
    if (_interface == UARTT)
    {
        sendCommand(FILE_ERASE);
        _answer = readSerDataUSB();
    }
    cd("/", 0);
    return _answer;
}

uint8_t USB ::listDir(const char *filename)
{
    bool moreFiles = true;  // more files waiting for read out
    bool doneFiles = false; // done with reading a file
    uint32_t tmOutCnt = millis();

    while (!doneFiles)
    {
        if (millis() - tmOutCnt >= ANSWTIMEOUT)
            setError(CH376_ERR_TIMEOUT);
        if (!_deviceAttached)
        {
            moreFiles = false;
            break;
        }
        switch (fileProcesSTM)
        {
        case REQUEST:
            setFileName(filename);
            _answer = openFile();
            //_fileWrite = 2; // if in subdir
            fileProcesSTM = READWRITE;
            break;
        case READWRITE:
            if (_answer == ANSW_ERR_MISS_FILE)
            {
                fileProcesSTM = DONE;
                moreFiles = false; // no more files in the directory
            }                      //end if
            if (_answer == ANSW_USB_INT_DISK_READ)
            {
                rdFatInfo(); // read data to fatInfo buffer
                fileProcesSTM = NEXT;
            }
            break;
        case NEXT:
            _answer = fileEnumGo(); // go for the next filename
            fileProcesSTM = DONE;
            break;
        case DONE:
            if (!moreFiles)
            {
                fileProcesSTM = REQUEST;
            }
            else
            {
                fileProcesSTM = READWRITE;
            }
            doneFiles = true;
            break;
        } // end switch
    }     //end while
    return moreFiles;
}

uint8_t USB ::moveCursor(uint32_t position)
{
    uint8_t tmpReturn = 0;
    if (!_deviceAttached)
        return 0x00;

    if (position > _fileData.size)
    {
        _sectorCounter = _fileData.size % SECTORSIZE;
    }
    else
    {
        _sectorCounter = position % SECTORSIZE;
    }
    _cursorPos.value = position;
    sendCommand(BYTE_LOCATE);
    write(_cursorPos.b[0]);
    write(_cursorPos.b[1]);
    write(_cursorPos.b[2]);
    write(_cursorPos.b[3]);
    if (_interface == UARTT)
    {
        tmpReturn = readSerDataUSB();
    }
    if (_cursorPos.value > _fileData.size)
    {
        _cursorPos.value = _fileData.size; //set the valid position
    }
    return tmpReturn;
}

uint8_t USB ::cd(const char *dirPath, bool mkDir)
{
    uint8_t tmpReturn = 0;
    uint8_t pathLen = strlen(dirPath);
    if (!_deviceAttached)
        return 0x00;

    _dirDepth = 0;
    if (pathLen < ((MAXDIRDEPTH * 8) + (MAXDIRDEPTH + 1)))
    { //depth*(8char filename)+(directory separators)
        char input[pathLen + 1];
        strcpy(input, dirPath);
        setFileName("/");
        tmpReturn = openFile();
        char *command = strtok(input, "/"); //split path into tokens
        while (command != NULL && !_errorCode)
        {
            if (strlen(command) > 8)
            { //if a dir name is longer than 8 char
                tmpReturn = CH376_ERR_LONGFILENAME;
                break;
            }
            setFileName(command);
            tmpReturn = openFile();
            if (tmpReturn == ANSW_USB_INT_SUCCESS)
            { //if file already exist with this name
                tmpReturn = ANSW_ERR_FOUND_NAME;
                closeFile();
                break;
            }
            else if (mkDir && (tmpReturn == ANSW_ERR_MISS_FILE))
            {
                tmpReturn = dirCreate();
                if (tmpReturn != ANSW_USB_INT_SUCCESS)
                    break;
            } //end if file exist
            _dirDepth++;
            command = strtok(NULL, "/");
        }
    }
    else
    {
        tmpReturn = CH376_ERR_LONGFILENAME;
    } //end if path is to long
    return tmpReturn;
}

uint8_t USB ::deleteDir()
{
    if (!_deviceAttached)
        return 0x00;
    if (_interface == UARTT)
    {
        sendCommand(FILE_ERASE);
        _answer = readSerDataUSB();
    }
    cd("/", 0);
    return _answer;
}

uint8_t USB ::dirInfoRead()
{
    uint8_t tmpReturn;
    if (_interface == UARTT)
    {
        sendCommand(DIR_INFO_READ); // max 16 files 0x00 - 0x0f
        write(0xff);                // current file is 0xff
        tmpReturn = readSerDataUSB();
    }
    rdFatInfo();
    return tmpReturn;
}

void USB ::writeFatData()
{
    uint8_t fatInfBuffer[32];                                //temporary buffer for raw file FAT info
    memcpy(&fatInfBuffer, &_fileData, sizeof(fatInfBuffer)); //copy raw data to temporary buffer
    sendCommand(WR_OFS_DATA);
    write((uint8_t)0x00);
    write(32);
    for (uint8_t d = 0; d < 32; d++)
    {
        write(fatInfBuffer[d]);
    }
}

uint8_t USB ::mount()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(DISK_MOUNT);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && tmpReturn != ANSW_USB_INT_SUCCESS)
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

uint8_t USB ::fileEnumGo()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(FILE_ENUM_GO);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && (tmpReturn != ANSW_USB_INT_DISK_READ) && (tmpReturn != ANSW_ERR_MISS_FILE))
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

uint8_t USB ::byteRdGo()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(BYTE_RD_GO);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && (tmpReturn != ANSW_USB_INT_DISK_READ) && (tmpReturn != ANSW_USB_INT_SUCCESS))
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

uint8_t USB ::byteWrGo()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(BYTE_WR_GO);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && (tmpReturn != ANSW_USB_INT_DISK_WRITE) && (tmpReturn != ANSW_USB_INT_SUCCESS))
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

uint8_t USB ::fileCreate()
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(FILE_CREATE);
        tmpReturn = readSerDataUSB();
    }
    return tmpReturn;
}

void USB ::rdFatInfo()
{
    uint8_t fatInfBuffer[32]; //temporary buffer for raw file FAT info
    uint8_t dataLength;
    bool owrflow = false;
    if (_interface == UARTT)
    {
        sendCommand(RD_USB_DATA);
        dataLength = readSerDataUSB();
        if (dataLength > sizeof(fatInfBuffer))
        {
            owrflow = true;
            dataLength = sizeof(fatInfBuffer);
        }
        for (uint8_t s = 0; s < dataLength; s++)
        {
            fatInfBuffer[s] = readSerDataUSB(); // fill up temporary buffer
        }                                       //end for
    }
    if (owrflow)
    {
        setError(CH376_ERR_OVERFLOW);
    }
    else
    {
        memcpy(&_fileData, &fatInfBuffer, sizeof(fatInfBuffer)); //copy raw data to structured variable
    }
}

uint8_t USB ::reqByteRead(uint8_t a)
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(BYTE_READ);
        write(a); // request data stream length for reading, 00 - FF
        write((uint8_t)0x00);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && (tmpReturn != ANSW_USB_INT_SUCCESS) && (tmpReturn != ANSW_USB_INT_DISK_READ))
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

uint8_t USB ::reqByteWrite(uint8_t a)
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(BYTE_WRITE);
        write(a); // request data stream length for writing, 00 - FF
        write((uint8_t)0x00);
        tmpReturn = readSerDataUSB();
    }
    if (!_errorCode && (tmpReturn != ANSW_USB_INT_SUCCESS) && (tmpReturn != ANSW_USB_INT_DISK_WRITE))
    {
        setError(tmpReturn);
    }
    return tmpReturn;
}

void USB ::sendFilename()
{
    sendCommand(SET_FILE_NAME);
    print(_filename);
    write((uint8_t)0x00);
}

void USB ::rdDiskInfo()
{
    uint8_t dataLength;
    uint8_t tmpReturn;
    uint8_t tmpdata[9];
    if (_interface == UARTT)
    {
        sendCommand(DISK_QUERY);
        tmpReturn = readSerDataUSB();
        if (tmpReturn == ANSW_USB_INT_SUCCESS)
        {
            sendCommand(RD_USB_DATA);
            dataLength = readSerDataUSB();
            for (uint8_t s = 0; s < dataLength; s++)
            {
                tmpdata[s] = readSerDataUSB(); // fill up temporary buffer
            }                                  //end for
        }                                      //end if success
    }
    if (tmpReturn != ANSW_USB_INT_SUCCESS)
    { // unknown partition issue #22
        if (!_errorCode)
        {
            setError(tmpReturn);
        } //end if error
    }
    else
    {
        clearError();
        _deviceAttached = true;
        memcpy(&_diskData, &tmpdata, sizeof(tmpdata)); //copy raw data to structured variable
    }
}

uint8_t USB ::dirCreate()
{
    uint8_t tmpReturn;
    if (_interface == UARTT)
    {
        sendCommand(DIR_CREATE);
        tmpReturn = readSerDataUSB();
    }
    return tmpReturn;
}

void USB ::driveAttach()
{
    uint8_t tmpReturn = 0;
    if (_driveSource == 0)
    {
        setMode(MODE_HOST_1);
        setMode(MODE_HOST_2);
        if (_interface == UARTT)
        {
            tmpReturn = readSerDataUSB();
        }
    }
    if (tmpReturn == ANSW_USB_INT_CONNECT)
    {
        for (uint8_t a = 0; a < 5; a++)
        {
            tmpReturn = mount();
            if (tmpReturn == ANSW_USB_INT_SUCCESS)
            {
                clearError();
                _deviceAttached = true;
                break;
            }
            else if (_errorCode != CH376_ERR_TIMEOUT)
            {
                break;
            } //end if Success
        }     //end for
    }
    else
        driveDetach();
    if (_deviceAttached)
        rdDiskInfo();
}

void USB ::driveDetach()
{
    if (_driveSource == 0)
    { //if USB
        setMode(MODE_HOST_0);
    }
    _deviceAttached = false;
    rstDriveContainer();
    rstFileContainer();
}

void USB ::sendCommand(uint8_t b_parancs)
{
    if (_interface == UARTT)
    {
        write(0x57); // UART first sync command
        write(0xAB); // UART second sync command
    }                //end if
    write(b_parancs);
}

uint8_t USB ::readSerDataUSB()
{
    uint32_t oldMillis = millis();
    while (!_comPort->available())
    { // wait until data is arrive
        if ((millis() - oldMillis) > ANSWTIMEOUT)
        {
            setError(CH376_ERR_TIMEOUT);
            return 0x00; // Timeout valasz
        }                //end if
    }                    //end while
    return _comPort->read();
}

void USB ::write(uint8_t data)
{
    if (_interface == UARTT)
    {
        _comPort->write(data);
    }
}

void USB ::print(const char str[])
{
    if (_interface == UARTT)
    {
        _comPort->print(str);
    }
}

bool USB ::readFileUntil(char trmChar, char *buffer, uint8_t b_size)
{
    char tmpBuff[2]; //temporary buffer to read string and analyze
    bool readMore = true;
    uint8_t charCnt = 0;
    b_size--; // last byte is reserved for NULL terminating character
    if (!_deviceAttached)
        return false;
    for (; charCnt < b_size; charCnt++)
    {
        readMore = readFile(tmpBuff, sizeof(tmpBuff));
        buffer[charCnt] = tmpBuff[0];
        if ((tmpBuff[0] == trmChar) || !readMore)
        { // reach terminate character or EOF
            readMore = false;
            break;
        }
    }
    buffer[charCnt + 1] = 0x00;
    return readMore;
}

uint8_t USB ::readFile(char *buffer, uint8_t b_size)
{
    uint8_t tmpReturn;
    b_size--; // last byte is reserved for NULL terminating character
    tmpReturn = readMachine((uint8_t *)buffer, b_size);
    buffer[_byteCounter] = '\0'; // NULL terminating char
    _cursorPos.value += _byteCounter;
    _streamLength = _byteCounter;
    _byteCounter = 0;
    return tmpReturn;
}

uint8_t USB ::readRaw(uint8_t *buffer, uint8_t b_size)
{
    uint8_t tmpReturn;
    tmpReturn = readMachine(buffer, b_size);
    _cursorPos.value += _byteCounter;
    _streamLength = _byteCounter;
    _byteCounter = 0; // make it 0 when the buffer is full
    return tmpReturn;
}

int32_t USB ::readLong(char trmChar = '\n')
{
    char workBuffer[18];
    int32_t retval;
    readFileUntil(trmChar, workBuffer, 14);
    retval = atol(workBuffer);
    return retval;
}

uint32_t USB ::readULong(char trmChar = '\n')
{
    char workBuffer[18];
    uint32_t retval;
    readFileUntil(trmChar, workBuffer, 14);
    retval = atol(workBuffer);
    return retval;
}

double USB ::readDouble(char trmChar = '\n')
{
    double retval;
    char workBuffer[18];
    readFileUntil(trmChar, workBuffer, 14);
    retval = atof(workBuffer);
    return retval;
}

uint8_t USB ::readDataToBuff(uint8_t *buffer, uint8_t siz)
{
    uint8_t oldCounter = _byteCounter; //old buffer counter
    uint8_t dataLength = 0;            // data stream size

    if (_interface == UARTT)
    {
        sendCommand(RD_USB_DATA);
        dataLength = readSerDataUSB(); // data stream size
        if (dataLength > siz)
        {
            setError(CH376_ERR_OVERFLOW); //overflow
            return 0;
        }
        while (_byteCounter < (dataLength + oldCounter))
        {
            buffer[_byteCounter] = readSerDataUSB(); // incoming data add to buffer
            _byteCounter++;
        } //end while
    }
    return dataLength;
}

uint8_t USB ::readMachine(uint8_t *buffer, uint8_t b_size)
{
    uint8_t tmpReturn = 0; // more data
    uint8_t byteForRequest = 0;
    bool bufferFull = false;
    uint32_t tmOutCnt = 0;
    _fileWrite = 0; // read mode, required for close procedure
    if (_answer == ANSW_ERR_FILE_CLOSE || _answer == ANSW_ERR_MISS_FILE)
    {
        bufferFull = true;
        tmpReturn = 0; // we have reached the EOF
    }
    tmOutCnt = millis();

    while (!bufferFull)
    {
        if (millis() - tmOutCnt >= ANSWTIMEOUT)
            setError(CH376_ERR_TIMEOUT);
        if (!_deviceAttached)
        {
            tmpReturn = 0;
            break;
        }
        switch (fileProcesSTM)
        {
        case REQUEST:
            byteForRequest = b_size - _byteCounter;
            if (_sectorCounter == SECTORSIZE)
            { //if one sector has read out
                _sectorCounter = 0;
                fileProcesSTM = NEXT;
                break;
            }
            else if ((_sectorCounter + byteForRequest) > SECTORSIZE)
            {
                byteForRequest = SECTORSIZE - _sectorCounter;
            }
            ////////////////
            _answer = reqByteRead(byteForRequest);
            if (_answer == ANSW_USB_INT_DISK_READ)
            {
                fileProcesSTM = READWRITE;
                tmpReturn = 1; //we have not reached the EOF
            }
            else if (_answer == ANSW_USB_INT_SUCCESS)
            { // no more data, EOF
                fileProcesSTM = DONE;
                tmpReturn = 0;
            }
            break;
        case READWRITE:
            _sectorCounter += readDataToBuff(buffer, byteForRequest); //fillup the buffer
            if (_byteCounter != b_size)
            {
                fileProcesSTM = REQUEST;
            }
            else
            {
                fileProcesSTM = DONE;
            }
            break;
        case NEXT:
            _answer = byteRdGo();
            fileProcesSTM = REQUEST;
            break;
        case DONE:
            fileProcesSTM = REQUEST;
            bufferFull = true;
            break;
        } //end switch
    }     //end while
    return tmpReturn;
}

void USB ::setSpeed()
{
    if (_speed == 9600)
    {
        // do nothing
    }
    else
    {
        sendCommand(SET_BAUDRATE);
        switch (_speed)
        {
        case 19200:
            _comPortHW->write(uint8_t(0x02)); //detach freq. coef
            _comPortHW->write(uint8_t(0xD9)); //detach freq. constant
            break;
        case 57600:
            _comPortHW->write(uint8_t(0x03));
            _comPortHW->write(uint8_t(0x98));
            break;
        case 115200:
            _comPortHW->write(uint8_t(0x03));
            _comPortHW->write(uint8_t(0xCC));
            break;
        default:
            _speed = 9600;
            break;
        } //end switch
        _comPortHW->end();
        _comPortHW->begin(_speed);
        delay(2);
    }
}

void USB ::setSource(uint8_t inpSource)
{
    if (_driveSource != inpSource)
    {
        _driveSource = inpSource;
        if (_driveSource == 1)
        { // SD mode
            driveDetach();
            setMode(MODE_DEFAULT);
            setMode(MODE_HOST_SD);
            driveAttach();
        }
        else
        { // USB mode
            driveDetach();
            driveAttach();
        } //end if SD
    }     //end if not
}

uint8_t USB ::setMode(uint8_t mode)
{
    uint8_t tmpReturn = 0;
    if (_interface == UARTT)
    {
        sendCommand(SET_USB_MODE);
        write(mode);
        tmpReturn = readSerDataUSB();
    }
    return tmpReturn;
}

uint8_t USB :: getSource()
{
    return _driveSource;
}

bool USB :: getDeviceStatus()
{
    return _deviceAttached;
}

bool USB :: getCHpresence()
{
    return _controllerReady;
}

uint8_t USB :: getFileAttrb()
{
    return _fileData.fattr;
}

char* USB :: getFileName()
{
    strncpy(_filename, _fileData.name,11);
    _filename[11] = '\0';
    return _filename;
}

void USB :: setFileName(const char* filename)
{
    if(_deviceAttached)
    {
        if(strlen(filename))
        {
            strncpy(_filename, filename, 12);
        }
        else
        {
            getFileName();
        }
        sendFilename();
    }
}

uint8_t USB :: getStatus()
{
    return _answer;
}

uint32_t USB :: getFileSize()
{
    return _fileData.size;
}

uint32_t USB :: getCursorPos()
{
    return _cursorPos.value;
}

char* USB :: getFileSizeStr()
{
    static char fsizeString[10];
    fsizeString[0] = 0;						  // or 1023,9 Kb\0
	uint32_t ul_size = _fileData.size;
	float fl_size;
	char strNumber[7]; // e.g 1023,9\0 , temporary buffer
	if(_deviceAttached){
		if(ul_size >= 1048576){ // if the filesize is 1Mb or bigger
			fl_size = ul_size / 1048576.0;
			dtostrf(fl_size, 1, 1, fsizeString);//convert float to string
			strcat(fsizeString," Mb");  // concatenate unit symbol
		} else if(ul_size >= 1024){ // if the filesize is in Kb range
			fl_size = ul_size / 1024.0;
			dtostrf(fl_size, 1, 1, fsizeString);//convert float to string
			strcat(fsizeString," Kb");
		} else { // if the filesize is in byte range
			ltoa(ul_size, strNumber, 10);// convert long to string
			strcpy(fsizeString,strNumber);// copy to the final string
			strcat(fsizeString," byte");// concatenate unit symbol
		}//end size
	}//end if attached

	return fsizeString; //return the final string
}

void USB :: setYear(uint16_t year)
{
    if(year > 2099) year = 2099;
    if(year < 1980) year = 1980;
    year -= 1980;
    constructDate(year, 0);
}

uint16_t USB :: getYear()
{
    uint16_t year = _fileData.modDate;
	year = year>> 9;
	year += 1980;
	return year;
}

void USB :: setMonth(uint16_t month)
{
    if(month > 12) month = 12;
    if(month < 1) month = 1;
    constructDate(month, 1);
}

uint16_t USB :: getMonth()
{
    uint16_t month = _fileData.modDate;
    month = month << 7;
    month = month >> 12;
    return month;
}

void USB :: setDay(uint16_t day)
{
    if(day > 31) day = 31;
    if(day < 1) day = 1;
    constructDate(day, 2);
}

uint16_t USB :: getDay()
{
    uint16_t day = _fileData.modDate;
    day = day << 11;
    day = day >> 11;
    return day;
}

void USB :: setHour(uint16_t hour)
{
    if(hour > 23) hour = 23;
    constructTime(hour, 0);
}

uint16_t USB :: getHour()
{
    uint16_t hour = _fileData.modTime;
    hour = hour >> 11;
    return hour;
}

void USB :: setMinute(uint16_t minute)
{
    if(minute > 59) minute = 59;
    constructTime(minute, 1);
}

uint16_t USB :: getMinute()
{
    uint16_t minute = _fileData.modTime;
	minute = minute << 5;
	minute = minute >> 10;
	return minute;
}

void USB :: setSecond(uint16_t second)
{
    if(second > 59) second = 59;
	second /= 2;
	constructTime(second, 2);
}

uint16_t USB :: getSecond(){
	uint16_t second = _fileData.modTime;
	second = second << 11;
	second = second >> 11;
	second *= 2;
	return second;
}

void USB :: constructDate(uint16_t value, uint8_t ymd){ // 0-year, 1-month, 2-day
	uint16_t tmpInt = _fileData.modDate;
	uint16_t year;
	uint16_t month;
	uint16_t day;
	/*<------- 0x19 --------> <------- 0x18 -------->
	 *15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
	 *y  y  y  y  y  y  y  m  m  m  m  d  d  d  d  d
	 */
	if(_deviceAttached){
		year = tmpInt >> 9;
		year = year << 9;

		month = tmpInt << 7;
		month = month >> 12;
		month = month << 5;

		day = tmpInt << 11;
		day = day >> 11;

		switch (ymd) {
			case 0://year
				year = value;
				year = year << 9;
				break;
			case 1://month
				month = value;
				month = month << 5;
				break;
			case 2://day
				day = value;
				break;
			default:
				break;
		}//end switch
		_fileData.modDate = year + month + day;
	}//end if attached
}

void USB :: constructTime(uint16_t value, uint8_t hms){
	uint16_t tmpInt = _fileData.modTime;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	/*<------- 0x17 --------> <------- 0x16 -------->
	 *15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
	 *h  h  h  h  h  m  m  m  m  m  m  s  s  s  s  s
	 */
	if(_deviceAttached){
		hour = tmpInt >> 11; //cut minute and second
		hour = hour << 11; // set hour

		minute = tmpInt << 5; // cut hour
		minute = minute >> 10;// cut seconds
		minute = minute << 5; // set minute

		second = tmpInt << 11; // cut hour and minute
		second = second >> 11; // set second

		switch (hms) {
			case 0://hour
				hour = value;
				hour = hour << 11;
				break;
			case 1://minute
				minute = value;
				minute = minute << 5;
				break;
			case 2://second
				second = value;
				break;
			default:
				break;
		}//end switch
		_fileData.modTime = hour + minute + second;
	}//end if attached
}

uint32_t USB :: getTotalSectors()
{
    return _diskData.totalSector;
}

uint32_t USB :: getFreeSectors()
{
    return _diskData.freeSector;
}

uint8_t USB :: getFileSystem()
{
    return _diskData.diskFat;
}

void USB :: clearError()
{
    _errorCode = 0;
}

void USB :: setError(uint8_t errCode)
{
    _errorCode = errCode;
	_deviceAttached = false;
	_dirDepth = 0;
	_byteCounter = 0;
	_answer = 0;
	resetFileList();
	rstDriveContainer();
	rstFileContainer();
}

void USB :: rstDriveContainer()
{
    memset(&_diskData, 0, sizeof(_diskData));
}

void USB :: rstFileContainer()
{
    memset(&_fileData, 0, sizeof(_fileData));// fill up with NULL file data container
	_filename[0] = '\0'; // put  NULL char at the first place in a name string
	_fileWrite = 0;
	_sectorCounter = 0;
	_cursorPos.value = 0;
	_streamLength = 0;
}

uint8_t USB :: getError()
{
    return _errorCode;
}

void USB :: resetFileList()
{
    fileProcesSTM = REQUEST;
}

bool USB :: getEOF()
{
    if(_cursorPos.value < _fileData.size){
		return false;
	} else {
		return true;
	}
}

uint8_t USB :: getChipVer(){
	uint8_t tmpReturn = 0;
	if(_interface == UARTT){
		sendCommand(GET_IC_VER);
		//write(mode);
		tmpReturn = readSerDataUSB();
	} 
	return tmpReturn;
}

uint8_t USB :: getStreamLen(){
	return _streamLength;
}

uint8_t USB :: writeFile(char* buffer, uint8_t b_size)
{
    return writeMachine((uint8_t*)buffer, b_size);
}

uint8_t USB :: writeRaw(uint8_t* buffer, uint8_t b_size)
{
    return writeMachine(buffer, b_size);
}

uint8_t USB :: writeChar(char trmChar)
{
    return writeMachine((uint8_t*)&trmChar, 1);
}

uint8_t USB :: writeNum(uint8_t buffer){
	char strBuffer[4];//max 255 = 3+1 char
	itoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNum(int8_t buffer){
	char strBuffer[5];//max -128 = 4+1 char
	itoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNumln(uint8_t buffer){
	char strBuffer[6];//max 255 = 3+2+1 char
	itoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNumln(int8_t buffer){
	char strBuffer[7];//max -128 = 4+2+1 char
	itoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeMachine(uint8_t* buffer, uint8_t b_size){
	bool diskFree = true; //free space on a disk
	bool bufferFull = true; //continue to write while there is data in the temporary buffer
	uint32_t tmOutCnt = 0;
	if(!_deviceAttached) return 0x00;
	_fileWrite = 1; // read mode, required for close procedure
	_byteCounter = 0;

	if(_diskData.freeSector == 0){
		diskFree = false;
		return diskFree;
	}
	if(_answer == ANSW_ERR_MISS_FILE){ // no file with given name
		_answer = fileCreate();
	}//end if CREATED

	if(_answer == ANSW_ERR_FILE_CLOSE){
		_answer = openFile();
	}

	if(_answer == ANSW_USB_INT_SUCCESS){ // file created succesfully
		tmOutCnt = millis();
		while(bufferFull){
			if(millis() - tmOutCnt >= ANSWTIMEOUT) setError(CH376_ERR_TIMEOUT);
			if(!_deviceAttached){
				diskFree = false;
				break;
			}
			switch (fileProcesSTM) {
				case REQUEST:
					_answer = reqByteWrite(b_size - _byteCounter);

					if(_answer == ANSW_USB_INT_SUCCESS){
						fileProcesSTM = NEXT;

					} else if(_answer == ANSW_USB_INT_DISK_WRITE){
						fileProcesSTM = READWRITE;
						}
					break;
				case READWRITE:
					writeDataFromBuff(buffer);
					if(_byteCounter != b_size) {
						fileProcesSTM = NEXT;
					} else {
						fileProcesSTM = DONE;
					}
					break;
				case NEXT:
					if(_diskData.freeSector > 0){
						_diskData.freeSector --;
						_answer = byteWrGo();
						if(_answer == ANSW_USB_INT_SUCCESS){
							fileProcesSTM = REQUEST;
						} else if(_byteCounter != b_size ){
							fileProcesSTM = READWRITE;
						}
					} else { // if disk is full
						fileProcesSTM = DONE;
						diskFree = false;
					}
					break;
				case DONE:
					fileProcesSTM = REQUEST;
					_cursorPos.value += _byteCounter;
					_byteCounter = 0;
					_answer = byteWrGo();
					bufferFull = false;
					break;
			}//end switch
		}//end while
	}// end file created

	return diskFree;
}

uint8_t USB :: writeDataFromBuff(uint8_t* buffer){//====================
	uint8_t oldCounter = _byteCounter; //old buffer counter
	uint8_t dataLength; // data stream size
	if(_interface == UARTT) {
		sendCommand(WR_REQ_DATA);
		dataLength = readSerDataUSB(); // data stream size
	} 
	while(_byteCounter < (dataLength + oldCounter)){
		write(buffer[_byteCounter]); // read data from buffer and write to serial port
		_byteCounter ++;
	}//end while
	return dataLength;
}

uint8_t USB :: writeNum(double buffer){
	char strBuffer[15];
	if(buffer > 4100000.00 || buffer < -4100000.00){//beyond these values, the accuracy decreases rapidly
		strcpy(strBuffer,"ovf");					//constant determined by trial and error
	} else {
		dtostrf(buffer, 1, 2, strBuffer);
	}
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNumln(double buffer){
	char strBuffer[15];
	if(buffer > 4100000.00 || buffer < -4100000.00){
		strcpy(strBuffer,"ovf");
	} else {
		dtostrf(buffer, 1, 2, strBuffer);
	}
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNumln(uint32_t buffer){
	char strBuffer[13];//max 4 294 967 295 = 10+2+1 char
	ltoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB :: writeNumln(int32_t buffer){
	char strBuffer[14];//max -2147483648 = 11+2+1 char
	ltoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB::writeNum(uint32_t buffer){
	char strBuffer[11];//max 4 294 967 295 = 10+1 char
	ltoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB::writeNum(int32_t buffer){
	char strBuffer[12];//max -2147483648 = 11+1 char
	ltoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}

uint8_t USB::writeNum(uint16_t buffer){
	char strBuffer[6];//max 65535 = 5+1 char
	itoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}
//	//	//	//	//
uint8_t USB::writeNum(int16_t buffer){
	char strBuffer[7];//max -32768 = 6+1 char
	itoa(buffer, strBuffer, 10);
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}
///////////////////////////ln16/////////////////////////////////
uint8_t USB::writeNumln(uint16_t buffer){
	char strBuffer[8];//max 65535 = 5+2+1 char
	itoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}
//	//	//	//	//
uint8_t USB::writeNumln(int16_t buffer){
	char strBuffer[9];//max -32768 = 6+2+1 char
	itoa(buffer, strBuffer, 10);
	strcat(strBuffer,"\r\n");
	return writeMachine((uint8_t*)strBuffer,strlen(strBuffer));
}