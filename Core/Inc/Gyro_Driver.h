/*
 * Gyro_Driver.h
 *
 *  Created on: Oct 31, 2024
 *      Author: connorsorrell
 */

#ifndef INC_GYRO_DRIVER_H_
#define INC_GYRO_DRIVER_H_

//macros for Gyro (ADDR of needed registers
//Macros for pin and port info for Gyro

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

#define WHO_AMI 0x0F
#define CR1 0x20
#define CR2 0x21
#define CR3 0x22
#define CR4 0x23
#define CR5 0x24
#define REF 0x25
#define OUT_TEMP 0x26
#define SR 0x27

#define READ (1 << 7)
#define WRITE (0 << 7)

#define GYRO_CS_PORT GPIOC // idk if these work
#define GYRO_CS_PIN GPIO_PIN_1

#define FIFO_CR 0x2E
#define FIFO_SRC 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31

#define INT1_DURATION 0x38

#define A5_PIN GPIO_PIN_5
#define GYRO_PORT GPIOF
#define GYRO_SCK_PIN GPIO_PIN_7
#define GYRO_MISO_PIN GPIO_PIN_8
#define GYRO_MOSI_PIN GPIO_PIN_9

void Gyro_Init();

void Gyro_Get_Device_ID();

void Gyro_Power_On();

void Gyro_Get_Temp();

void Gyro_Config_Reg();

void Gyro_Write_Reg(uint16_t command);


void Gyro_Read_Reg(uint16_t command, uint16_t registerData);


void Gyro_Verify_HAL();

void Gyro_Enable_Slave();

void Gyro_Disable_Slave();

void Gyro_Reboot();

#endif /* INC_GYRO_DRIVER_H_ */
