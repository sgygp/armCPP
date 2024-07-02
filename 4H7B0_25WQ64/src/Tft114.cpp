/*
 * Tft114.cpp
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */

#include "Tft114.h"

#include "string.h"

void Tft114::MX_SPI6_Init(void) {

	memset(&hspi6,0,sizeof(hspi6));

	hspi6.Instance = SPI6;
	hspi6.Init.Mode = SPI_MODE_MASTER;
	hspi6.Init.Direction = SPI_DIRECTION_1LINE;
	hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi6.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi6.Init.NSS = SPI_NSS_SOFT;
	hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi6.Init.CRCPolynomial = 0x0;
	hspi6.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi6.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		Error_Handler();
	}

}

void Tft114::init() {
	MX_SPI6_Init();

	GPIO_InitTypeDef GPIO_InitStruct;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);


	/*Configure GPIO pins : PDPin PDPin */
	GPIO_InitStruct.Pin = LCD_PWR_Pin|LCD_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_PWR_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = LCD_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);

	LCD_PWR(0);
	HAL_Delay(20);
	LCD_RST(0);
	HAL_Delay(20);
	LCD_RST(1);

	HAL_Delay(20);
	/* Sleep Out */
	LCD_Write_Cmd(0x11);
	/* wait for power stability */
	HAL_Delay(20);

	/* Memory Data Access Control */
	LCD_Write_Cmd(0x36);
	if(USE_HORIZONTAL==0)LCD_Write_Data(0x00);
	else if(USE_HORIZONTAL==1)LCD_Write_Data(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Write_Data(0x70);
	else LCD_Write_Data(0xA0);

	/* RGB 5-6-5-bit  */
	LCD_Write_Cmd(0x3A);
	LCD_Write_Data(0x05);

	/* Porch Setting */
	LCD_Write_Cmd(0xB2);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x33);
	LCD_Write_Data(0x33);

	/*  Gate Control */
	LCD_Write_Cmd(0xB7);
	LCD_Write_Data(0x35);

	/* VCOM Setting */
	LCD_Write_Cmd(0xBB);
	LCD_Write_Data(0x19);   //Vcom=1.625V

	/* LCM Control */
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x2C);

	/* VDV and VRH Command Enable */
	LCD_Write_Cmd(0xC2);
	LCD_Write_Data(0x01);

	/* VRH Set */
	LCD_Write_Cmd(0xC3);
	LCD_Write_Data(0x12);

	/* VDV Set */
	LCD_Write_Cmd(0xC4);
	LCD_Write_Data(0x20);

	/* Frame Rate Control in Normal Mode */
	LCD_Write_Cmd(0xC6);
	LCD_Write_Data(0x0F);	//60MHZ

	/* Power Control 1 */
	LCD_Write_Cmd(0xD0);
	LCD_Write_Data(0xA4);
	LCD_Write_Data(0xA1);

	/* Positive Voltage Gamma Control */
	LCD_Write_Cmd(0xE0);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0x4C);
	LCD_Write_Data(0x18);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x23);

	/* Negative Voltage Gamma Control */
	LCD_Write_Cmd(0xE1);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x44);
	LCD_Write_Data(0x51);
	LCD_Write_Data(0x2F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x20);
	LCD_Write_Data(0x23);

	/* Display Inversion On */
	LCD_Write_Cmd(0x21);

	LCD_Write_Cmd(0x29);

	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

	/*打开显示*/
	LCD_PWR(1);


}
void Tft114::LCD_DisplayOn(void) {
	LCD_PWR(0);
}

void Tft114::LCD_DisplayOff(void) {
	LCD_PWR(1);
}

void Tft114::LCD_Write_HalfWord(const uint16_t da) {
	uint8_t data[2] = {0};

	data[0] = da >> 8;
	data[1] = da;

	LCD_DC(1);
	LCD_SPI_Send(data, 2);
}

void Tft114::LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2) {
	if(USE_HORIZONTAL==0)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+52);
		LCD_Write_HalfWord(x2+52);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+53);
		LCD_Write_HalfWord(x2+53);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+53);
		LCD_Write_HalfWord(y2+53);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+52);
		LCD_Write_HalfWord(y2+52);
		LCD_Write_Cmd(0x2c);//储存器写
	}
}

void Tft114::LCD_Clear(uint16_t color) {
	LCD_Fill(0,0,LCD_Width,LCD_Height,color);
}

void Tft114::LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end,
		uint16_t y_end, uint16_t color) {
	uint16_t i = 0;
	uint32_t size = 0, size_remain = 0;

	size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

	if(size > LCD_Buf_Size)
	{
		size_remain = size - LCD_Buf_Size;
		size = LCD_Buf_Size;
	}

	LCD_Address_Set(x_start, y_start, x_end, y_end);

	while(1)
	{
		for(i = 0; i < size / 2; i++)
		{
			lcd_buf[2 * i] = color >> 8;
			lcd_buf[2 * i + 1] = color;
		}

		LCD_DC(1);
		LCD_SPI_Send(lcd_buf, size);

		if(size_remain == 0)
			break;

		if(size_remain > LCD_Buf_Size)
		{
			size_remain = size_remain - LCD_Buf_Size;
		}

		else
		{
			size = size_remain;
			size_remain = 0;
		}
	}
}

void Tft114::LCD_Draw_Point(uint16_t x, uint16_t y) {
	LCD_Address_Set(x, y, x, y);
	LCD_Write_HalfWord(POINT_COLOR);
}
void Tft114::LCD_Draw_Point(uint16_t x, uint16_t y,uint8_t t) {
	LCD_Address_Set(x, y, x, y);
	if(t==1)
		LCD_Write_HalfWord(POINT_COLOR);
	if(t==0)
		LCD_Write_HalfWord(BACK_COLOR);
}

void Tft114::LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color) {
	LCD_Address_Set(x, y, x, y);
	LCD_Write_HalfWord(color);
}

void Tft114::LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, row, col;
	uint16_t i = 0;

	if(y1 == y2)
	{
		/*快速画水平线*/
		LCD_Address_Set(x1, y1, x2, y2);

		for(i = 0; i < x2 - x1; i++)
		{
			lcd_buf[2 * i] = POINT_COLOR >> 8;
			lcd_buf[2 * i + 1] = POINT_COLOR;
		}

		LCD_DC(1);
		LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
		return;
	}

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	row = x1;
	col = y1;

	if(delta_x > 0)incx = 1;

	else if(delta_x == 0)incx = 0;

	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	if(delta_y > 0)incy = 1;

	else if(delta_y == 0)incy = 0;

	else
	{
		incy = -1;
		delta_y = -delta_y;
	}

	if(delta_x > delta_y)distance = delta_x;

	else distance = delta_y;

	for(t = 0; t <= distance + 1; t++)
	{
		LCD_Draw_Point(row, col);
		xerr += delta_x ;
		yerr += delta_y ;

		if(xerr > distance)
		{
			xerr -= distance;
			row += incx;
		}

		if(yerr > distance)
		{
			yerr -= distance;
			col += incy;
		}
	}
}

void Tft114::LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2) {
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}

void Tft114::LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);

	while(a <= b)
	{
		LCD_Draw_Point(x0 - b, y0 - a);
		LCD_Draw_Point(x0 + b, y0 - a);
		LCD_Draw_Point(x0 - a, y0 + b);
		LCD_Draw_Point(x0 - b, y0 - a);
		LCD_Draw_Point(x0 - a, y0 - b);
		LCD_Draw_Point(x0 + b, y0 + a);
		LCD_Draw_Point(x0 + a, y0 - b);
		LCD_Draw_Point(x0 + a, y0 + b);
		LCD_Draw_Point(x0 - b, y0 + a);
		a++;

		if(di < 0)di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}

		LCD_Draw_Point(x0 + a, y0 + b);
	}
}

void Tft114::LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size) {
	uint8_t temp, t1, t;
	uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
	uint16_t colortemp;
	uint8_t sta;

	chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

	if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

	LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

	if((size == 16) || (size == 32) )	//16和32号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			if(size == 16)
				temp = asc2_1608[u8(chr)][t];	//调用1608字体
			else if(size == 32)
				temp = asc2_3216[u8(chr)][t];	//调用3216字体
			else return;			//没有的字库

			for(t1 = 0; t1 < 8; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}

	else if  (size == 12)	//12号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			temp = asc2_1206[u8(chr)][t];

			for(t1 = 0; t1 < 6; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}

	else if(size == 24)		//24号字体
	{
		csize = (size * 16) / 8;

		for(t = 0; t < csize; t++)
		{
			temp = asc2_2412[u8(chr)][t];

			if(t % 2 == 0)sta = 8;
			else sta = 4;

			for(t1 = 0; t1 < sta; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}
}

void Tft114::LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
		uint8_t size) {
	uint8_t t, temp;
	uint8_t enshow = 0;

	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;

		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x + (size / 2)*t, y, ' ', size);
				continue;
			}

			else enshow = 1;
		}

		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
	}
}

void Tft114::LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
		uint8_t size, uint8_t mode) {
	uint8_t t, temp;
	uint8_t enshow = 0;

	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;

		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				if(mode)LCD_ShowChar(x + (size / 2)*t, y, '0', size);
				else
					LCD_ShowChar(x + (size / 2)*t, y, ' ', size);

				continue;
			}

			else enshow = 1;
		}

		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
	}
}

void Tft114::LCD_ShowString(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, uint8_t size, char* p) {
	uint8_t x0 = x;
	width += x;
	height += y;

	while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
	{
		if(x >= width)
		{
			x = x0;
			y += size;
		}

		if(y >= height)break; //退出

		LCD_ShowChar(x, y, *p, size);
		x += size / 2;
		p++;
	}
}

void Tft114::LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, const uint8_t* p) {
	if(x + width > LCD_Width || y + height > LCD_Height)
	{
		return;
	}

	LCD_Address_Set(x, y, x + width - 1, y + height - 1);
	for(int i=0;i<width*height/8;i++)
	{
		u8 temp=p[i];
		u16 colortemp;
		for(int t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)
				colortemp = POINT_COLOR;
			else
				colortemp = BACK_COLOR;

			LCD_Write_HalfWord(colortemp);
			temp <<= 1;
		}

	}
}


uint8_t Tft114::LCD_SPI_Send(uint8_t* data, uint16_t size)
{
	return HAL_SPI_Transmit(&hspi6,data,size, 1000);
}

void Tft114::LCD_Write_Cmd(uint8_t cmd)
{
	LCD_DC(0);

	LCD_SPI_Send(&cmd, 1);
}

void Tft114::LCD_Write_Data(uint8_t data)
{
	LCD_DC(1);

	LCD_SPI_Send(&data, 1);
}

void Tft114::LCD_Show_HZ16(uint16_t x, uint16_t y, uint8_t geshu,
		const uint8_t* p) {
	for(int i=0;i<geshu;i++)
		LCD_Show_Image(x+16*i,y,16,16,p+32*i);
}

uint32_t Tft114::LCD_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;

	while(n--)result *= m;

	return result;
}
