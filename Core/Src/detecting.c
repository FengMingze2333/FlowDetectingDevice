#include "detecting.h"
#include "gpio.h"
#include "oled.h"
#include <stdio.h>

extern int flow_num;
extern int flow_limit;
extern int time_limit;
extern int time_count;
extern uint8_t detection;
extern TIM_HandleTypeDef htim2;
static int time_refresh;

//refresh main page
static void detection_show(void)
{
	oled_area_clear(0, 16, 128, 16);
	static char flow_num_char[8];
	sprintf(flow_num_char, "%d", flow_num);
	oled_draw_ASCII(120, 16, flow_num_char, SET, RIGHT);

	oled_area_clear(0, 48, 128, 16);
	static char time_limit_char[8];
	sprintf(time_limit_char, "%d", time_limit);
	oled_draw_ASCII(0, 48, "MaxTime:", SET, LEFT);
	oled_draw_ASCII(120, 48, time_limit_char, SET, RIGHT);
}

//start timer
static void start_timer(void)
{
	HAL_TIM_Base_Init(&htim2);
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
	time_count = 0;
	time_refresh = -1;
	oled_area_clear(0, 48, 128, 16);
	start_timer();
	while (detection == 0)
	{
		if (time_count >= time_limit)
		{
			if (time_refresh != time_count)
			{
				oled_area_clear(40, 48, 88, 16);

				static char time_current_char[8];
				sprintf(time_current_char, "%d", time_limit-time_count);
				oled_draw_ASCII(0, 48, "Time:", SET, LEFT);
				oled_draw_ASCII(120, 48, time_current_char, SET, RIGHT);

				time_refresh = time_count;
			}

			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET);
			HAL_Delay(200);
		}
		else
		{
			if (time_refresh != time_count)
			{
				oled_area_clear(40, 48, 88, 16);

				static char time_current_char[8];
				sprintf(time_current_char, "%d", time_limit-time_count);
				oled_draw_ASCII(0, 48, "Time:", SET, LEFT);
				oled_draw_ASCII(120, 48, time_current_char, SET, RIGHT);

				time_refresh = time_count;
			}

			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
		}
	}
	stop_timer();
	if (detection == 2)
		flow_num++;
	detection = 0;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	detection_show();
}

//reduce flow number detection
static void reduce_detection(void)
{
	detection = 0;
	time_count = 0;
	time_refresh = -1;
	oled_area_clear(0, 48, 128, 16);
	start_timer();
	while (detection == 0)
	{
		if (time_count >= time_limit)
		{
			if (time_refresh != time_count)
			{
				oled_area_clear(40, 48, 88, 16);

				static char time_current_char[8];
				sprintf(time_current_char, "%d", time_limit-time_count);
				oled_draw_ASCII(0, 48, "Time:", SET, LEFT);
				oled_draw_ASCII(120, 48, time_current_char, SET, RIGHT);

				time_refresh = time_count;
			}

			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET);
			HAL_Delay(200);
		}
		else
		{
			if (time_refresh != time_count)
			{
				oled_area_clear(40, 48, 88, 16);

				static char time_current_char[8];
				sprintf(time_current_char, "%d", time_limit-time_count);
				oled_draw_ASCII(0, 48, "Time:", SET, LEFT);
				oled_draw_ASCII(120, 48, time_current_char, SET, RIGHT);

				time_refresh = time_count;
			}

			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
		}
	}
	stop_timer();
	if (detection == 1)
		flow_num--;
	detection = 0;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	detection_show();
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
void check_flow_num(void)
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
