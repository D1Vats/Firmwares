#include "imu.h"

ICM_20948_I2C myICM;

ICM_20948_AGMT_t agmt;

void IMU ::imuBegin()
{
    Wire.begin();
    Wire.setClock(400000);
    myICM.begin(Wire, AD_VAL);
}

Vector IMU ::getMagRawData()
{
    myICM.getAGMT();
    mag.xAxis = myICM.agmt.mag.axes.x;
    mag.yAxis = myICM.agmt.mag.axes.y;
    mag.zAxis = myICM.agmt.mag.axes.z;
    return mag;
}

Vector IMU ::getMagOffset()
{
    float timer = 0;
    while (timer < 15)
    {
        getMagRawData();
        if (mag.xAxis < magMin.xAxis)
            magMin.xAxis = mag.xAxis;
        if (mag.xAxis > magMax.xAxis)
            magMax.xAxis = mag.xAxis;
        if (mag.yAxis < magMin.yAxis)
            magMin.yAxis = mag.yAxis;
        if (mag.yAxis > magMax.yAxis)
            magMax.yAxis = mag.yAxis;
        if (mag.zAxis < magMin.zAxis)
            magMin.zAxis = mag.zAxis;
        if (mag.zAxis > magMax.zAxis)
            magMax.zAxis = mag.zAxis;
        delay(100);
        timer += 0.1;
    }
    offMag.xAxis = (magMin.xAxis + magMax.xAxis) / 2;
    offMag.yAxis = (magMin.yAxis + magMax.yAxis) / 2;
    offMag.zAxis = (magMin.zAxis + magMax.zAxis) / 2;

    return offMag;
}

Vector IMU ::getMagCalibratedData()
{
    getMagRawData();

    magCalibrated.xAxis = mag.xAxis - offMag.xAxis;
    magCalibrated.yAxis = mag.yAxis - offMag.yAxis;
    magCalibrated.zAxis = mag.zAxis - offMag.zAxis;

    magCalibrated.xAxis = magCalibrated.xAxis * 0.15;
    magCalibrated.yAxis = magCalibrated.yAxis * 0.15;
    magCalibrated.zAxis = magCalibrated.zAxis * 0.15;

    return magCalibrated;
}

Vector IMU ::getMagVector()
{
    getMagCalibratedData();

    b_Vector.xAxis = magCalibrated.xAxis;
    b_Vector.yAxis = magCalibrated.yAxis;
    b_Vector.zAxis = magCalibrated.zAxis;

    return b_Vector;
}

Vector IMU ::getGyroRawData()
{
    myICM.getAGMT();
    gyro.xAxis = myICM.agmt.gyr.axes.x / 131.0;
    gyro.yAxis = myICM.agmt.gyr.axes.y / 131.0;
    gyro.zAxis = myICM.agmt.gyr.axes.z / 131.0;

    return gyro;
}

Vector IMU ::getGyroOffset()
{
    int i = 0;
    while (i < 500)
    {
        getGyroRawData();
        offGyro.xAxis += gyro.xAxis;
        offGyro.yAxis += gyro.yAxis;
        offGyro.zAxis += gyro.zAxis;
        i++;
    }
    offGyro.xAxis /= 500;
    offGyro.yAxis /= 500;
    offGyro.zAxis /= 500;

    return offGyro;
}

Vector IMU ::getGyroCalibratedData()
{
    getGyroRawData();

    gyroCalibrated.xAxis = gyro.xAxis - offGyro.xAxis;
    gyroCalibrated.yAxis = gyro.yAxis - offGyro.yAxis;
    gyroCalibrated.zAxis = gyro.zAxis - offGyro.zAxis;

    return gyroCalibrated;
}

Vector IMU ::getGyroVector()
{
    getGyroCalibratedData();

    w_Vector.xAxis = (gyroCalibrated.xAxis * PI) / 180;
    w_Vector.yAxis = -(gyroCalibrated.yAxis * PI) / 180;
    w_Vector.zAxis = -(gyroCalibrated.zAxis * PI) / 180;

    return w_Vector;
}

Vector IMU ::getAccelRawData()
{
    myICM.getAGMT();
    accel.xAxis = myICM.agmt.acc.axes.x / 16384.0;
    accel.yAxis = myICM.agmt.acc.axes.y / 16384.0;
    accel.zAxis = myICM.agmt.acc.axes.z / 16384.0;

    return accel;
}

Vector IMU ::getAccelVector()
{
    getAccelRawData();

    g_Vector.xAxis = -(accel.xAxis * 9.80665f);
    g_Vector.yAxis = (accel.yAxis * 9.80665f);
    g_Vector.zAxis = (accel.zAxis * 9.80665f);

    return g_Vector;
}

ANGLE IMU ::calAttitude()
{
    getGyroVector();
    getAccelVector();
    getMagVector();

    Raw.phi = atan2(g_Vector.yAxis, g_Vector.zAxis);
    //Serial.println(Raw.phi *(180/PI),2);
    Raw.theta = atan2(-g_Vector.xAxis, sqrt(pow(g_Vector.yAxis, 2) + pow(g_Vector.zAxis, 2)));
    Raw.psi = atan2(b_Vector.zAxis * sin(Raw.phi) - b_Vector.yAxis * cos(Raw.phi),
                    b_Vector.xAxis * cos(Raw.theta) + b_Vector.yAxis * sin(Raw.theta) * sin(Raw.phi) +
                        b_Vector.zAxis * sin(Raw.theta) * cos(Raw.phi));

    // Calculating Change in Phi, Psi and Theta with Respect to Time.
    Dot.phi = w_Vector.xAxis + w_Vector.yAxis * sin(Raw.phi) * tan(Raw.theta) + w_Vector.zAxis * cos(Raw.phi) * tan(Raw.theta);
    Dot.theta = w_Vector.yAxis * cos(Raw.phi) - w_Vector.zAxis * sin(Raw.phi);
    Dot.psi = w_Vector.yAxis * (sin(Raw.phi) / cos(Raw.theta)) + w_Vector.zAxis * (cos(Raw.phi) / cos(Raw.theta));

    // Calculating Estimated Phi, Psi and Theta using Complementry Filter.
    initial_T = final_T;
    final_T = millis();
    delta_T = (final_T - initial_T) / 1000.0;

    Est.phi = ALPHA * (Est.phi + Dot.phi * delta_T) + (1 - ALPHA) * Raw.phi;
    Est.theta = ALPHA * (Est.theta + Dot.theta * delta_T) + (1 - ALPHA) * Raw.theta;
    if (Raw.psi > (170 * (PI / 180)) || Raw.psi < (-170 * (PI / 180)))
    {
        Est.psi = Raw.psi;
    }
    else
    {
        Est.psi = ALPHA * (Est.psi + Dot.psi * delta_T) + (1 - ALPHA) * Raw.psi;
    }

    return Est;
}

void IMU ::gyro_data_To_EEprom()
{
  getGyroOffset();
  eeprom_Adr = 0;
  EEPROM.put(eeprom_Adr, offGyro);
  eeprom_Adr += sizeof(offGyro);
}

void IMU ::mag_data_To_EEprom()
{
  getMagOffset();
  EEPROM.put(eeprom_Adr, offMag);
}

Vector IMU ::gyro_data_From_EEprom()
{
  eeprom_Adr = 0;
  EEPROM.get(eeprom_Adr, offGyro);
  eeprom_Adr += sizeof(offGyro);
  return offGyro;
}

Vector IMU ::mag_data_From_EEprom()
{
  EEPROM.get(eeprom_Adr, offMag);
  eeprom_Adr += sizeof(offMag);
  return offMag;
}
