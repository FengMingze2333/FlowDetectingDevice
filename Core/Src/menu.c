#include <detection.h>
#include "menu.h"
#include "gpio.h"
#include "oled.h"
#include <stdio.h>

extern int flow_num;//flow number
extern int flow_limit;//flow number limit
extern int time_limit;//stay time limit
extern uint8_t menu;//menu selection
static uint8_t menu_current;//current menu selection

//scan number keyboard
static uint8_t scan_num_key(void)
{
	HAL_GPIO_WritePin(NUM_KEY_O1_GPIO_Port, NUM_KEY_O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NUM_KEY_O2_GPIO_Port, NUM_KEY_O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O3_GPIO_Port, NUM_KEY_O3_Pin, GPIO_PIN_SET);
	HAL_Delay(10);//eliminate the interference of parasitic capacitance of keyboard
	if (HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET);
		return 14;//#
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 9;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I3_GPIO_Port, NUM_KEY_I3_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 6;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I4_GPIO_Port, NUM_KEY_I4_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 3;
	}

	HAL_GPIO_WritePin(NUM_KEY_O1_GPIO_Port, NUM_KEY_O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O2_GPIO_Port, NUM_KEY_O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NUM_KEY_O3_GPIO_Port, NUM_KEY_O3_Pin, GPIO_PIN_SET);
	HAL_Delay(10);//eliminate the interference of parasitic capacitance of keyboard
	if (HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET);
		return 0;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 8;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I3_GPIO_Port, NUM_KEY_I3_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 5;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I4_GPIO_Port, NUM_KEY_I4_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 2;
	}

	HAL_GPIO_WritePin(NUM_KEY_O1_GPIO_Port, NUM_KEY_O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O2_GPIO_Port, NUM_KEY_O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O3_GPIO_Port, NUM_KEY_O3_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);//eliminate the interference of parasitic capacitance
	if (HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I1_GPIO_Port, NUM_KEY_I1_Pin) == GPIO_PIN_RESET);
		return 13;//*
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 7;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I3_GPIO_Port, NUM_KEY_I3_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 4;
	}
	if (HAL_GPIO_ReadPin(NUM_KEY_I4_GPIO_Port, NUM_KEY_I4_Pin) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);//eliminate buffering of keyboard
		while(HAL_GPIO_ReadPin(NUM_KEY_I2_GPIO_Port, NUM_KEY_I2_Pin) == GPIO_PIN_RESET);
		return 1;
	}

	HAL_GPIO_WritePin(NUM_KEY_O1_GPIO_Port, NUM_KEY_O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O2_GPIO_Port, NUM_KEY_O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NUM_KEY_O3_GPIO_Port, NUM_KEY_O3_Pin, GPIO_PIN_SET);
	HAL_Delay(10);//eliminate the interference of parasitic capacitance of keyboard

	return 15;
}

//get number keyboard input
static int get_num_key(int num_current)
{
	uint8_t num_mode = 0;
	uint8_t num_key = 15;
	int num_input = 0;

	if (num_current > 99999999)
		num_current = 99999999;

	oled_area_clear(0, 16, 128, 48);

	char num_current_char[8];
	sprintf(num_current_char, "%d", num_current);
	oled_draw_ASCII(0, 16, "Current Value:", SET, LEFT);
	oled_draw_ASCII(120, 32, num_current_char, SET, RIGHT);
	oled_draw_ASCII(0, 48, "Reset", SET, LEFT);
	oled_draw_ASCII(120, 48, "0", SET, RIGHT);

	while (HAL_GPIO_ReadPin(ENTER_KEY_GPIO_Port, ENTER_KEY_Pin) == GPIO_PIN_SET){
		//check whether change the menu
		if (menu != menu_current)
			return num_current;

		num_key = scan_num_key();
		if (num_key == 14)//#
		{
			num_input = 0;

			oled_area_clear(40, 48, 88, 16);
			oled_draw_ASCII(120, 48, "0", SET, RIGHT);
		}
		if (num_key == 13)//*
		{
			if (num_mode == 0)
			{
				num_input = 0;
				num_mode = 1;

				oled_area_clear(0, 48, 128, 16);
				oled_draw_ASCII(0, 48, "+", SET, LEFT);
				oled_draw_ASCII(120, 48, "0", SET, RIGHT);
			}
			else if (num_mode == 1)
			{
				num_input = 0;
				num_mode = 2;

				oled_area_clear(0, 48, 128, 16);
				oled_draw_ASCII(0, 48, "-", SET, LEFT);
				oled_draw_ASCII(120, 48, "0", SET, RIGHT);
			}
			else
			{
				num_input = 0;
				num_mode = 0;

				oled_area_clear(0, 48, 128, 16);
				oled_draw_ASCII(0, 48, "Reset", SET, LEFT);
				oled_draw_ASCII(120, 48, "0", SET, RIGHT);
			}
		}
		if (num_key < 10)
		{
			num_input = 10*num_input + num_key;

			if (num_input >= 99999999)
			{
				num_input = 99999999;

				oled_draw_ASCII(40, 48, "Max", SET, LEFT);
				oled_area_clear(64, 48, 64, 16);
			}
			else
				oled_area_clear(40, 48, 88, 16);

			char num_input_char[8];
			sprintf(num_input_char, "%d", num_input);
			oled_draw_ASCII(120, 48, num_input_char, SET, RIGHT);
		}
		HAL_Delay(100);
	}

	if (num_mode == 1)
	{
		if (num_current+num_input > 99999999)
			return 99999999;
		else
			return num_current+num_input;
	}
	else if (num_mode == 2)
	{
		if (num_current-num_input < 0)
			return 0;
		else
			return num_current-num_input;
	}
	else
		return num_input;
}

//set current flow number
static void set_flow_num(void)
{
	menu_current = menu;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	oled_clear();
	oled_draw_ASCII(0, 0, "Set Flow Number", SET, LEFT);
	flow_num = get_num_key(flow_num);
	oled_clear();
	if (menu_current == menu)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		menu = 0;
		main_show();
	}
	else
		menu_check_selection();
}

//set flow number limit
static void set_flow_limit(void)
{
	menu_current = menu;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	oled_clear();
	oled_draw_ASCII(0, 0, "Set Flow Limit", SET, LEFT);
	flow_limit = get_num_key(flow_limit);
	oled_clear();
	if (menu_current == menu)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		menu = 0;
		main_show();
	}
	else
		menu_check_selection();
}

//set stay time limit
static void set_time_limit(void)
{
	menu_current = menu;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET);
	oled_clear();
	oled_draw_ASCII(0, 0, "Set Time Limit", SET, LEFT);
	time_limit = get_num_key(time_limit);
	oled_clear();
	if (menu_current == menu)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		menu = 0;
		main_show();
	}
	else
		menu_check_selection();
}

//check whether select menu
void menu_check_selection(void)
{
	if (menu == 1)
		set_flow_num();
	else if (menu == 2)
		set_flow_limit();
	else if (menu == 3)
		set_time_limit();
}
