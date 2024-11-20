/*
 * Gyro_Driver.c
 *
 *  Created on: Nov 5, 2024
 *      Author: connorsorrell
 */

#include "Gyro_Driver.h"
#include "ErrorHandling.h"

uint32_t DELAY = 20000;
static HAL_StatusTypeDef HALStatus;
SPI_HandleTypeDef SPI5Config;




void Gyro_Init()
{
	GPIO_InitTypeDef SPIPinConfig;
	__HAL_RCC_GPIOF_CLK_ENABLE();
	SPIPinConfig.Pin = GYRO_MISO_PIN | GYRO_MOSI_PIN | GYRO_SCK_PIN; // this might be wrong
	SPIPinConfig.Mode = GPIO_MODE_AF_PP;
	SPIPinConfig.Alternate = GPIO_AF5_SPI5;
	SPIPinConfig.Pull = GPIO_NOPULL;
	SPIPinConfig.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &SPIPinConfig);

	//enable chip select for SPI communication
	__HAL_RCC_GPIOC_CLK_ENABLE(); //CHIP SELECT
	SPIPinConfig.Pin = GYRO_CS_PIN; // this might be wrong
	SPIPinConfig.Mode = GPIO_MODE_OUTPUT_OD;
	SPIPinConfig.Pull = GPIO_PULLUP;
	SPIPinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GYRO_CS_PORT, &SPIPinConfig); // GPIOC = GYRO_CS_PORT
	Gyro_Disable_Slave();


	SPI5Config.Instance = SPI5;
	SPI5Config.Init.Mode = SPI_MODE_MASTER;
	SPI5Config.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI5Config.Init.Direction = SPI_DIRECTION_2LINES;
	SPI5Config.Init.CLKPhase = SPI_PHASE_2EDGE;
	SPI5Config.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SPI5Config.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	SPI5Config.Init.NSS = SPI_NSS_SOFT;
	SPI5Config.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPI5Config.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // these are unneeded i think?
	SPI5Config.Init.CRCPolynomial = 0;
	SPI5Config.Init.TIMode = SPI_TIMODE_DISABLE;
	HAL_SPI_Init(&SPI5Config);

	Gyro_Verify_HAL();


}

void Gyro_Get_Device_ID()
{
	uint16_t whoAmI = (READ | WHO_AMI);
	uint16_t Response = 0;
	//set CS to LOW, then send command over SPI and store the register data in Response
	Gyro_Enable_Slave();
	HALStatus = HAL_SPI_TransmitReceive(&SPI5Config, (uint8_t*) &whoAmI, (uint8_t*) &Response, 2, DELAY);
	Gyro_Verify_HAL();
	Gyro_Disable_Slave();
	uint8_t deviceID = (Response >> 8);
	printf("Device ID is: 0x%X \n \r", deviceID);
}

void Gyro_Power_On()
{
	uint16_t onCommand = (READ | CR1);
	uint16_t Buffer;
	Gyro_Enable_Slave();
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) != GPIO_PIN_RESET); // maybe dont need
	HALStatus = HAL_SPI_TransmitReceive(&SPI5Config, (uint8_t*) &onCommand, (uint8_t*) &Buffer, 2, DELAY);
	Gyro_Verify_HAL();
	Gyro_Disable_Slave();
	//Buffer |= (1 << 3);
	//Buffer = ((Buffer & 0xFF00) >> 8);
	//onCommand = ((0 << 7) | CR5) | Buffer;
	uint16_t power_on_cmd;
	power_on_cmd = (Buffer & (0xFF00 | 0x08)) | (0 << 7 | CR1);
	//(isolate the 8 upper bits of Buffer, then set bit 3 to 1 (zeros out all bits except upper 8 and 3rd bit)
	//sets the 7th bit to 0, then OR with CR1 address
	Gyro_Enable_Slave();
	HAL_SPI_Transmit(&SPI5Config, (uint8_t*) &power_on_cmd, 2, DELAY);
	Gyro_Disable_Slave();

}

void Gyro_Get_Temp()
{
	uint16_t out_temp = (READ | OUT_TEMP);
	uint16_t Response = 0;
	Gyro_Enable_Slave();
	HALStatus = HAL_SPI_TransmitReceive(&SPI5Config, (uint8_t*) &out_temp, (uint8_t*) &Response, 2, DELAY);
	Gyro_Verify_HAL();
	Gyro_Disable_Slave();
	uint8_t deviceTemp = ((Response & 0xFF00) >> 8); // isolate the upper 8 bytes (makes lower 8 all 0)
	printf("Device temperature is: %u \n \r", deviceTemp);
}

void Gyro_Config_Reg() //THESE COULD BE WRONG, CHECK THURSDAY
{
	Gyro_Write_Reg((WRITE | CR1) | (0x1F << 8));
	;

	Gyro_Write_Reg(WRITE | CR4 | (0x10 << 8));


	Gyro_Write_Reg(WRITE | CR5 | (0xC0 << 8));


	Gyro_Write_Reg(WRITE | FIFO_CR | (0x20 << 8));



}

void Gyro_Write_Reg(uint16_t command)
{
	Gyro_Enable_Slave();
	HALStatus = HAL_SPI_Transmit(&SPI5Config, (uint8_t*) &command, 2, DELAY);
	Gyro_Disable_Slave();
}

void Gyro_Read_Reg(uint16_t command, uint16_t registerData)
{
	Gyro_Enable_Slave();
	HALStatus = HAL_SPI_TransmitReceive(&SPI5Config, (uint8_t*) &command, (uint8_t*) &registerData, 2, DELAY);
	Gyro_Verify_HAL();
	Gyro_Disable_Slave();
}

//void Gyro_Read_Reg(uint32_t ADDR)
//{
//	uint16_t command = (1 << 7) | ADDR; // read (set 7th bit)
//	uint16_t registerData;
//
//	Gyro_Enable_Slave();
//	HALStatus = HAL_SPI_TransmitReceive(&SPI5Config, (uint8_t*) &command, (uint8_t*) &registerData, 2, DELAY);
//	Gyro_Verify_HAL();
//	Gyro_Disable_Slave();
//
//	return (registerData & 0xFF);
//
//}

void Gyro_Verify_HAL()
{

	APPLICATION_ASSERT(HALStatus == HAL_OK);
}

void Gyro_Enable_Slave()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
}

void Gyro_Disable_Slave()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
}


