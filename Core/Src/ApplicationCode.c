/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */

static bool firstPress = false;
extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#if TOUCH_INTERRUPT_ENABLED == 1
static EXTI_HandleTypeDef LCDTouchIRQ;
void LCDTouchScreenInterruptGPIOInit(void);
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	//initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#if TOUCH_INTERRUPT_ENABLED == 1
	LCDTouchScreenInterruptGPIOInit();
	#endif // TOUCH_INTERRUPT_ENABLED
	#endif // COMPILE_TOUCH_FUNCTIONS


}
void EXTI0_IRQHandler() // copied from lab5
{
    EXTI_HandleTypeDef localStruct = {0};
    localStruct.Line = EXTI_LINE_0;
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    addSchedulerEvent(ROTATE_EVENT);
    HAL_EXTI_ClearPending(&localStruct, EXTI_TRIGGER_RISING);
    HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

    // Re-enable the interrupt
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}



//TIMER CODE 	////////////////////////////////////////////////////////////////////////////////////
TIM_HandleTypeDef htim6;
void TIM6_Init()
{
	//dont need to enable clock????
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = PRESCALER;
    htim6.Init.Period = PERIOD;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; //default

    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)    // Initialize TIM6
    {
        while(1)
        {
        	//error handler
        }
    }

    if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK) //start the timer in interrupt mode (THIS IS CONFUSING)
    {
        while(1)
        {
        	//error handler
        }
    }
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);   //enable interrupts
}

void TIM6_DAC_IRQHandler()
{
    HAL_TIM_IRQHandler(&htim6);  //using hal tim irq handler
    addSchedulerEvent(GAME_TICK_EVENT); // gameplay event to move blocks down
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void LCD_Visual_Demo(void)
{
	visualDemo();
}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}


// TouchScreen Interrupt
#if TOUCH_INTERRUPT_ENABLED == 1

void LCDTouchScreenInterruptGPIOInit(void)
{
	GPIO_InitTypeDef LCDConfig = {0};
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init      
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	LCDTouchIRQ.Line = EXTI_LINE_15;

}

#define TOUCH_DETECTED_IRQ_STATUS_BIT   (1 << 0)  // Touchscreen detected bitmask

static uint8_t statusFlag;

void EXTI15_10_IRQHandler()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a universial interrupt guard
	bool isTouchDetected = false;

	//static uint32_t count;
//	count = 0;
//	while(count == 0){
//		count = STMPE811_Read(STMPE811_FIFO_SIZE);
//	}

	// Disable touch interrupt bit on the STMPE811
	uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
	WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

	// Clear the interrupt bit in the STMPE811
	statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
	uint8_t clearIRQData = (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);
	
	uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
	if (ctrlReg & 0x80)
	{
		isTouchDetected = true;

	}

	// Determine if it is pressed or unpressed
	if(isTouchDetected) // Touch has been detected
	{

		printf("\nPressed");
		// May need to do numerous retries? 
		DetermineTouchPosition(&StaticTouchData);
		if (StaticTouchData.x > 120 && firstPress == 1) // if touch is right half of screen
		{
			addSchedulerEvent(MOVE_RIGHT_EVENT);
		}
		if(StaticTouchData.x <= 120 && firstPress)
		{
			addSchedulerEvent(MOVE_LEFT_EVENT); //left touch detected
		}
		if(firstPress == 0)
		{
			if(StaticTouchData.x >50 && StaticTouchData.x < 190 && StaticTouchData.y < 210 && StaticTouchData.y > 70 )
			{
				addSchedulerEvent(START_EVENT);
				firstPress = true;
			}
		}
		/* Touch valid */
		//printf("\nX: %03d\nY: %03d \n", StaticTouchData.x, StaticTouchData.y);
		//LCD_Clear(0, LCD_COLOR_RED);

	}else{

		/* Touch not pressed */
		//printf("\nNot pressed \n");
		//LCD_Clear(0, LCD_COLOR_GREEN);
	}

	STMPE811_Write(STMPE811_FIFO_STA, 0x01);
	STMPE811_Write(STMPE811_FIFO_STA, 0x00);

	// Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
	HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered DURING the handling of this IRQ..
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

}
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS



