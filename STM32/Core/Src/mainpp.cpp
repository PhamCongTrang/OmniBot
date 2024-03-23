/*
 * mainpp.cpp
 *
 *  Created on: Mar 13, 2024
 *      Author: longn
 */

#include <math.h>
#include <mainpp.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include "STM32Hardware.h"

void cmd_vel_callback(const std_msgs::Float32MultiArray& msg);

ros::NodeHandle nh;

std_msgs::Float32MultiArray vel_set_msg;
std_msgs::Float32MultiArray vel_enc_msg;
std_msgs::Float32MultiArray cmd_vel_msg;

ros::Publisher vel_set("vel_set", &vel_set_msg);
ros::Publisher vel_enc("vel_enc", &vel_enc_msg);

ros::Subscriber<std_msgs::Float32MultiArray> cmd_vel("cmd_vel",&cmd_vel_callback);

extern float v_enc[4];
extern float v_set[4];
extern float d, l;
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

	nh.subscribe(cmd_vel);
}

void loop(void) {

	vel_set_msg.data_length = sizeof(v_set)/sizeof(v_set[0]);
	vel_set_msg.data = v_set;

	vel_enc_msg.data_length = sizeof(v_enc)/sizeof(v_enc[0]);
	vel_enc_msg.data = v_enc;

	vel_set.publish(&vel_set_msg);
	vel_enc.publish(&vel_enc_msg);

	nh.spinOnce();
	HAL_Delay(1000);
}

void cmd_vel_callback(const std_msgs::Float32MultiArray& msg){
	if (msg.data_length >= 3){
		v_set[0] = 60*((msg.data[0]+msg.data[1])/sqrt(2)-msg.data[2]*d)/l;
		v_set[1] = 60*((msg.data[0]-msg.data[1])/sqrt(2)-msg.data[2]*d)/l;
		v_set[2] = 60*((-msg.data[0]-msg.data[1])/sqrt(2)-msg.data[2]*d)/l;
		v_set[3] = 60*((-msg.data[0]+msg.data[1])/sqrt(2)-msg.data[2]*d)/l;
	}
	/*if ((int) msg.data[0] % 5 == 0)		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);*/
}
