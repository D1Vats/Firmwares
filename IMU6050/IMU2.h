#ifndef IMU2_H_INCLUDED
#define IMU2_H_INCLUDED

#define PI 3.1416
#define ALPHA 0.96

// Declaring Registers for MPU6050
#define MPU_ADR                (0x68)
#define GYRO_CONFIG            (0x1B)
#define ACCEL_CONFIG           (0x1C)
#define INT_PIN_CFG            (0x37)
#define ACCEL_XOUT_H           (0x3B)
#define TEMP_OUT_H             (0x41)
#define GYRO_XOUT_H            (0x43)
#define USER_CTRL              (0x6A)
#define PWR_MNGT_1             (0x6B)

// Declaring register for HMC5883L
#define HMC_ADR                (0x1E)
#define CONFIG_REG_A           (0x00)
#define CONFIG_REG_B           (0x01)
#define MODE_REG               (0x02)
#define MAG_XOUT_H             (0x03)

// Bit Assignment of Register
#define GYRO_CONFIG_BIT        (0x18)  // 2000 dps
#define ACCEL_CONFIG_BIT       (0x18) // 16g
#define INT_PIN_CFG_BIT        (0x02)  // I2C bypass enable bit
#define USER_CTRL_BIT          (0x00)
#define PWR_MNGT_1_BIT         (0x00)
#define CONFIG_REG_A_BIT       (0x74) // 8 samples averaged per measurment output // Data rate 30Hz
#define CONFIG_REG_B_BIT       (0x20) // +/- 1.3Ga sensor field range
#define MODE_REG_BIT           (0x00)     // Continuous Mode

// struct VEC3
// {
//     double xAxis;
//     double yAxis;
//     double zAxis;
// };
// VEC3 gyro, accel, mag, offGyro, offMag, magMin, magMax, normAccel;
// VEC3 gVector, wVector, BVector;

// struct Angle
// {
//     double phi;
//     double theta;
//     double psi;
// };
// Angle est, dot, raw;

// long final_T;
// long initial_T;
// double delta_T;

void setupMPU();
void setupHMC();
VEC3 offsetGyroData();
VEC3 offAccelData();
VEC3 offsetMagData();
VEC3 setGyroOffset();
VEC3 gyroData();
VEC3 setMagOffset();
VEC3 magData();
VEC3 accelData();
VEC3 gyroVector();
VEC3 accelVector();
VEC3 magVector();
Angle calAttitude();

#endif