#include "oled.h"
#include "i2c.h"
#include "font.h"
#include <string.h>

#define I2Cx hi2c1

const uint16_t display_width = 128;
const uint16_t display_height = 64;
static uint8_t OLED_RAM[64/8][128];

//send command
static void send_cmd(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

//send data
static void send_data(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

//send RAM
static void send_RAM(void)
{
	for(uint16_t i = 0; i < display_height/8; i++)
	{
		send_cmd(0xB0+i);
		send_cmd(0x00);
		send_cmd(0x10);
		for(uint16_t j = 0; j < display_width; j++)
		{
				send_data(OLED_RAM[i][j]);
		}
	}
}

//invert string
static void reverse_char(char arr[])
{
	int left = 0;
	int right = strlen(arr)-1;
	while (left < right)
	{
		char temp = arr[left];
		arr[left] = arr[right];
		arr[right] = temp;
		left++;
		right--;
	}

}

//initialize OLED
void oled_init(void)
{
	HAL_Delay(100);

	send_cmd(0xAE);
	send_cmd(0x20);
	send_cmd(0x10);
	send_cmd(0xB0);
	send_cmd(0x00);
	send_cmd(0x10);
	send_cmd(0xC8);
	send_cmd(0x40);
	send_cmd(0x81);
	send_cmd(0xFF);
	send_cmd(0xA1);
	send_cmd(0xA6);
	send_cmd(0xA8);
	send_cmd(0x3F);
	send_cmd(0xA4);
	send_cmd(0xD3);
	send_cmd(0x00);
	send_cmd(0xD5);
	send_cmd(0xF0);
	send_cmd(0xD9);
	send_cmd(0x22);
	send_cmd(0xDA);
	send_cmd(0x12);
	send_cmd(0xDB);
	send_cmd(0x20);
	send_cmd(0x8D);
	send_cmd(0x14);
	send_cmd(0xAF);

	oled_clear();
}

//turn on OLED
void oled_on(void)
{
	send_cmd(0X8D);
	send_cmd(0X14);
	send_cmd(0XAF);
}

//turn off OLED
void oled_off(void)
{
	send_cmd(0X8D);
	send_cmd(0X10);
	send_cmd(0XAE);
}


//enable pixel
void oled_set_pixel(int16_t x, int16_t y)
{
	if (x >= 0 && x < display_width && y >= 0 && y < display_height)
	{
		OLED_RAM[y/8][x] |= (0x01 << (y%8));
	}
}

//disable pixel
void oled_reset_pixel(int16_t x, int16_t y)
{
	if (x >= 0 && x < display_width && y >= 0 && y < display_height)
	{
		OLED_RAM[y/8][x] &= ~(0x01 << (y%8));
	}
}

//fill area
void oled_area_fill(int16_t x0, int16_t y0, int16_t w, int16_t h)
{
	if (x0 >= 0 && x0+w <= display_width && y0 >= 0 && y0+h <= display_height)
	{
		for(int16_t y = y0; y < y0+h; y++)
		{
			for(int16_t x = x0; x < x0+w; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
						oled_set_pixel(x, y);
					}
			}
		}
		send_RAM();
	}
}

//clear area
void oled_area_clear(int16_t x0, int16_t y0, int16_t w, int16_t h)
{
	if (x0 >= 0 && x0+w <= display_width && y0 >= 0 && y0+h <= display_height)
	{
		for(int16_t y = y0; y < y0+h; y++)
		{
			for(int16_t x = x0; x < x0+w; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
						oled_reset_pixel(x, y);
					}
			}
		}
		send_RAM();
	}
}

//fully fill OLED
void oled_fill(void)
{
	oled_area_fill(0, 0, display_width, display_height);
}

//fully clear OLED
void oled_clear(void)
{
	oled_area_clear(0, 0, display_width, display_height);
}

//draw ASCII character
void oled_draw_ASCII(int16_t x, int16_t y, char arr[], FlagStatus mode, FlushStatus flush)
{
	if (x >= 0 && x <= display_width && y >= 0 && y <= display_height) {
		int32_t c = 0;
		uint8_t j = 0;
		if (flush)
		{
			reverse_char(arr);
		}
		while(arr[j] != '\0')
		{
			c = arr[j] - 32;
			if(c < 0)	//undefined character
				break;
			if(128-x < 8)//warp draw
			{
				x = 0;
				y += 16;
				if(64 - y < 16)	//no more space,break
					break;
			}
			for(uint8_t m = 0; m < 2; m++)
			{
				for(uint8_t n = 0; n < 8; n++)
				{
					for(uint8_t i = 0; i < 8; i++)
					{
						if((ASCII[c][n+m*8] >> i) & 0x01){
							if(mode)
							{
								oled_set_pixel(x+n, y+i+m*8);
							}
							else
							{
								oled_reset_pixel(x+n, y+i+m*8);
							}
						}
					}
				}
			}
			if (flush)
			{
				x -= 8;
			}
			else
			{
				x += 8;
			}
			j++;
		}
	}
	send_RAM();
}
