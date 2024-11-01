#include "Arduino.h"
#include <Wire.h>
#include "IMU2.h"
// #define PI 3.1416
// #define ALPHA 0.96

// // Declaring Registers for MPU6050
// #define MPU_ADR                (0x68)
// #define GYRO_CONFIG            (0x1B)
// #define ACCEL_CONFIG           (0x1C)
// #define INT_PIN_CFG            (0x37)
// #define ACCEL_XOUT_H           (0x3B)
// #define TEMP_OUT_H             (0x41)
// #define GYRO_XOUT_H            (0x43)
// #define USER_CTRL              (0x6A)
// #define PWR_MNGT_1             (0x6B)

// // Declaring register for HMC5883L
// #define HMC_ADR                (0x1E)
// #define CONFIG_REG_A           (0x00)
// #define CONFIG_REG_B           (0x01)
// #define MODE_REG               (0x02)
// #define MAG_XOUT_H             (0x03)

// // Bit Assignment of Register
// #define GYRO_CONFIG_BIT        (0x18)  // 2000 dps
// #define ACCEL_CONFIG_BIT       (0x18) // 16g
// #define INT_PIN_CFG_BIT        (0x02)  // I2C bypass enable bit
// #define USER_CTRL_BIT          (0x00)
// #define PWR_MNGT_1_BIT         (0x00)
// #define CONFIG_REG_A_BIT       (0x74) // 8 samples averaged per measurment output // Data rate 30Hz
// #define CONFIG_REG_B_BIT       (0x20) // +/- 1.3Ga sensor field range
// #define MODE_REG_BIT           (0x00)     // Continuous Mode

struct VEC3
{
    double xAxis;
    double yAxis;
    double zAxis;
};
VEC3 gyro, accel, mag, offGyro, offMag, magMin, magMax, normAccel;
VEC3 gVector, wVector, BVector;

struct Angle
{
    double phi;
    double theta;
    double psi;
};
Angle est, dot, raw;

long final_T;
long initial_T;
double delta_T;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    setupMPU();
    setupHMC();
    Serial.println("GyroScope Calibrating.....");
    setGyroOffset();
    delay(5000);
    Serial.println("Start Magnetometer Calibration... Rotate your Device Like Infinty");
    setMagOffset();
    Serial.println("Calibration Done...");
    delay(2000);

    final_T = millis();
}

void loop()
{
    calAttitude();

    Serial.print("Roll:- ");
    Serial.print(est.phi * (180 / PI), 4);
    Serial.print("\t");
    Serial.print("Pitch:- ");
    Serial.print(est.theta * (180 / PI), 4);
    Serial.print("\t");
    Serial.print("Yaw:- ");
    Serial.println(est.psi * (180 / PI), 4);
}

// Setting up MPU6050 for Data Transfer
void setupMPU()
{
    // Setting up PWR_MNGT Register
    Wire.beginTransmission(MPU_ADR);
    Wire.write(PWR_MNGT_1);
    Wire.write(PWR_MNGT_1_BIT);
    Wire.endTransmission();

    // Setting up GYRO_CONFIG Register
    Wire.beginTransmission(MPU_ADR);
    Wire.write(GYRO_CONFIG);
    Wire.write(GYRO_CONFIG_BIT);
    Wire.endTransmission();

    // Setting up ACCEL_CONFIG Register
    Wire.beginTransmission(MPU_ADR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(ACCEL_CONFIG_BIT);
    Wire.endTransmission();

    // Setting up I2C Bypass Enable Bit
    Wire.beginTransmission(MPU_ADR);
    Wire.write(INT_PIN_CFG);
    Wire.write(INT_PIN_CFG_BIT);
    Wire.endTransmission();

    // To Enable I2C Bypass I2C_MST_EN Bit in USER_CTRL must be 0
    Wire.beginTransmission(MPU_ADR);
    Wire.write(USER_CTRL);
    Wire.write(USER_CTRL_BIT);
    Wire.endTransmission();
}

// Setting up HMC5833L for Data Transfer
void setupHMC()
{
    // Setting up CONFIG_REG_A
    Wire.beginTransmission(HMC_ADR);
    Wire.write(CONFIG_REG_A);
    Wire.write(CONFIG_REG_A_BIT);
    Wire.endTransmission();

    // Setting up CONFIG_REG_B
    Wire.beginTransmission(HMC_ADR);
    Wire.write(CONFIG_REG_B);
    Wire.write(CONFIG_REG_B_BIT);
    Wire.endTransmission();

    // Setting up MODE_REG
    Wire.beginTransmission(HMC_ADR);
    Wire.write(MODE_REG);
    Wire.write(MODE_REG_BIT);
    Wire.endTransmission();
}

VEC3 offsetGyroData()
{
    // Reading Gyroscope Data
    Wire.beginTransmission(MPU_ADR);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADR, 6);
    while (Wire.available() < 6)
        ;
    gyro.xAxis = (Wire.read() << 8 | Wire.read()) / 16.4;
    gyro.yAxis = (Wire.read() << 8 | Wire.read()) / 16.4;
    gyro.zAxis = (Wire.read() << 8 | Wire.read()) / 16.4;
    return gyro;
}

VEC3 offAccelData()
{
    // Reading Accelerometer Data
    Wire.beginTransmission(MPU_ADR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADR, 6);
    while (Wire.available() < 6)
        ;
    accel.xAxis = (Wire.read() << 8 | Wire.read()) / 2048.00;
    accel.yAxis = (Wire.read() << 8 | Wire.read()) / 2048.00;
    accel.zAxis = (Wire.read() << 8 | Wire.read()) / 2048.00;
    return accel;
}

VEC3 offsetMagData()
{
    // Reading Magnetometer Data
    Wire.beginTransmission(HMC_ADR);
    Wire.write(MAG_XOUT_H);
    Wire.endTransmission();
   float Wire.requestFrom(HMC_ADR, 6);
    while (Wire.available() < 6)
        ;
    mag.xAxis = Wire.read() << 8 | Wire.read();
    mag.zAxis = Wire.read() << 8 | Wire.read();
    mag.yAxis = Wire.read() << 8 | Wire.read();
    return mag;
}

// Calculating Offset of Gyro Assuming at rest all the Three axes will show zero
VEC3 setGyroOffset()
{
    int count = 0;
    while (count < 500)
    {
        offsetGyroData();
        offGyro.xAxis = offGyro.xAxis + gyro.xAxis;
        offGyro.yAxis = offGyro.yAxis + gyro.yAxis;
        offGyro.zAxis = offGyro.zAxis + gyro.zAxis;
        count++;
    }
    offGyro.xAxis = offGyro.xAxis / 500;
    offGyro.yAxis = offGyro.yAxis / 500;
    offGyro.zAxis = offGyro.zAxis / 500;
    return offGyro;
}

// Getting Accurate Gyro Data
VEC3 gyroData()
{
    offsetGyroData();
    gyro.xAxis = gyro.xAxis - offGyro.xAxis;
    gyro.yAxis = gyro.yAxis - offGyro.yAxis;
    gyro.zAxis = gyro.zAxis - offGyro.zAxis;
    return gyro;
}

// Removing The Bias from Magnetometer
VEC3 setMagOffset()
{
     timer = 0;
    while (timer < 15)
    {
        offsetMagData();
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

// Getting Nearly Accurate Earth Magnetic Field Value.
VEC3 magData()
{
    offsetMagData();
    mag.xAxis = mag.xAxis - offMag.xAxis;
    mag.yAxis = mag.yAxis - offMag.yAxis;
    mag.zAxis = mag.zAxis - offMag.zAxis;

    // Converting Raw Data to MicroTesla
    mag.xAxis = (mag.xAxis * 100) / 1090;
    mag.yAxis = (mag.yAxis * 100) / 1090;
    mag.zAxis = (mag.zAxis * 100) / 1090;
    return mag;
}

// Assigning Value of 'g'.
VEC3 accelData()
{
    offAccelData();
    normAccel.xAxis = accel.xAxis * 9.80665f;
    normAccel.yAxis = accel.yAxis * 9.80665f;
    normAccel.zAxis = accel.zAxis * 9.80665f;
    return normAccel;
}

// Changing Frame of Reference and Direction
// For GyroScope
VEC3 gyroVector()
{
    gyroData();
    wVector.xAxis = (gyro.yAxis * PI) / 180;
    wVector.yAxis = (gyro.xAxis * PI) / 180;
    wVector.zAxis = -(gyro.zAxis * PI) / 180;
    return wVector;
}
// For Accelerometer
VEC3 accelVector()
{
    accelData();
    gVector.xAxis = -normAccel.yAxis;
    gVector.yAxis = -normAccel.xAxis;
    gVector.zAxis = normAccel.zAxis;
    return gVector;
}
// For Magnetometer
VEC3 magVector()
{
    magData();
    BVector.xAxis = mag.xAxis;
    BVector.yAxis = mag.yAxis;
    BVector.zAxis = mag.zAxis;
    return BVector;
}

// Calculating Yaw , Pitch and Roll angle using Complimentry Filter.
Angle calAttitude()
{
    gyroVector();
    accelVector();
    magVector();

    // Calcualting Phi and Theta from Accelerometer and Psi from Magnetometer Data
    raw.phi = atan2(gVector.yAxis, gVector.zAxis);
    raw.theta = atan2(-gVector.xAxis, sqrt(pow(gVector.yAxis, 2) + pow(gVector.zAxis, 2)));
    raw.psi = atan2(BVector.zAxis * sin(raw.phi) - BVector.yAxis * cos(raw.phi),
                    BVector.xAxis * cos(raw.theta) + BVector.yAxis * sin(raw.theta) * sin(raw.phi) +
                        BVector.zAxis * sin(raw.theta) * cos(raw.phi));

    // Calculating Change in Phi, Psi and Theta with Respect to Time.
    dot.phi = wVector.xAxis + wVector.yAxis * sin(raw.phi) * tan(raw.theta) + wVector.zAxis * cos(raw.phi) * tan(raw.theta);
    dot.theta = wVector.yAxis * cos(raw.phi) - wVector.zAxis * sin(raw.phi);
    dot.psi = wVector.yAxis * (sin(raw.phi) / cos(raw.theta)) + wVector.zAxis * (cos(raw.phi) / cos(raw.theta));

    // Calculating Estimated Phi, Psi and Theta using Complementry Filter.
    initial_T = final_T;
    final_T = millis();
    delta_T = (final_T - initial_T) / 1000.00;

    est.phi = ALPHA * (est.phi + dot.phi * delta_T) + (1 - ALPHA) * raw.phi;
    est.theta = ALPHA * (est.theta + dot.theta * delta_T) + (1 - ALPHA) * raw.theta;
    est.psi = ALPHA * (est.psi + dot.psi * delta_T) + (1 - ALPHA) * raw.psi;

    return est;
}
