/*
 * mainpp.cpp
 *
 *  Created on: Mar 13, 2024
 *      Author: Nghiem Long & Cong Anh
 */

#include <math.h>
#include <mainpp.h>
#include <ros.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "string.h"
#include "stdio.h"

#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int16MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>
#include "STM32Hardware.h"


/* Rosserial */

void cmd_vel_callback(const geometry_msgs::Twist& msg);
void cmd_PID_callback(const std_msgs::Float32MultiArray& msg);

ros::NodeHandle nh;

std_msgs::Float32MultiArray vel_set_msg;
std_msgs::Float32MultiArray vel_enc_msg;
std_msgs::Float32MultiArray PID_msg;
sensor_msgs::Imu imu_msg;

ros::Publisher vel_set("vel_set", &vel_set_msg);
ros::Publisher vel_enc("vel_enc", &vel_enc_msg);
//ros::Publisher PID("PID", &PID_msg);
ros::Publisher imu("imu/raw", &imu_msg);

ros::Subscriber<geometry_msgs::Twist> cmd_vel("cmd_vel",&cmd_vel_callback);
ros::Subscriber<std_msgs::Float32MultiArray> cmd_PID("cmd_PID",&cmd_PID_callback);

extern I2C_HandleTypeDef hi2c1;
extern float v_enc[4];
extern float v_set[4];
extern float Kp[4], Ki[4], Kd[4], data_PID[12];
extern float d, l;
long sequence = 0;
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	nh.getHardware()->reset_rbuf();
}

void setup(void) {
	nh.initNode();

	nh.advertise(vel_set);
	nh.advertise(vel_enc);
	//nh.advertise(PID);
	nh.advertise(imu);

	nh.subscribe(cmd_vel);
	nh.subscribe(cmd_PID);

}
void loop(void) {


	vel_set_msg.data_length = sizeof(v_set)/sizeof(v_set[0]);
	vel_set_msg.data = v_set;
	vel_set.publish(&vel_set_msg);
	HAL_Delay(30);

	vel_enc_msg.data_length = sizeof(v_enc)/sizeof(v_enc[0]);
	vel_enc_msg.data = v_enc;
	vel_enc.publish(&vel_enc_msg);
	HAL_Delay(30);

	/*PID_msg.data_length = sizeof(data_PID)/sizeof(data_PID[0]);
	PID_msg.data = data_PID;
	PID.publish(&PID_msg);
	HAL_Delay(40);*/
	  //Header
	MPU_readSensorData(&hi2c1);
	  imu_msg.header.stamp = nh.now();
	  imu_msg.header.frame_id  ="imu";

	  //Linear Acceleration
	  imu_msg.linear_acceleration.x = sensorData.ax ;
	  imu_msg.linear_acceleration.y = sensorData.ay ;
	  imu_msg.linear_acceleration.z = sensorData.az ;

	  //Angular Velocity
	  imu_msg.angular_velocity.x = sensorData.gx ;
	  imu_msg.angular_velocity.y = sensorData.gy ;
	  imu_msg.angular_velocity.z = sensorData.gz ;

	  //Orientation
	  imu_msg.orientation.x = 0 ;
	  imu_msg.orientation.y = 0 ;
	  imu_msg.orientation.z = 0 ;
	  imu_msg.orientation.w = 0 ;

	  imu.publish(&imu_msg);
	  HAL_Delay(40);
	nh.spinOnce();
}

void cmd_vel_callback(const geometry_msgs::Twist& msg){
	v_set[0] = 60*((-msg.linear.x+msg.linear.y)/sqrt(2)+msg.angular.z*d)/l;
	v_set[1] = 60*((msg.linear.x+msg.linear.y)/sqrt(2)+msg.angular.z*d)/l;
	v_set[2] = 60*((msg.linear.x-msg.linear.y)/sqrt(2)+msg.angular.z*d)/l;
	v_set[3] = 60*((-msg.linear.x-msg.linear.y)/sqrt(2)+msg.angular.z*d)/l;
}

void cmd_PID_callback(const std_msgs::Float32MultiArray& msg){   //{Kp1,Ki1,Kd1,Kp2,Ki2,Kd2,Kp3,Ki3,Kd3,Kp4,Ki4,Kd4}
	Kp[0] = msg.data[0]; Kp[1] = msg.data[3]; Kp[2] = msg.data[6]; Kp[3] = msg.data[9];
	Ki[0] = msg.data[1]; Ki[1] = msg.data[4]; Ki[2] = msg.data[7]; Ki[3] = msg.data[10];
	Kd[0] = msg.data[2]; Kd[1] = msg.data[5]; Kd[2] = msg.data[8]; Kd[3] = msg.data[11];

	Flash_Write_Array_Float(0x080A0000, Kp, 4);
	Flash_Write_Array_Float(0x080C0000, Ki, 4);
	Flash_Write_Array_Float(0x080E0000, Kd, 4);
}


/* Flash memory */


/* DEFINE the SECTORS according to your reference manual
 * STM32F407VG have:-
 *  Sector 0 to Sector 3 each 16KB
 *  Sector 4 as 64KB
 *  Sector 5 to Sector 11 each 128KB
 */

static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < 0x08003FFF) && (Address >= 0x08000000))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < 0x08007FFF) && (Address >= 0x08004000))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < 0x0800BFFF) && (Address >= 0x08008000))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < 0x0800FFFF) && (Address >= 0x0800C000))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < 0x0801FFFF) && (Address >= 0x08010000))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < 0x0803FFFF) && (Address >= 0x08020000))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < 0x0805FFFF) && (Address >= 0x08040000))
  {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < 0x0807FFFF) && (Address >= 0x08060000))
  {
    sector = FLASH_SECTOR_7;
  }
  else if((Address < 0x0809FFFF) && (Address >= 0x08080000))
  {
    sector = FLASH_SECTOR_8;
  }
  else if((Address < 0x080BFFFF) && (Address >= 0x080A0000))
  {
    sector = FLASH_SECTOR_9;
  }
  else if((Address < 0x080DFFFF) && (Address >= 0x080C0000))
  {
    sector = FLASH_SECTOR_10;
  }
  else if((Address < 0x080FFFFF) && (Address >= 0x080E0000))
  {
    sector = FLASH_SECTOR_11;
  }
  return sector;
}

void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}

uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError;
	int sofar=0;


	 /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Erase the user Flash area */

	  /* Get the number of sector to erase from 1st sector */

	  uint32_t StartSector = GetSector(StartSectorAddress);
	  uint32_t EndSectorAddress = StartSectorAddress + numberofwords*4;
	  uint32_t EndSector = GetSector(EndSectorAddress);

	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	  EraseInitStruct.Sector        = StartSector;
	  EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	  {
		  return HAL_FLASH_GetError ();
	  }

	  /* Program the user Flash area word by word
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartSectorAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

	   return 0;
}

void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{

		*RxBuf = *(__IO uint32_t *)StartSectorAddress;
		StartSectorAddress += 4;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}

void Convert_To_Str (uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char *)Data)/4) + ((strlen((char *)Data) % 4) != 0)) *4;

	for (int i=0; i<numberofbytes; i++)
	{
		Buf[i] = Data[i/4]>>(8*(i%4));
	}
}

void Flash_Write_NUM (uint32_t StartSectorAddress, float Num)
{
	uint8_t bytes_temp[4];
	float2Bytes(bytes_temp, Num);

	Flash_Write_Data (StartSectorAddress, (uint32_t *)bytes_temp, 1);
}

float Flash_Read_NUM (uint32_t StartSectorAddress)
{
	uint8_t buffer[4];
	float value;

	Flash_Read_Data(StartSectorAddress, (uint32_t *)buffer, 1);
	value = Bytes2float(buffer);
	return value;
}

void Flash_Write_Array_Float(uint32_t StartSectorAddress, float *Array, uint16_t ArrayLength)
{
    uint8_t bytes_temp[4];

    uint32_t i;
    uint32_t WordsToWrite = ArrayLength * sizeof(float) / sizeof(uint32_t);

    HAL_FLASH_Unlock();

    uint32_t StartSector = GetSector(StartSectorAddress);
    uint32_t EndSectorAddress = StartSectorAddress + WordsToWrite * sizeof(uint32_t);
    uint32_t EndSector = GetSector(EndSectorAddress);

    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = StartSector;
    EraseInitStruct.NbSectors = (EndSector - StartSector) + 1;

    uint32_t SECTORError;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        return;
    }

    for (i = 0; i < ArrayLength; i++)
    {
        float2Bytes(bytes_temp, Array[i]);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, *(uint32_t *)bytes_temp) != HAL_OK)
        {
            return;
        }
        StartSectorAddress += sizeof(uint32_t);
    }

    HAL_FLASH_Lock();
}

void Flash_Read_Array_Float(uint32_t StartSectorAddress, float *Array, uint16_t ArrayLength)
{
    uint32_t i;
    uint8_t bytes_temp[4];

    for (i = 0; i < ArrayLength; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bytes_temp[j] = *(__IO uint8_t *)(StartSectorAddress + j);
        }

        Array[i] = Bytes2float(bytes_temp);

        StartSectorAddress += 4;
    }
}

/* IMU */

uint8_t _addr, _maddr ,_name;
int AFSR = 4, GFSR = 500;
float _tau, _dt, RAD2DEG = 57.2977951308;

RawData rawData;
SensorData sensorData;
GyroCal gyroCal;
Attitude attitude;
ScaleFactor scaleFactor;

/// @brief Limit attitude value within (-180;180) degree.
/// @param angle Attitude value.
float limAngle(float angle)
{
	if (angle > 180)
	{
		angle -= 360;
	}
	else if (angle <= -180)
	{
		angle += 360;
	}

	return angle;
}

/// @brief Set the IMU address, check for connection, reset IMU, and set full range scale.
/// @param I2Cx Pointer to I2C structure config.
/// @param addr Hex address based on AD0 pin - 0x68 low or 0x69 high.
/// @param tau Set tau value for the complementary filter (typically 0.98).
/// @param dt Set sampling rate in seconds determined by the timer interrupt.
uint8_t MPU_begin(I2C_HandleTypeDef *I2Cx, uint8_t addr, float tau, float dt)
{
    // Save values
    _addr = addr << 1;
    _tau = tau;
    _dt = dt;

    // Initialize variables
    uint8_t check;
    uint8_t select;

    // Confirm device
    HAL_I2C_Mem_Read(I2Cx, _addr, WHO_AM_I, 1, &check, 1, I2C_TIMOUT_MS);

    // TODO: If 9250 or 6050 fails could it trigger the opposite check???
    if ((check == WHO_AM_I_9250_ANS) || (check == WHO_AM_I_6050_ANS))
    {
    	if (check == WHO_AM_I_9250_ANS)
    	{
    		_name = MPU9250;
    		_maddr = MAG_LOW << 1;
    	}
    	else
    	{
    		_name = MPU6050;
    	}
        // Startup / reset the sensor
        select = 0x00;
        HAL_I2C_Mem_Write(I2Cx, _addr, PWR_MGMT_1, 1, &select, 1, I2C_TIMOUT_MS);
        MPU_setFullScaleRange(I2Cx, AFSR_4G, GFSR_500DPS);

        return 1;
    }
    else
    {
        return 0;
    }
}

/// @brief Set the MPU full scale range.
/// @param I2Cx Pointer to I2C stucture config.
/// @param _addr Hex address based on AD0 pin.
/// @param scaleFactor Save scale range.
/// @param aScale Set accelerometer full scale range: 0 for ±2g, 1 for ±4g, 2 for ±8g, and 3 for ±16g.
/// @param gScale Set gyroscope full scale range: 0 for ±250°/s, 1 for ±500°/s, 2 for ±1000°/s, and 3 for ±2000°/s.
void MPU_setFullScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t aScale, uint8_t gScale)
{
	// Set full scale range
	MPU_writeAccScaleRange(I2Cx, aScale);
	MPU_writeGyrScaleRange(I2Cx, gScale);
}

/// @brief Set the accelerometer full scale range.
/// @param I2Cx Pointer to I2C structure config.
/// @param scaleFactor Save scale range.
/// @param aScale Set 0 for ±2g, 1 for ±4g, 2 for ±8g, and 3 for ±16g.
void MPU_writeAccScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t aScale)
{
    // Variable init
    uint8_t select;

    // Set the value
    switch (aScale)
    {
    case AFSR_2G:
    	AFSR = 2;
        scaleFactor.a = 16384.0;
        select = 0x00;
        HAL_I2C_Mem_Write(I2Cx, _addr, ACCEL_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case AFSR_4G:
    	AFSR = 4;
        scaleFactor.a = 8192.0;
        select = 0x08;
        HAL_I2C_Mem_Write(I2Cx, _addr, ACCEL_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case AFSR_8G:
    	AFSR = 8;
        scaleFactor.a = 4096.0;
        select = 0x10;
        HAL_I2C_Mem_Write(I2Cx, _addr, ACCEL_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case AFSR_16G:
    	AFSR = 16;
        scaleFactor.a = 2048.0;
        select = 0x18;
        HAL_I2C_Mem_Write(I2Cx, _addr, ACCEL_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    default:
    	AFSR = 4;
        scaleFactor.a = 8192.0;
        select = 0x08;
        HAL_I2C_Mem_Write(I2Cx, _addr, ACCEL_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    }
}

/// @brief Set the gyroscope full scale range.
/// @param I2Cx Pointer to I2C structure config.
/// @param scaleFactor Save scale range.
/// @param gScale Set 0 for ±250°/s, 1 for ±500°/s, 2 for ±1000°/s, and 3 for ±2000°/s.
void MPU_writeGyrScaleRange(I2C_HandleTypeDef *I2Cx, uint8_t gScale)
{
    // Variable init
    uint8_t select;

    // Set the value
    switch (gScale)
    {
    case GFSR_250DPS:
    	GFSR = 250;
        scaleFactor.g = 131.0;
        select = 0x00;
        HAL_I2C_Mem_Write(I2Cx, _addr, GYRO_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case GFSR_500DPS:
    	GFSR = 500;
        scaleFactor.g = 65.5;
        select = 0x08;
        HAL_I2C_Mem_Write(I2Cx, _addr, GYRO_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case GFSR_1000DPS:
    	GFSR = 1000;
        scaleFactor.g = 32.8;
        select = 0x10;
        HAL_I2C_Mem_Write(I2Cx, _addr, GYRO_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    case GFSR_2000DPS:
    	GFSR = 2000;
        scaleFactor.g = 16.4;
        select = 0x18;
        HAL_I2C_Mem_Write(I2Cx, _addr, GYRO_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    default:
    	GFSR = 500;
        scaleFactor.g = 65.5;
        select = 0x08;
        HAL_I2C_Mem_Write(I2Cx, _addr, GYRO_CONFIG, 1, &select, 1, I2C_TIMOUT_MS);
        break;
    }
}

/// @brief Read raw data from IMU.
/// @param I2Cx Pointer to I2C structure config.
void MPU_readRawData(I2C_HandleTypeDef *I2Cx)
{
    // Init buffer
    uint8_t buf[14], mbuf[6];

    // Subroutine for reading the raw data
    HAL_I2C_Mem_Read(I2Cx, _addr, ACCEL_XOUT_H, 1, buf, 14, I2C_TIMOUT_MS);

    // Bit shift the data
    rawData.ax = buf[0] << 8 | buf[1];
    rawData.ay = buf[2] << 8 | buf[3];
    rawData.az = buf[4] << 8 | buf[5];
    // temperature = buf[6] << 8 | buf[7];
    rawData.gx = buf[8] << 8 | buf[9];
    rawData.gy = buf[10] << 8 | buf[11];
    rawData.gz = buf[12] << 8 | buf[13];

    if (_name == MPU9250)
    {
        // Read the magnetometer
        HAL_I2C_Mem_Read(I2Cx, _maddr, MAG_XOUT_H, 1, mbuf, 6, I2C_TIMOUT_MS);

        // Bit shift the data
        rawData.mx = mbuf[0] << 8 | mbuf[1];
        rawData.my = mbuf[2] << 8 | mbuf[3];
        rawData.mz = mbuf[4] << 8 | mbuf[5];
    }
}

/// @brief Find offsets for each axis of gyroscope.
/// @param I2Cx Pointer to I2C structure config.
/// @param numCalPoints Number of points to get average offsets.
void MPU_calibrateGyro(I2C_HandleTypeDef *I2Cx, uint16_t numCalPoints)
{
    // Init
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    // Zero guard
    if (numCalPoints == 0)
    {
        numCalPoints = 1;
    }

    // Save specified number of points
    for (uint16_t ii = 0; ii < numCalPoints; ii++)
    {
        MPU_readRawData(I2Cx);
        x += rawData.gx;
        y += rawData.gy;
        z += rawData.gz;
        HAL_Delay(3);
    }

    // Average the saved data points to find the gyroscope offset
    gyroCal.x = (float)x / (float)numCalPoints;
    gyroCal.y = (float)y / (float)numCalPoints;
    gyroCal.z = (float)z / (float)numCalPoints;
}

/// @brief Calibrate the magnetometer
/// @param I2Cx Pointer to I2C structure config.
void MPU_calibrateMag(I2C_HandleTypeDef *I2Cx)
{

}

/// @brief Calculate the real world sensor values.
/// @param I2Cx Pointer to I2C structure config.
void MPU_readSensorData(I2C_HandleTypeDef *I2Cx)
{
    // Get raw values from the IMU
    MPU_readRawData(I2Cx);

    // Convert accelerometer values to g's
    sensorData.ax = (float)rawData.ax / scaleFactor.a;
    sensorData.ay = (float)rawData.ay / scaleFactor.a;
    sensorData.az = rawData.az / scaleFactor.a;

    // Compensate for gyro offset
    sensorData.gx = rawData.gx - gyroCal.x;
    sensorData.gy = rawData.gy - gyroCal.y;
    sensorData.gz = rawData.gz - gyroCal.z;

    // Convert gyro values to deg/s
    sensorData.gx /= scaleFactor.g;
    sensorData.gy /= scaleFactor.g;
    sensorData.gz /= scaleFactor.g;
}

/// @brief Calculate the attitude of the sensor in degrees using a complementary filter.
/// @param I2Cx Pointer to I2C structure config.
void MPU_calcAttitude(I2C_HandleTypeDef *I2Cx)
{
    // Read processed data
    MPU_readSensorData(I2Cx);

    // Complementary filter
    float accelPitch = atan2(sensorData.ay, sensorData.az) * RAD2DEG;
    float accelRoll = atan2(sensorData.ax, sensorData.az) * RAD2DEG;

    attitude.r = _tau * (attitude.r - sensorData.gy * _dt) + (1 - _tau) * accelRoll;
    attitude.p = _tau * (attitude.p + sensorData.gx * _dt) + (1 - _tau) * accelPitch;
    attitude.y += sensorData.gz * _dt;

    // Limit attitude value from -180 degree to 180 degree
    attitude.r = limAngle(attitude.r);
    attitude.p = limAngle(attitude.p);
    attitude.y = limAngle(attitude.y);
}
