/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include <delay.h>
#include "uart.h"
#include "dht22.h"
			
uint32_t response;
uint16_t humidity,temperature;

int main(void)
{

	UART_Init();
	UART_SendStr("\nSTM32F103RET6 is online.\n");

		while (1) {
			DHT22_Init();
			response = DHT22_GetReadings();
			if (response != DHT22_RCV_OK) {
				UART_SendStr("DHT22_GetReadings() error = ");
				UART_SendInt(response); UART_SendChar('\n');
			} else {
				response = DHT22_DecodeReadings();
				UART_SendStr("Parity: Received = "); UART_SendInt(response & 0xff);
				UART_SendStr(" Actual = "); UART_SendInt(response >> 8);
				UART_SendChar('\n');
				if ((response & 0xff) != (response >> 8)) {
					UART_SendStr("Wrong data received.\n");
				} else {
					temperature = DHT22_GetTemperature();
					humidity = DHT22_GetHumidity();

					UART_SendStr("Humidity: ");
					UART_SendInt(humidity / 10); UART_SendChar('.');
					UART_SendInt(humidity % 10); UART_SendStr("%RH");
					UART_SendChar('\n');
					UART_SendStr("Temperature: ");
					if ((temperature & 0x8000) != 0) UART_SendChar('-');
					UART_SendInt((temperature & 0x7fff) / 10); UART_SendChar('.');
					UART_SendInt((temperature & 0x7fff) % 10); UART_SendStr("C");
					UART_SendChar('\n');
				}
			}

			Delay_ms(1000);
		}
}
