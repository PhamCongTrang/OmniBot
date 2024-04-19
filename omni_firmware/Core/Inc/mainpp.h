/*
 * mainpp.h
 *
 *  Created on: Mar 13, 2024
 *      Author: longn
 */

#ifndef INC_MAINPP_H_
#define INC_MAINPP_H_

#ifdef __cplusplus
 extern "C"{
#endif
#include <stdint.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"

 // Defines
 #define WHO_AM_I_6050_ANS 0x68
 #define WHO_AM_I_9250_ANS 0x71
 #define WHO_AM_I          0x75
 #define AD0_LOW           0x68
 #define AD0_HIGH          0x69
 #define MAG_LOW			  0x0C
 #define MAG_HIGH		  0x0D
 #define GYRO_CONFIG       0x1B
 #define ACCEL_CONFIG      0x1C
 #define PWR_MGMT_1        0x6B
 #define ACCEL_XOUT_H      0x3B
 #define ACCEL_YOUT_H	  0x3D
 #define ACCEL_ZOUT_H	  0x3F
 #define GYRO_XOUT_H		  0x43
 #define GYRO_YOUT_H		  0x45
 #define GYRO_ZOUT_H		  0x47
 #define MAG_XOUT_H		  0x03
 #define MAG_YOUT_H		  0x05
 #define MAG_ZOUT_H		  0x07
 #define I2C_TIMOUT_MS     1000

// IMU names
enum imuName
{
	MPU6050,
	MPU9250,
};

// Full scale ranges
enum gyroscopeFullScaleRange
{
    GFSR_250DPS,
    GFSR_500DPS,
    GFSR_1000DPS,
    GFSR_2000DPS
};

enum accelerometerFullScaleRange
{
    AFSR_2G,
    AFSR_4G,
    AFSR_8G,
    AFSR_16G
};

// Raw data
typedef struct {
	int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
} RawData;

// Processed data
typedef struct {
	float ax, ay, az, gx, gy, gz, mx, my, mz;
} SensorData;

// Calibrated data
typedef struct {
	float x, y, z;
} GyroCal;

// Euler angles
typedef struct {
	float r, p, y;
} Attitude;

// Scale range
typedef struct {
	float a, g;
} ScaleFactor;

extern uint8_t _addr, _maddr, _name;
extern int AFSR, GFSR;
extern float _tau, _dt, RAD2DEG;

extern RawData rawData;
extern SensorData sensorData;
extern GyroCal gyroCal;
extern Attitude attitude;
extern ScaleFactor scaleFactor;

// Functions IMU
float limAngle(float angle);
uint8_t MPU_begin(I2C_HandleTypeDef *I2Cx, uint8_t addr, float tau, float dt);
void MPU_setFullScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t aScale, uint8_t gScale);
void MPU_writeAccScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t aScale);
void MPU_writeGyrScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t gScale);
void MPU_readRawData(I2C_HandleTypeDef *I2Cx);
void MPU_calibrateGyro(I2C_HandleTypeDef *I2Cx, uint16_t numCalPoints);
void MPU_readSensorData(I2C_HandleTypeDef *I2Cx);
void MPU_calcAttitude(I2C_HandleTypeDef *I2Cx);

// Functions Rosserial
void setup();
void loop();

// Functions Flash Memory
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);
void Convert_To_Str (uint32_t *Data, char *Buf);
void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);
float Flash_Read_NUM (uint32_t StartSectorAddress);
void Flash_Write_Array_Float (uint32_t StartSectorAddress, float *arr, uint16_t len);
void Flash_Read_Array_Float(uint32_t StartSectorAddress, float *Array, uint16_t ArrayLength);

#ifdef __cplusplus
 }
#endif

#endif /* INC_MAINPP_H_ */
