#ifndef _IMU_H
#define _IMU_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ICM_20948.h>

#define ALPHA 0.6
#define AD_VAL 1

struct Vector
{
    double xAxis;
    double yAxis;
    double zAxis;
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
    Vector gyro, accel, mag;
    Vector magMin, magMax;
    Vector offMag;
    Vector magCalibrated;
    Vector b_Vector;
    Vector offGyro;
    Vector gyroCalibrated;
    Vector w_Vector;
    Vector g_Vector;
    ANGLE Raw, Dot, Est;
    long final_T;
    long initial_T;
    double delta_T;
    int eeprom_Adr;

    Vector getMagRawData();
    Vector getMagOffset();
    Vector getMagCalibratedData();
    Vector getMagVector();
    Vector getGyroRawData();
    Vector getGyroOffset();
    Vector getGyroCalibratedData();
    Vector getGyroVector();
    Vector getAccelRawData();
    Vector getAccelVector();

public:
    void imuBegin();
    ANGLE calAttitude();
    void gyro_data_To_EEprom();
    void mag_data_To_EEprom();
    Vector gyro_data_From_EEprom();
    Vector mag_data_From_EEprom();
};

#endif