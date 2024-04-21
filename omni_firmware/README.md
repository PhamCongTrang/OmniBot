# Motor_Controller_STM32F4
Controlling Motor Speed with STM32F407 Kit
![Untitled](https://github.com/WanL0q/Motor_Controller_STM32F4/assets/134664967/8cce08ca-791c-4710-b91d-051e33a09514)
## Forward Kinematics
$$
\begin{bmatrix}
v_x \\
v_y \\
\omega \\
\end{bmatrix}
\=\
\begin{bmatrix}
1/2\sqrt{2} & 1/2\sqrt{2} & -1/2\sqrt{2} & -1/2\sqrt{2} \\
1/2\sqrt{2} & -1/2\sqrt{2} & -1/2\sqrt{2} & 1/2\sqrt{2} \\
-1/4d & -1/4d & -1/4d & -1/4d \\
\end{bmatrix}
\begin{bmatrix}
v_1 \\
v_2 \\
v_3 \\
v_4 \\
\end{bmatrix}
$$
## Inverse Kinematics
$$
\begin{bmatrix}
v_1 \\
v_2 \\
v_3 \\
v_4 \\
\end{bmatrix}
\=\
\begin{bmatrix}
1/\sqrt{2} & 1/\sqrt{2} & -d \\
1/\sqrt{2} & -1/\sqrt{2} & -d \\
-1/\sqrt{2} & -1/\sqrt{2} & -d \\
-1/\sqrt{2} & 1/\sqrt{2} & -d \\
\end{bmatrix}
\begin{bmatrix}
v_x \\
v_y \\
\omega \\
\end{bmatrix}
$$
## PID Controller
![image](https://github.com/WanL0q/Motor_Controller_STM32F4/assets/134664967/dc0a0fbd-e9ea-480a-b046-0eff0817b364)

![Screenshot 2024-03-23 121907](https://github.com/WanL0q/Motor_Controller_STM32F4/assets/134664967/96d39baf-8c5f-40b6-9763-4bf98f72518c)



# Configure Pinout for STM32F407

![3](https://github.com/WanL0q/Motor_Controller_STM32F4/assets/134664967/fe212cb8-827a-4e58-b334-2dcba47d07bd)

| Wheel    | Pin       | Function        | Wheel | Pin       | Function        |
|----------|-----------|-----------------|-------|-----------|-----------------|
| **1**    | PE9       | R_EN=L_EN= PWM  |**2** | PE11      | R_EN=L_EN=PWM   |
|          | PC5       | RPWM            |       | PB1       | RPWM            |
|          | PB0       | LPWM            |       | PB2       | LPWM            |
|          | PA5       | Encoder CH_A    |       | PA6       | Encoder CH_A    |
|          | PB3       | Encoder CH_B    |       | PA7       | Encoder CH_B    |
|----------|-----------|-----------------|-------|-----------|-----------------|
| **3**    | PE13      | R_EN=L_EN=PWM   | **4** | PE14      | R_EN=L_EN=PWM   |
|          | PB12      | RPWM            |       | PB14      | RPWM            |
|          | PB13      | LPWM            |       | PB15      | LPWM            |
|          | PD12      | Encoder CH_A    |       | PA0       | Encoder CH_A    |
|          | PB7       | Encoder CH_B    |       | PA1       | Encoder CH_B    |
|----------|-----------|-----------------|-------|-----------|-----------------|
| **UART** | PA2       | USART2_TX       |       | PA3       | USART2_RX       |

# Communication between ROS and STM32
- Topic "cmd_vel": Longitudinal velocity and angular velocity set for the robot.
- Topic "vel_set": Velocity is set for 4 motors.
- Topic "vel_enc": Velocity of the 4 motors calculated from encoders.
