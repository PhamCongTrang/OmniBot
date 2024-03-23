################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/ROS/duration.cpp \
../Core/Inc/ROS/time.cpp 

OBJS += \
./Core/Inc/ROS/duration.o \
./Core/Inc/ROS/time.o 

CPP_DEPS += \
./Core/Inc/ROS/duration.d \
./Core/Inc/ROS/time.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/ROS/%.o Core/Inc/ROS/%.su Core/Inc/ROS/%.cyclo: ../Core/Inc/ROS/%.cpp Core/Inc/ROS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/longn/STM32CubeIDE/workspace_1.13.2/chatter_F407VG/Core/Inc/ROS" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-ROS

clean-Core-2f-Inc-2f-ROS:
	-$(RM) ./Core/Inc/ROS/duration.cyclo ./Core/Inc/ROS/duration.d ./Core/Inc/ROS/duration.o ./Core/Inc/ROS/duration.su ./Core/Inc/ROS/time.cyclo ./Core/Inc/ROS/time.d ./Core/Inc/ROS/time.o ./Core/Inc/ROS/time.su

.PHONY: clean-Core-2f-Inc-2f-ROS

