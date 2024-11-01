#include "imu.h"

//Fuctions Definition
bool IMU ::MPU_begin(gyro_fs_sel scale, accel_fs_sel range, clock_source source)
{
    Wire.begin();
    setClockSource(source);
    setScale(scale);
    setRange(range);
    i2cBypass();
    return 1;
}

bool IMU ::BMM_begin(data_Rate rate, operation_Mode mode)
{
    setupBMM();
    delay(100);
    setDataRate(rate);
    setOperationMode(mode);
    return 1;
}

bool IMU ::testConnection()
{
    uint8_t value;
    value = readRegister8(BMM150_ADR, CHIP_ID);
    return value;
}

void IMU ::setupBMM()
{
    writeRegister8(BMM150_ADR, PWR_MNGT_REG, PWR_MNGT_REG_BIT);
}

void IMU ::setDataRate(data_Rate rate)
{
    uint8_t value;
    value = readRegister8(BMM150_ADR, USER_CTRL_REG);
    value &= 0b11000111;
    value |= (rate << 3);
    writeRegister8(BMM150_ADR, USER_CTRL_REG, value);
}

void IMU ::setOperationMode(operation_Mode mode)
{
    uint8_t value;
    value = readRegister8(BMM150_ADR, USER_CTRL_REG);
    value &= 0b11111001;
    value |= (mode << 1);
    writeRegister8(BMM150_ADR, USER_CTRL_REG, value);
}

Vector IMU ::magRawData()
{
    lsb.x = readRegister8(BMM150_ADR, MAG_XOUT_L) >> 3;
    msb.x = readRegister8(BMM150_ADR, MAG_XOUT_H) * 32;
    raw.xAxis = (msb.x | lsb.x);
    lsb.y = readRegister8(BMM150_ADR, MAG_YOUT_L) >> 3;
    msb.y = readRegister8(BMM150_ADR, MAG_YOUT_H) * 32;
    raw.yAxis = (msb.y | lsb.y);
    lsb.z = readRegister8(BMM150_ADR, MAG_ZOUT_L) >> 1;
    msb.z = readRegister8(BMM150_ADR, MAG_ZOUT_H) * 128;
    raw.zAxis = (msb.z | lsb.z);
    return raw;
}

Vector IMU ::calibrateMag()
{
    float timer = 0;
    while (timer < 15)
    {
        magRawData();
        if (raw.xAxis < magMin.xAxis)
            magMin.xAxis = raw.xAxis;
        if (raw.xAxis > magMax.xAxis)
            magMax.xAxis = raw.xAxis;
        if (raw.yAxis < magMin.yAxis)
            magMin.yAxis = raw.yAxis;
        if (raw.yAxis > magMax.yAxis)
            magMax.yAxis = raw.yAxis;
        if (raw.zAxis < magMin.zAxis)
            magMin.zAxis = raw.zAxis;
        if (raw.zAxis > magMax.zAxis)
            magMax.zAxis = raw.zAxis;
        delay(100);
        timer += 0.1;
    }
    offMag.xAxis = (magMin.xAxis + magMax.xAxis) / 2;
    offMag.yAxis = (magMin.yAxis + magMax.yAxis) / 2;
    offMag.zAxis = (magMin.zAxis + magMax.zAxis) / 2;
    return offMag;
}

Vector IMU ::getMagData()
{
    magRawData();

    mag.xAxis = raw.xAxis - offMag.xAxis;
    mag.yAxis = raw.yAxis - offMag.yAxis;
    mag.zAxis = raw.zAxis - offMag.zAxis;

    mag.xAxis = mag.xAxis * 0.3;
    mag.yAxis = mag.yAxis * 0.3;
    mag.zAxis = mag.zAxis * 0.3;
    return mag;
}

Vector IMU ::magVector()
{
    getMagData();
    BVector.xAxis = -mag.yAxis;
    BVector.yAxis = mag.xAxis;
    BVector.zAxis = mag.zAxis;
    return BVector;
}

bool IMU ::whoAmI()
{
    uint8_t value;
    value = readRegister8(MPU9250_ADR, WHO_AM_I);
    return value;
}

void IMU ::setClockSource(clock_source source)
{
    uint8_t value;

    value = readRegister8(MPU9250_ADR, PWR_MNGT_1);
    //Serial.println(value);
    value &= 0b10111000;
    //Serial.println(value);
    value |= source;
    //Serial.println(value);
    writeRegister8(MPU9250_ADR, PWR_MNGT_1, value);
}

void IMU ::setScale(gyro_fs_sel scale)
{
    uint8_t value;

    switch (scale)
    {
    case GYRO_FS_SEL_250:
        gyroSensitivity = .007633f;
        break;
    case GYRO_FS_SEL_500:
        gyroSensitivity = .015267f;
        break;
    case GYRO_FS_SEL_1000:
        gyroSensitivity = .030487f;
        break;
    case GYRO_FS_SEL_2000:
        gyroSensitivity = .060975f;
        break;
    default:
        break;
    }

    value = readRegister8(MPU9250_ADR, GYRO_CONFIG);
    value &= 0b11100111;
    value |= (scale << 3);
    //Serial.println(value);
    writeRegister8(MPU9250_ADR, GYRO_CONFIG, value);
}

void IMU ::setRange(accel_fs_sel range)
{
    uint8_t value;

    switch (range)
    {
    case ACCEL_FS_SEL_2G:
        accelSensitivity = .000061f;
        break;
    case ACCEL_FS_SEL_4G:
        accelSensitivity = .000122f;
        break;
    case ACCEL_FS_SEL_8G:
        accelSensitivity = .000244f;
        break;
    case ACCEL_FS_SEL_16G:
        accelSensitivity = .0004882f;
        break;
    default:
        break;
    }

    value = readRegister8(MPU9250_ADR, ACCEL_CONFIG);
    value &= 0b11100111;
    value |= (range << 3);
    //Serial.print(value);
    writeRegister8(MPU9250_ADR, ACCEL_CONFIG, value);
}

void IMU ::i2cBypass()
{
    writeRegister8(MPU9250_ADR, USER_CTRL, USER_CTRL_BIT);
    writeRegister8(MPU9250_ADR, INT_PIN_CFG, INT_PIN_CFG_BIT);
}

void IMU ::setDlpfGyro(dlpf_gyro freq)
{
    uint8_t value;

    value = readRegister8(MPU9250_ADR, CONFIG);
    value &= 0b11111000;
    value |= freq;
    writeRegister8(MPU9250_ADR, CONFIG, value);
}

void IMU ::setDlpfAccel(dlpf_accel freq)
{
    uint8_t value;

    value = readRegister8(MPU9250_ADR, ACCEL_CONFIG_2);
    value &= 0b11111000;
    value |= freq;
    writeRegister8(MPU9250_ADR, ACCEL_CONFIG_2, value);
}

Vector IMU ::gyroRawData()
{
    Wire.beginTransmission(MPU9250_ADR);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU9250_ADR, 6);
    while (Wire.available() < 6)
        ;
    raw.xAxis = (Wire.read() << 8 | Wire.read()) * gyroSensitivity;
    raw.yAxis = (Wire.read() << 8 | Wire.read()) * gyroSensitivity;
    raw.zAxis = (Wire.read() << 8 | Wire.read()) * gyroSensitivity;
    return raw;
}

Vector IMU ::setGyroOffset()
{
    int i = 0;
    while (i < 500)
    {
        gyroRawData();
        off.xAxis += raw.xAxis;
        off.yAxis += raw.yAxis;
        off.zAxis += raw.zAxis;
        i++;
    }
    off.xAxis /= 500;
    off.yAxis /= 500;
    off.zAxis /= 500;
    return off;
}

Vector IMU ::gyroCalibratedData()
{
    gyroRawData();
    cal.xAxis = raw.xAxis - off.xAxis;
    cal.yAxis = raw.yAxis - off.yAxis;
    cal.zAxis = raw.zAxis - off.zAxis;
    return cal;
}

Vector IMU ::gyroVector()
{
    gyroCalibratedData();
    wVector.xAxis = (cal.yAxis * PI) / 180;
    wVector.yAxis = (cal.xAxis * PI) / 180;
    wVector.zAxis = -(cal.zAxis * PI) / 180;
    return wVector;
}

Vector IMU ::accelRawData()
{
    Wire.beginTransmission(MPU9250_ADR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU9250_ADR, 6);
    while (Wire.available() < 6)
        ;
    accel.xAxis = ((Wire.read() << 8 | Wire.read()) + 1300) * accelSensitivity;
    accel.yAxis = (Wire.read() << 8 | Wire.read()) * accelSensitivity;
    accel.zAxis = (Wire.read() << 8 | Wire.read()) * accelSensitivity;

    return accel;
}

Vector IMU ::accelVector()
{
    accelRawData();
    gVector.xAxis = -(accel.yAxis * 9.80665f);
    gVector.yAxis = -(accel.xAxis * 9.80665f);
    gVector.zAxis = (accel.zAxis * 9.80665f);
    return gVector;
}

ANGLE IMU ::calAttitude()
{
    gyroVector();
    accelVector();
    magVector();

    Raw.phi = atan2(gVector.yAxis, gVector.zAxis);
    Raw.theta = atan2(-gVector.xAxis, sqrt(pow(gVector.yAxis, 2) + pow(gVector.zAxis, 2)));
    Raw.psi = atan2(BVector.zAxis * sin(Raw.phi) - BVector.yAxis * cos(Raw.phi),
                    BVector.xAxis * cos(Raw.theta) + BVector.yAxis * sin(Raw.theta) * sin(Raw.phi) +
                        BVector.zAxis * sin(Raw.theta) * cos(Raw.phi));

    // Calculating Change in Phi, Psi and Theta with Respect to Time.
    Dot.phi = wVector.xAxis + wVector.yAxis * sin(Raw.phi) * tan(Raw.theta) + wVector.zAxis * cos(Raw.phi) * tan(Raw.theta);
    Dot.theta = wVector.yAxis * cos(Raw.phi) - wVector.zAxis * sin(Raw.phi);
    Dot.psi = wVector.yAxis * (sin(Raw.phi) / cos(Raw.theta)) + wVector.zAxis * (cos(Raw.phi) / cos(Raw.theta));

    // Calculating Estimated Phi, Psi and Theta using Complementry Filter.
    initial_T = final_T;
    final_T = millis();
    delta_T = (final_T - initial_T) / 1000.00;

    Est.phi = ALPHA * (Est.phi + Dot.phi * delta_T) + (1 - ALPHA) * Raw.phi;
    Est.theta = ALPHA * (Est.theta + Dot.theta * delta_T) + (1 - ALPHA) * Raw.theta;
    Est.psi = ALPHA * (Est.psi + Dot.psi * delta_T) + (1 - ALPHA) * Raw.psi;

    return Est;
}

void IMU ::gyro_data_To_EEprom()
{
    setGyroOffset();
    eeprom_Adr = 0;
    EEPROM.put(eeprom_Adr, off);
    eeprom_Adr += sizeof(off);
}

void IMU ::mag_data_To_EEprom()
{
    calibrateMag();
    EEPROM.put(eeprom_Adr, offMag);
    eeprom_Adr += sizeof(offMag);
}

Vector IMU ::gyro_data_From_EEprom()
{
    eeprom_Adr = 0;
    EEPROM.get(eeprom_Adr, off);
    eeprom_Adr += sizeof(off);
    return off;
}

Vector IMU ::mag_data_From_EEprom()
{
    EEPROM.get(eeprom_Adr, offMag);
    eeprom_Adr += sizeof(offMag);
    return offMag;
}

uint8_t IMU ::readRegister8(uint8_t addr, uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    while (Wire.available() < 1)
        ;
    value = Wire.read();
    return value;
}

void IMU ::writeRegister8(uint8_t addr, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}
