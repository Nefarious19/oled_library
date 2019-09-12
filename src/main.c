
#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Drivers/PERIPHERALS/inc/stm32f303_uart.h"
#include "../Drivers/PERIPHERALS/inc/stm32f303_i2c.h"
#include "../Drivers/PERIPHERALS/inc/stm32f303_spi.h"
#include "../BME280/bme280_defs.h"
#include "utils.h"

#include "../SH1106/inc/oled.h"
#include "../SH1106/icons/icons.h"

volatile uint32_t timer = 0;

void SysTick_Handler (void)
{
	if(timer) timer --;
}

int main(void)
{
  RCC_Init();
  GPIO_init();
  SysTick_Config(72000000UL / 1000);

//  I2C_init();
//  OLED_CommunicationInterface_t interface;
//  i2cForOled.OLED_InterfaceWriteNBytes = I2C_writeNBytes;

  SPI_Init();
  OLED_CommunicationInterface_t interface = { 0 };
  interface.OLED_InterfaceWriteNBytes = SPI_writeNData;
  interface.OLED_chipSelectHigh = SPI_csHigh;
  interface.OLED_chipSelectLow = SPI_csLow;
  interface.OLED_comandMode = SPI_cdLow;
  interface.OLED_dataMode = SPI_cdHIGH;
  OLED_init(&interface);

  OLED_GFX_drawRoundedRect(0,0, OLED_WIDTH - 1, OLED_HEIGHT - 1, 5, OLED_COLOR_WHITE);
  OLED_GFX_drawRoundedRect(0,0, OLED_WIDTH - 1, 10, 5, OLED_COLOR_WHITE);
  OLED_GFX_loadSmallBitmap(wifi_lvl_3_8x8, 8, 8, 10, 1);
  OLED_GFX_loadSmallBitmap(message_8x8,8,8,20,1);
  OLED_PRT_putStr("Hello world!",10,OLED_LINE2+2, 1);

  OLED_updateScreen();

  while (1)
  {
	  if(!timer)
	  {
		  static uint32_t shift = 0x0100;
		  timer = 100;
		  GPIOE->ODR &= 0x00FF;
		  GPIOE->ODR |= shift;
		  shift <<= 1;
		  if(shift == 0x10000) shift = 0x0100;

	  }
  }
}



