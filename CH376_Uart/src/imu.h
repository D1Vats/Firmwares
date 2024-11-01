#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#define ALPHA 0.96

// Register Mapping of BMM150
#define BMM150_ADR (0x13)
#define CHIP_ID (0x40)
#define MAG_XOUT_L (0x42)
#define MAG_XOUT_H (0x43)
#define MAG_YOUT_L (0x44)
#define MAG_YOUT_H (0x45)
#define MAG_ZOUT_L (0x46)
#define MAG_ZOUT_H (0x47)
#define HALL_LSB (0x48)
#define HALL_MSB (0x49)
#define INT_STAT_REG (0x4A)
#define PWR_MNGT_REG (0x4B)
#define USER_CTRL_REG (0x4C)
#define INT_CTRL_REG_1 (0x4D)
#define INT_CTRL_REG_2 (0x4E)
#define LOW_THRESH_INT (0x4F)
#define HIGH_THRESH_INT (0x50)
#define REP_CTRL_REG_1 (0x51)
#define REP_CTRL_REG_2 (0x52)

// Register mapping For  MPU9250
#define MPU9250_ADR (0x69)
#define XG_OFFSET_H (0x13)
#define XG_OFFSET_L (0x14)
#define YG_OFFSET_H (0x15)
#define YG_OFFSET_L (0x16)
#define ZG_OFFSET_H (0x17)
#define ZG_OFFSET_L (0x18)
#define SMPLRT_DIV (0x19)
#define CONFIG (0x1A)
#define GYRO_CONFIG (0x1B)
#define ACCEL_CONFIG (0x1C)
#define ACCEL_CONFIG_2 (0x1D)
#define INT_PIN_CFG (0x37)
#define ACCEL_XOUT_H (0x3B)
#define ACCEL_XOUT_L (0x3C)
#define ACCEL_YOUT_H (0x3D)
#define ACCEL_YOUT_L (0x3E)
#define ACCEL_ZOUT_H (0x3F)
#define ACCEL_ZOUT_L (0x40)
#define TEMP_OUT_H (0x41)
#define TEMP_OUT_L (0x42)
#define GYRO_XOUT_H (0x43)
#define GYRO_XOUT_L (0x44)
#define GYRO_YOUT_H (0x45)
#define GYRO_YOUT_L (0x46)
#define GYRO_ZOUT_H (0x47)
#define GYRO_ZOUT_L (0x48)
#define USER_CTRL (0x6A)
#define PWR_MNGT_1 (0x6B)
#define PWR_MNGT_2 (0x6C)
#define WHO_AM_I (0x75)

// Bit Assignment for Register
#define PWR_MNGT_REG_BIT (0x01)

// Bit Defination to each Registers
#define USER_CTRL_BIT (0x00)
#define INT_PIN_CFG_BIT (0x02)

//Data rate For Compass
typedef enum
{
    MAG_OUT_DATA_RATE_10_HZ = (0b000),
    MAG_OUT_DATA_RATE_2_HZ = (0b001),
    MAG_OUT_DATA_RATE_6_HZ = (0b010),
    MAG_OUT_DATA_RATE_8_HZ = (0b011),
    MAG_OUT_DATA_RATE_15_HZ = (0b100),
    MAG_OUT_DATA_RATE_20_HZ = (0b101),
    MAG_OUT_DATA_RATE_25_HZ = (0b110),
    MAG_OUT_DATA_RATE_30_HZ = (0b111)
} data_Rate;

//Mode of Operation for Compass
typedef enum
{
    MAG_OP_MODE_NORM = (0b00),
    MAG_OP_MODE_FORC = (0b01),
    MAG_OP_MODE_RES = (0b10),
    MAG_OP_MODE_SLEEP = (0b11)
} operation_Mode;

//Data rate for MPU9250
typedef enum
{
    INTERNAL_20HZ = (0b000),
    AUTO_SELECT_1 = (0b001),
    AUTO_SELECT_2 = (0b010),
    AUTO_SELECT_3 = (0b011),
    AUTO_SELECT_4 = (0b100),
    AUTO_SELECT_5 = (0b101),
    INTERNAL_20HZ_2 = (0b110),
    STOP_THE_CLOCK = (0b111)
} clock_source;

//Gyroscope Full Scale Range
typedef enum
{
    GYRO_FS_SEL_250 = (0b00),
    GYRO_FS_SEL_500 = (0b01),
    GYRO_FS_SEL_1000 = (0b10),
    GYRO_FS_SEL_2000 = (0b11)
} gyro_fs_sel;

//Accelerometer Full Scale Range
typedef enum
{
    ACCEL_FS_SEL_2G = (0b00),
    ACCEL_FS_SEL_4G = (0b01),
    ACCEL_FS_SEL_8G = (0b10),
    ACCEL_FS_SEL_16G = (0b11)
} accel_fs_sel;

// Digital Low Pass Filter For GyroScope
typedef enum
{
    DLPF_CFG_250 = (0b000),
    DLPF_CFG_184 = (0b001),
    DLPF_CFG_92 = (0b010),
    DLPF_CFG_41 = (0b011),
    DLPF_CFG_20 = (0b100),
    DLPF_CFG_10 = (0b101),
    DLPF_CFG_5 = (0b110),
    DLPF_CFG_3600 = (0b111)
} dlpf_gyro;

//Digital Low Pass Filter for Accelerometer
typedef enum
{
    A_DLPF_CFG_0 = (0b000),
    A_DLPF_CFG_1 = (0b001),
    A_DLPF_CFG_2 = (0b010),
    A_DLPF_CFG_3 = (0b011),
    A_DLPF_CFG_4 = (0b100),
    A_DLPF_CFG_5 = (0b101),
    A_DLPF_CFG_6 = (0b110),
    A_DLPF_CFG_7 = (0b111)
} dlpf_accel;

// Structure for Three Axes
struct Vector
{
    double xAxis;
    double yAxis;
    double zAxis;
};

struct VECTOR
{
    int8_t x;
    int8_t y;
    int8_t z;
};

struct ANGLE
{
    double phi;
    double theta;
    double psi;
};

class IMU
{
private:
    Vector raw;
    Vector off, cal, wVector, gVector;
    Vector accel;
    Vector mag;
    Vector BVector;
    VECTOR lsb;
    VECTOR msb;
    Vector magMin;
    Vector magMax;
    Vector offMag;
    ANGLE Raw, Dot, Est;
    int buffer;
    float gyroSensitivity;
    float accelSensitivity;
    long final_T;
    long initial_T;
    double delta_T;
    int eeprom_Adr;
    uint8_t readRegister8(uint8_t addr, uint8_t reg);
    //uint8_t readRegister8_MPU(uint8_t reg);
    void writeRegister8(uint8_t addr, uint8_t reg, uint8_t value);
    //void writeRegister8_MPU(uint8_t reg, int16_t value);
public:
    bool MPU_begin(gyro_fs_sel scale, accel_fs_sel range, clock_source source);
    bool BMM_begin(data_Rate rate, operation_Mode mode);
    bool testConnection();
    void setupBMM();
    void setDataRate(data_Rate rate);
    void setOperationMode(operation_Mode mode);
    Vector magRawData();
    Vector calibrateMag();
    Vector getMagData();
    Vector magVector();
    bool whoAmI();
    void setClockSource(clock_source source);
    void setScale(gyro_fs_sel scale);
    void setRange(accel_fs_sel range);
    void i2cBypass();
    Vector gyroRawData();
    Vector setGyroOffset();
    Vector gyroCalibratedData();
    Vector gyroVector();
    Vector accelRawData();
    Vector accelVector();
    void setDlpfGyro(dlpf_gyro freq);
    void setDlpfAccel(dlpf_accel freq);
    ANGLE calAttitude();
    void gyro_data_To_EEprom();
    void mag_data_To_EEprom();
    Vector gyro_data_From_EEprom();
    Vector mag_data_From_EEprom();
};

#endif