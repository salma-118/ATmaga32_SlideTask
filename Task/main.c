#include "MCAL/inc/Atmega32.h"
#include "MCAL/inc/USART.h"
#include "MCAL/inc/SPI.h"

#define START_BYTE 0xAA

void USART_Send2Digits(uint8 value)
{
    USART_send((value / 10) + '0');
    USART_send((value % 10) + '0');
}

int main(void)
{
    uint8 data;

    uint8 day;
    uint8 month;
    uint8 year;

    uint8 hour;
    uint8 minute;
    uint8 second;

    UART_Config_t UART_Config;

    USART_init(&UART_Config);
    SPI_Init();

    USART_sendString((uint8*)"ATmega32 Ready\r\n");

    while (1)
    {
        /* Wait for start byte */
        do
        {
            data = SPI_SendRecieveData(0x00);
        }
        while (data != START_BYTE);

        /* Date */
        day = SPI_SendRecieveData(0x00);
        month = SPI_SendRecieveData(0x00);
        year = SPI_SendRecieveData(0x00);

        /* Time */
        hour = SPI_SendRecieveData(0x00);
        minute = SPI_SendRecieveData(0x00);
        second = SPI_SendRecieveData(0x00);

        /* Display */
        USART_sendString((uint8*)"Date: ");

        USART_Send2Digits(day);
        USART_send('/');
        USART_Send2Digits(month);
        USART_send('/');
        USART_sendString((uint8*)"20");
        USART_Send2Digits(year);

        USART_sendString((uint8*)"  Time: ");

        USART_Send2Digits(hour);
        USART_send(':');
        USART_Send2Digits(minute);
        USART_send(':');
        USART_Send2Digits(second);

        USART_sendString((uint8*)"\r\n");
    }
}