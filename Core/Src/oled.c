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
		send_cmd(0xB0+i);	//设置页地址b0~b7
		send_cmd(0x00);		//设置显示位置—列低地址00-0f
		send_cmd(0x10);		//设置显示位置—列高地址10-1f
		for(uint16_t j = 0; j < display_width; j++)
		{
				send_data(OLED_RAM[i][j]);
		}
	}
}

//invert string
static void reverse(char arr[])
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

	send_cmd(0xAE); //关闭显示
	send_cmd(0x20);	//设置内存寻址模式
	send_cmd(0x10);	//页面寻址模式
	send_cmd(0xB0);	//页面开始地址
	send_cmd(0x00); //低列地址
	send_cmd(0x10); //高列地址
	send_cmd(0xC8);	//设置COM输出扫描方向
	send_cmd(0x40); //设置起始行地址
	send_cmd(0x81); //设置对比度
	send_cmd(0xFF);
	send_cmd(0xA1); //设置段重映射
	send_cmd(0xA6); //设置正常显示
	send_cmd(0xA8); //设置复用比
	send_cmd(0x3F);
	send_cmd(0xA4); //恢复内存内容显示
	send_cmd(0xD3); //设置显示补偿
	send_cmd(0x00);
	send_cmd(0xD5); //设置显示时钟分比率/振荡器频率
	send_cmd(0xF0);
	send_cmd(0xD9); //设置预充电间隔
	send_cmd(0x22);
	send_cmd(0xDA); //设置COM引脚硬件配置
	send_cmd(0x12);
	send_cmd(0xDB); //设置VCOMH电压
	send_cmd(0x20);
	send_cmd(0x8D); //设置电荷泵
	send_cmd(0x14); //开启电荷泵
	send_cmd(0xAF); //打开显示

	oled_clear();
}

//on OLED
void oled_on(void)
{
	send_cmd(0X8D);  //设置电荷泵
	send_cmd(0X14);  //开启电荷泵
	send_cmd(0XAF);  //打开显示
}

//off OLED
void oled_off(void)
{
	send_cmd(0X8D);  //设置电荷泵
	send_cmd(0X10);  //关闭电荷泵
	send_cmd(0XAE);  //关闭显示
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
			reverse(arr);
		}
		while(arr[j] != '\0')
		{
			c = arr[j] - 32;
			if(c < 0)	//无效字符
				break;
			if(128-x < 8)//不能显示完整字符，换行显示
			{
				x = 0;
				y += 16;
				if(64 - y < 16)	//不能显示一行时不显示
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
