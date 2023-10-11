#include "detecting.h"
#include "gpio.h"
#include "oled.h"
#include <stdio.h>

extern int flow_num;
extern int flow_limit;
extern int time_limit;
extern uint8_t detection;
extern uint8_t time_warning;
extern uint8_t timer;
extern TIM_HandleTypeDef htim2;

//start timer
static void start_timer(void)
{
	HAL_TIM_Base_Init(&htim2);
	__HAL_TIM_SET_AUTORELOAD(&htim2, time_limit*2000-1);
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim2);
}

//stop timer
static void stop_timer(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
}

//add flow number detection
static void add_detection(void)
{
	detection = 0;
	start_timer();
	while (detection == 0)
	{
		if (time_warning == 1)
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
		}
	}
	stop_timer();
	time_warning = 0;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	if (detection == 2)
		flow_num++;
	detection = 0;
	main_show();
}

//reduce flow number detection
static void reduce_detection(void)
{
	detection = 0;
	start_timer();
	while (detection == 0)
	{
		if (time_warning == 1)
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
		}
	}
	stop_timer();
	time_warning = 0;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	if (detection == 1)
		flow_num--;
	detection = 0;
	main_show();
}

//check whether trigger detection
void check_detection(void)
{
	if (detection == 1)
	{
		HAL_Delay(10);//eliminate the interference of parasitic capacitance of detection
		add_detection();
	}
	else if (detection == 2)
	{
		HAL_Delay(10);//eliminate the interference of parasitic capacitance of detection
		reduce_detection();
	}
}

//check whether flow number over the limit
void check_flow(void)
{
	if (flow_num > flow_limit)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
	}
	else
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	}
}
